#include "menu.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "i2clcd.h"
#include "hotend_control.h"
#include "noisuy.h"
#include <math.h>

KeyBoard_TypeDef board = {0,0, KEY_DISABLE,0,0, 0};
Manual_Form_TypeDef manual = {MANUAL_FORM_DISABLE};
MenuItem *currentMenu;
MenuItem displayMenu;
MenuItem mainMenu;
MenuItem menuConfig;
MenuItem menuPIDParameter;
MenuItem menuSetKpHotend;
MenuItem menuSetKiHotend;
MenuItem menuSetKdHotend;
MenuItem menuSetKpHedbed;
MenuItem menuSetKiHedbed;
MenuItem menuSetKdHedbed;
MenuItem menuMotorParameter;
MenuItem menuXStep;
MenuItem menuYStep;
MenuItem menuZStep;
MenuItem menuEStep;
MenuItem menuConTrol;
MenuItem menuMoveX;
MenuItem menuMoveY;
MenuItem menuMoveZ;
MenuItem menuMoveE;
MenuItem menuSetHome;
MenuItem menuAutoHome;
MenuItem menuSetFanPower;
MenuItem menuSetTempHotTend;
MenuItem menuSetTempHedBed;
MenuItem menuSave;
form1_TypeDef form1;
uint8_t button = BUTTON_STOP;
int cnt = 0;
char msg[48];
float Kp = 1.23;
float Ki = 3.24;
float Kd = 2.23;
float setPos = 0.0;
float curPos = 0.0;
float setVel = 0.0;
float curVel = 0.0;
float dt = 0.01;
float Buoc_vit = 8;
uint8_t start = 0;
MenuItem* createMenuItem(MenuItem *newItem, const char* name, void (*function)()) {

    strcpy(newItem->name, name);
    newItem->function = *function;
	for( uint8_t i = 0; i<STACK_SIZE; i++){
		newItem->itemStack[i] = NULL;
		}
    newItem->pre = NULL;
		newItem->choose = 1;
		newItem->stackNum = 0;
		newItem->index = 1;
		newItem->start = 1;
		newItem->offset = 0;
    return newItem;
}
void addMenuItem( MenuItem *parentMenu, MenuItem *childMenu, uint8_t stackIndex){
	if(stackIndex < STACK_SIZE -1){
		parentMenu->itemStack[stackIndex] = childMenu;
	  //Lien ket menu con voi menu cha
	  childMenu->pre = parentMenu;
		parentMenu->stackNum++;
		}
	}
void previousMenuReturn( MenuItem *curMenu){
	if(curMenu->pre != NULL){
		curMenu = curMenu->pre;
	}
}
volatile int32_t encoder_position = 0; // Giá tr? v? trí encoder
volatile int32_t last_encoder_position = 0;
uint8_t last_state_A = 0;              // Tr?ng thái tru?c dó c?a tín hi?u A
void Encoder_Rotary_Init(void){
	  last_state_A = HAL_GPIO_ReadPin(GPIO_CLK_PORT, GPIO_CLK_PIN);
}
void Encoder_Read(void) {
    uint8_t current_state_A = HAL_GPIO_ReadPin(GPIO_CLK_PORT, GPIO_CLK_PIN); // Ð?c tín hi?u A
    uint8_t current_state_B = HAL_GPIO_ReadPin(GPIO_DT_PORT, GPIO_DT_PIN); // Ð?c tín hi?u B

    // Ki?m tra thay d?i tr?ng thái tín hi?u A
    if (current_state_A != last_state_A) {
        // N?u tín hi?u A thay d?i, xác d?nh hu?ng quay d?a vào tín hi?u B
        if (current_state_A == GPIO_PIN_SET) {
            // A t? LOW -> HIGH
            if (current_state_B == GPIO_PIN_RESET) {
                encoder_position++; // Quay thu?n
            } else {
                encoder_position--; // Quay ngu?c
            }
        } else {
            // A t? HIGH -> LOW
            if (current_state_B == GPIO_PIN_SET) {
                encoder_position++; // Quay thu?n
            } else {
                encoder_position--; // Quay ngu?c
            }
        }
        last_state_A = current_state_A; // C?p nh?t tr?ng thái tru?c dó
    }
}
int menuDisplay( MenuItem * menu){
	uint8_t y = 0;
	menu->offset = menu->start;
	for(uint8_t i = 0; i< 4; i++){
		if(menu->offset > menu->stackNum )sprintf(msg, "             ");
		
		else {
			if(menu->offset == menu->choose)sprintf(msg, ">%d. %s", menu->offset, menu->itemStack[menu->offset]->name);
			else sprintf(msg, " %d. %s", menu->offset, menu->itemStack[menu->offset]->name);
			}
		lcd_write(msg, y, 0);
		y+=1;
		menu->offset+=1;
		}
	if(menu == &displayMenu){
		menu->function();
		}
	return 0;
	}
