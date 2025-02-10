#ifndef _MENU_H_
#define _MENU_H_
#include <stdint.h>
#include "main.h"
//
#define GPIO_CLK_PORT GPIOC
#define GPIO_CLK_PIN GPIO_PIN_0
#define GPIO_DT_PORT GPIOC
#define GPIO_DT_PIN GPIO_PIN_1
#define GPIO_SET_BUTTON_PORT GPIOC
#define GPIO_SET_BUTTON_PIN GPIO_PIN_2
#define GPIO_BUTTON_RETURN_PORT GPIOC
#define GPIO_BUTTON_RETURN_PIN GPIO_PIN_3
//
#define STACK_SIZE 8
#define KEY_DISABLE 0x00
#define KEY_ENABLE  0xFF
#define INPUT_ENABLE 0xFF
#define INPUT_DISABLE 0x00
#define KEY_ENTER_POS 10
#define MANUAL_FORM_ENABLE 0xFF
#define MANUAL_FORM_DISABLE 0x00
#define BUTTON_START 0xFF
#define BUTTON_STOP 0x00

typedef struct MenuItem{
    char name[20];                    // Tên c?a m?c menu
    void (*function)();               // Con tr? t?i hàm th?c hi?n ch?c nang
    struct MenuItem* itemStack[STACK_SIZE];         // Con tr? t?i m?c ti?p theo
    struct MenuItem* pre;         // Con tr? t?i submenu (n?u có)
	  int choose;
	  int stackNum;
	  uint8_t index;
	  uint8_t offset;
	  uint8_t start;
} MenuItem;
typedef struct {
	uint8_t row;
	uint8_t colum;
	uint8_t status;
	uint8_t row_pre;
	uint8_t colum_pre;
	uint8_t index;
	uint8_t inputStatus;
	char tmp[256];
	}KeyBoard_TypeDef;
typedef struct{
	uint16_t x_arc;
	uint16_t y_arc;
	}Component_TypeDef;
typedef struct{
	Component_TypeDef component1;
	Component_TypeDef component2;
	Component_TypeDef component3;
	Component_TypeDef component4;
	Component_TypeDef component5;
	}form1_TypeDef;
typedef struct{
	uint8_t status;
}Manual_Form_TypeDef;
/**********************/
MenuItem* createMenuItem(MenuItem *newItem, const char* name, void (*function)());
void addMenuItem( MenuItem *parentMenu, MenuItem *childMenu, uint8_t stackIndex);
void previousMenuReturn( MenuItem *curMenu);
void Encoder_Rotary_Init(void);
void Encoder_Read(void);
void Button_Set_Callback(void);
void Button_Reset_CallBack(void);
void menuSetUp(void);
int menuDisplay( MenuItem * menu);
void menuSetFunction(void);
void menuControl(void);
void menuSetFunction(void);
void DisplayMainMenu(void);
void UpdateMainMenuParameter(void);
extern volatile int32_t encoder_position; // Giá tr? v? trí encoder
extern volatile int32_t last_encoder_position;
extern uint8_t last_state_A ;       
extern uint8_t button;
extern int cnt;
extern char msg[48];
extern float Kp;
extern float Ki;
extern float Kd;
extern float setPos;
extern float curPos ;
extern float setVel ;
extern float curVel ;
extern float dt ;
extern float Buoc_vit ;
extern uint8_t start;
extern MenuItem *currentMenu;
extern MenuItem *currentMenu;
extern MenuItem displayMenu;
extern MenuItem mainMenu;
extern MenuItem menuConfig;
extern MenuItem menuPIDParameter;
extern MenuItem menuSetKpHotend;
extern MenuItem menuSetKiHotend;
extern MenuItem menuSetKdHotend;
extern MenuItem menuSetKpHedbed;
extern MenuItem menuSetKiHedbed;
extern MenuItem menuSetKdHedbed;
extern MenuItem menuMotorParameter;
extern MenuItem menuXStep;
extern MenuItem menuYStep;
extern MenuItem menuZStep;
extern MenuItem menuEStep;
extern MenuItem menuConTrol;
extern MenuItem menuMoveX;
extern MenuItem menuMoveY;
extern MenuItem menuMoveZ;
extern MenuItem menuMoveE;
extern MenuItem menuSetHome;
extern MenuItem menuAutoHome;
extern MenuItem menuSetFanPower;
extern MenuItem menuSetTempHotTend;
extern MenuItem menuSetTempHedBed;
extern MenuItem menuSave;
#endif