void menuSetUp(void){
	createMenuItem(&displayMenu, "", &DisplayMainMenu);
	createMenuItem(&mainMenu, "Main", NULL);
	createMenuItem(&menuConfig, "Configuration", NULL);
	createMenuItem(&menuPIDParameter, "PID Parameter", NULL);
	sprintf(msg, "Kp_Hotend: %.1f", Kp_THotend);
	createMenuItem(&menuSetKpHotend, msg, NULL);
	createMenuItem(&menuSetKiHotend, "Ki_Hotend", NULL);
	sprintf(msg, "Ki_Hotend: %.1f", Ki_THotend);
	createMenuItem(&menuSetKdHotend, msg, NULL);
	sprintf(msg, "Kd_Hotend: %.1f", Kd_THotend);
	createMenuItem(&menuSetKpHedbed, "Kp_Hedbed", NULL);
	createMenuItem(&menuSetKiHedbed, "Ki_Hedbed", NULL);
	createMenuItem(&menuSetKdHedbed, "Kd_Hedbed", NULL);
	createMenuItem(&menuMotorParameter, "Motor Reoulution", NULL);
	sprintf(msg, "XStep: %.4f", BLU);
	createMenuItem(&menuXStep, msg, NULL);
	sprintf(msg, "YStep: %.4f", BLU);
	createMenuItem(&menuYStep, msg, NULL);
	sprintf(msg, "ZStep: %.6f", BLU_Z);
	createMenuItem(&menuZStep, msg, NULL);
	sprintf(msg, "EStep: %.4f", BLU_E);
	createMenuItem(&menuEStep, msg, NULL);
	createMenuItem(&menuConTrol, "Control", NULL);
	createMenuItem(&menuMoveX, "Move X", NULL);
	createMenuItem(&menuMoveY, "Move Y", NULL);
	createMenuItem(&menuMoveZ, "Move Z", NULL);
	createMenuItem(&menuMoveE, "Move E", NULL);
	createMenuItem(&menuSetHome, "Set Home",NULL);
	createMenuItem(&menuAutoHome,"Auto Home", NULL);
	createMenuItem(&menuSetFanPower, "Set Fan Power", NULL);
	createMenuItem(&menuSetTempHotTend, "T_Hotend", NULL);
	createMenuItem(&menuSetTempHedBed, "T_HedBed", NULL);
	createMenuItem(&menuSave, "Save", NULL);
	//
	addMenuItem(&displayMenu, &mainMenu,1);
	addMenuItem(&mainMenu, &menuConfig, 1);
	addMenuItem(&mainMenu, &menuConTrol, 2);
	addMenuItem(&mainMenu, &menuSave, 3);
	addMenuItem(&menuConfig, &menuPIDParameter, 1);
	addMenuItem(&menuConfig, &menuMotorParameter, 2);
	addMenuItem(&menuPIDParameter, &menuSetKpHotend, 1);
	addMenuItem(&menuPIDParameter, &menuSetKiHotend, 2);
	addMenuItem(&menuPIDParameter, &menuSetKdHotend, 3);
	addMenuItem(&menuPIDParameter, &menuSetKpHedbed, 4);
	addMenuItem(&menuPIDParameter, &menuSetKiHedbed, 5);
	addMenuItem(&menuPIDParameter, &menuSetKdHedbed, 6);
	addMenuItem(&menuMotorParameter, &menuXStep, 1);
	addMenuItem(&menuMotorParameter, &menuYStep, 2);
	addMenuItem(&menuMotorParameter, &menuZStep, 3);
	addMenuItem(&menuMotorParameter, &menuEStep, 4);
	addMenuItem(&menuConTrol, &menuSetHome, 1);
	addMenuItem(&menuConTrol, &menuAutoHome, 2);
	addMenuItem(&menuConTrol, &menuSetFanPower, 3);
	addMenuItem(&menuConTrol, &menuSetTempHotTend, 4);
	addMenuItem(&menuConTrol, &menuSetTempHedBed, 5);
	addMenuItem(&menuSetHome, &menuMoveX, 1);
	addMenuItem(&menuSetHome, &menuMoveY, 2);
	addMenuItem(&menuSetHome, &menuMoveZ, 3);
	addMenuItem(&menuSetHome, &menuMoveE, 4);
	}
void menuControl(void){
	if(encoder_position > last_encoder_position){
		//
		if(currentMenu == &menuSetKpHotend){
			if(button == BUTTON_START)Kp_THotend+=0.1;
			if(button == BUTTON_STOP) Kp_THotend+=1;
			sprintf(msg, "Kp_HTend:%.1f", Kp_THotend);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuSetKiHotend){
			if(button == BUTTON_START)Ki_THotend+=0.1;
			if(button == BUTTON_STOP) Ki_THotend+=1;
			sprintf(msg, "Ki_HTend:%.1f", Ki_THotend);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuSetKdHotend){
			if(button == BUTTON_START)Kd_THotend+=0.001;
			if(button == BUTTON_STOP) Kd_THotend+=1;
			sprintf(msg, "Kd_HTend:%.1f", Kd_THotend);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuXStep){
			if(button == BUTTON_START)BLU+=0.001;
			if(button == BUTTON_STOP) BLU+=1;
			sprintf(msg, "XStep: %.4f", BLU);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuYStep){
			if(button == BUTTON_START)BLU+=0.001;
			if(button == BUTTON_STOP) BLU+=1;
			sprintf(msg, "YStep: %.4f", BLU);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuZStep){
			if(button == BUTTON_START)BLU_Z+=0.001;
			if(button == BUTTON_STOP) BLU_Z+=1;
			sprintf(msg, "ZStep: %.4f", BLU_Z);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuEStep){
			if(button == BUTTON_START)BLU_E+=0.001;
			if(button == BUTTON_STOP) BLU_E+=1;
			sprintf(msg, "EStep: %.4f", BLU_E);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuMoveX){
			if(button == BUTTON_START)X+=0.001;
			if(button == BUTTON_STOP) X+=1;
			setpoint1 = round(X/BLU);
			sprintf(msg, "X: %.4f", X);
			lcd_write(msg, 0,1);
		}
		else if(currentMenu == &menuMoveY){
			if(button == BUTTON_START)Y+=0.001;
			if(button == BUTTON_STOP) Y+=1;
			setpoint2 = round(Y/BLU);
			sprintf(msg, "Y: %.4f", Y);
			lcd_write(msg, 0,1);
		}
		else if(currentMenu == &menuMoveZ){
			if(button == BUTTON_START)Z+=0.001;
			if(button == BUTTON_STOP) Z+=1;
			setpoint3 = round(Z/BLU_Z);
			sprintf(msg, "Z: %.4f", Z);
			lcd_write(msg, 0,1);
		}
		else if(currentMenu == &menuMoveE){
			if(button == BUTTON_START)E+=0.1;
			if(button == BUTTON_STOP) E+=1;
			setpoint4 = round(E/BLU);
			sprintf(msg, "E: %.4f", E);
			lcd_write(msg, 0,1);
		}
		else if(currentMenu == &menuSetTempHotTend){
			if(button == BUTTON_START)setTempHotend+=0.1;
			if(button == BUTTON_STOP) setTempHotend+=1;
			sprintf(msg, "T_Hotend: %.4f", setTempHotend);
			lcd_write(msg, 0,1);
		}
		
		else{
			currentMenu->choose+=1;
			currentMenu->start+=1;
			if(currentMenu->choose > currentMenu->stackNum && currentMenu->start > currentMenu->stackNum){
				currentMenu->choose = currentMenu->stackNum;
				currentMenu->start = currentMenu->stackNum;
				}
			menuDisplay(currentMenu);
		}
		//
		last_encoder_position++;
		}
	if(encoder_position < last_encoder_position){
		//
		if(currentMenu == &menuSetKpHotend){
			if(button == BUTTON_START)Kp_THotend-=0.1;
			if(button == BUTTON_STOP) Kp_THotend-=1;
			sprintf(msg, "Kp_Hotend: %.1f", Kp_THotend);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuSetKiHotend){
			if(button == BUTTON_START)Ki_THotend-=0.1;
			if(button == BUTTON_STOP) Ki_THotend-=1;
			sprintf(msg, "Ki_Hotend: %.1f", Ki_THotend);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuSetKdHotend){
			if(button == BUTTON_START)Kd_THotend-=0.001;
			if(button == BUTTON_STOP) Kd_THotend-=1;
			sprintf(msg, "Kd_Hotend: %.1f", Kd_THotend);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuXStep){
			if(button == BUTTON_START)BLU-=0.001;
			if(button == BUTTON_STOP) BLU-=1;
			sprintf(msg, "XStep: %.4f", BLU);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuYStep){
			if(button == BUTTON_START)BLU-=0.001;
			if(button == BUTTON_STOP) BLU-=1;
			sprintf(msg, "YStep: %.4f", BLU);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuZStep){
			if(button == BUTTON_START)BLU_Z-=0.001;
			if(button == BUTTON_STOP) BLU_Z-=1;
			sprintf(msg, "ZStep: %.4f", BLU_Z);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuEStep){
			if(button == BUTTON_START)BLU_E-=0.001;
			if(button == BUTTON_STOP) BLU_E-=0.05;
			sprintf(msg, "EStep: %.4f", BLU_E);
			lcd_write(msg, 0,1);
			strcpy(currentMenu->name, msg);
		}
		else if(currentMenu == &menuMoveX){
			if(button == BUTTON_START)X-=0.001;
			if(button == BUTTON_STOP) X-=1;
			setpoint1 = round(X/BLU);
			sprintf(msg, "X: %.4f", X);
			lcd_write(msg, 0,1);
		}
		else if(currentMenu == &menuMoveY){
			if(button == BUTTON_START)Y-=0.001;
			if(button == BUTTON_STOP) Y-=1;
			setpoint2 = round(Y/BLU);
			sprintf(msg, "Y: %.4f", Y);
			lcd_write(msg, 0,1);
		}
		else if(currentMenu == &menuMoveZ){
			if(button == BUTTON_START)Z-=0.001;
			if(button == BUTTON_STOP) Z-=1;
			setpoint3 = round(Z/BLU_Z);
			sprintf(msg, "Z: %.4f", Z);
			lcd_write(msg, 0,1);
		}
		else if(currentMenu == &menuMoveE){
			if(button == BUTTON_START)E-=0.1;
			if(button == BUTTON_STOP) E-=1;
			setpoint4 = round(E/BLU);
			sprintf(msg, "E: %.4f", E);
			lcd_write(msg, 0,1);
		}
		else if(currentMenu == &menuSetTempHotTend){
			if(button == BUTTON_START)setTempHotend-=0.1;
			if(button == BUTTON_STOP) setTempHotend-=1;
			sprintf(msg, "T_Hotend: %.4f", setTempHotend);
			lcd_write(msg, 0,1);
		}
		//
		else{
			currentMenu->choose-=1;
			currentMenu->start-=1;
			if(currentMenu->choose < 1 && currentMenu->start < 1){
				currentMenu->choose = 1;
				currentMenu->start = 1;
				}
			menuDisplay(currentMenu);
		}
		//
		last_encoder_position--;
		}
	//
		
}

void Button_Set_Callback(void){
	if(HAL_GPIO_ReadPin(GPIO_SET_BUTTON_PORT, GPIO_SET_BUTTON_PIN) == 0){
		button = BUTTON_START;
		if((currentMenu->itemStack[currentMenu->choose] != NULL) && (start != 0)){
			currentMenu = currentMenu->itemStack[currentMenu->choose];
			menuDisplay(currentMenu);
				if(currentMenu == &menuSetKpHotend){
					sprintf(msg, "Kp_Hotend: %.1f", Kp_THotend);
					lcd_write(msg, 0,1);
				}
				else if(currentMenu == &menuSetKiHotend){
					sprintf(msg, "Ki_Hotend: %.1f", Ki_THotend);
					lcd_write(msg, 0,1);
				}
				else if(currentMenu == &menuSetKdHotend){
					sprintf(msg, "Kd_Hotend: %.1f", Kd_THotend);
					lcd_write(msg, 0,1);
				}
				else if(currentMenu == &menuXStep){
					sprintf(msg, "XStep: %.4f", BLU);
					lcd_write(msg, 0,1);
				}
				else if(currentMenu == &menuYStep){
					sprintf(msg, "YStep: %.4f", BLU);
					lcd_write(msg, 0,1);
				}
				else if(currentMenu == &menuZStep){
					sprintf(msg, "ZStep: %.4f", BLU_Z);
					lcd_write(msg, 0,1);
				}
				else if(currentMenu == &menuEStep){
					sprintf(msg, "EStep: %.4f", BLU_E);
					lcd_write(msg, 0,1);
				}
				else if(currentMenu == &menuMoveX){
					sprintf(msg, "X: %.4f", X);
					lcd_write(msg, 0,1);
				}
				else if(currentMenu == &menuMoveY){
					sprintf(msg, "Y: %.4f", Y);
					lcd_write(msg, 0,1);
				}
				else if(currentMenu == &menuMoveZ){
					sprintf(msg, "Z: %.4f", Z);
					lcd_write(msg, 0,1);
				}
				else if(currentMenu == &menuMoveE){
					sprintf(msg, "E: %.4f", E);
					lcd_write(msg, 0,1);
				}
				else if(currentMenu == &menuSetTempHotTend){
					sprintf(msg, "T_Hotend: %.4f", setTempHotend);
					lcd_write(msg, 0,1);
				}
		  }
		if((currentMenu->function != NULL) &(start != 0)){
			currentMenu->function();
		  }
		if(start == 0){
			currentMenu = &mainMenu;
      currentMenu->start = 1;
	    currentMenu->choose = 1;
	    menuDisplay(currentMenu);
			start = 0xFF;
		  }
		}
	if(HAL_GPIO_ReadPin(GPIO_SET_BUTTON_PORT, GPIO_SET_BUTTON_PIN)  != 0){
		button = BUTTON_STOP;
		
		}
}
void Button_Reset_CallBack(void){
	if(currentMenu->pre != NULL){
	 currentMenu = currentMenu->pre; 
 }
 menuDisplay(currentMenu);
}
/********USER FUNCTION*********/
void DisplayMainMenu(void){
	lcd_create_char(0,Hotend1, 0,0);
	int tmp = TempHotend;
	sprintf(msg, "%d", tmp);
	lcd_write(msg, 0, 1);
//	
	lcd_create_char(1,DO, 0,4);
  lcd_write("C", 0, 5);
	lcd_create_char(2, HedBed, 0,7);
  tmp = TempHedbed;
	sprintf(msg, "%d", tmp);
	lcd_write(msg, 0, 8);
	lcd_create_char(1,DO, 0,11);
	lcd_write("C", 0, 12);
	//
	
	}
void UpdateMainMenuParameter(void){
	if(currentMenu == &displayMenu){
		lcd_create_char(0,Hotend1, 0,0);
		int tmp = TempHotend;
		sprintf(msg, "%d", tmp);
		lcd_write(msg, 0, 1);
	//	
		lcd_create_char(1,DO, 0,4);
		lcd_write("C", 0, 5);
		lcd_create_char(2, HedBed, 0,7);
		tmp = TempHedbed;
		sprintf(msg, "%d", tmp);
		lcd_write(msg, 0, 8);
		lcd_create_char(1,DO, 0,11);
		lcd_write("C", 0, 12);
	}
}