#include "noisuy.h"
#include "stdint.h"
#include "main.h"
#include <math.h>
#include <stdlib.h>

///**************Global Variable***************/
int64_t setpoint1 = 0;
int64_t current_step1 = 0;
int64_t setpoint2= 0;
int64_t current_step2 = 0;
int64_t current_step3 = 0;
int64_t setpoint3 = 0;
int64_t setpoint4 = 0;
int64_t current_step4 = 0;
uint32_t numOfPoint = 0;
volatile float X, Y, Z, F, I, J, E, BLU_E = 0.0025;
volatile uint8_t  G_FLAG;
float BLU = 0.0125;
float BLU_Z = 0.000625;
//Interpolation_TypDef Interpolation_handle;
//PulseChange pulse_table[] = {
//    [MODE_CW1] = {+1, -1},
//    [MODE_CW2] = {+1, +1},
//    [MODE_CW3] = {-1, +1},
//    [MODE_CW4] = {-1, -1},
//    [MODE_CCW1] = {-1, +1},
//    [MODE_CCW2] = {-1, -1},
//    [MODE_CCW3] = {+1, -1},
//    [MODE_CCW4] = {+1, +1},
//};
//StopPoint stopPointTable [] = {
//	[CW_STOP_IN_Q1] = {0,0},
//	[CW_STOP_IN_Q2] = {0,0},
//	[CW_STOP_IN_Q3] = {0,0},
//	[CW_STOP_IN_Q4] = {0,0},
//	[CCW_STOP_IN_Q1] = {0,0},
//	[CCW_STOP_IN_Q2] = {0,0},
//	[CCW_STOP_IN_Q3] = {0,0},
//	[CCW_STOP_IN_Q4] = {0,0},
//	};
//void Init(void){
//	Interpolation_handle.BLU_X = X_STEP;
//	Interpolation_handle.BLU_Y = Y_STEP;
//	Interpolation_handle.BLU_Z = Z_STEP;
//	Interpolation_handle.BLU_E = E_STEP;
//	Interpolation_handle.step1 = 0;
//	Interpolation_handle.step2 = 0;
//	Interpolation_handle.step3 = 0;
//	Interpolation_handle.curStep1 = 0;
//	Interpolation_handle.curStep2 = 0;
//	Interpolation_handle.curStep3 = 0;
//	Interpolation_handle.curStep4 = 0;
//}
//void DDA_Start( Interpolation_TypDef *handle, float x0, float y0, float x1,float  y1){
//		handle->DDA_Handle.dx = x1 - x0;
//		handle->DDA_Handle.dy = y1 - y0;
//		handle->L = sqrt(handle->DDA_Handle.dx*handle->DDA_Handle.dx + handle->DDA_Handle.dy*handle->DDA_Handle.dy);
//		handle->totalNumPoint = (int64_t)(handle->L/handle->BLU_X + 1);
//		//
//		handle->totalTime = (handle->L/handle->F)*1000000;//us
//		//
//		handle->timPerStep = (uint16_t)(handle->totalTime/ handle->totalNumPoint);
//		//
//		handle->DDA_Handle.x_inc = handle->DDA_Handle.dx/handle->totalNumPoint;
//		handle->DDA_Handle.y_inc = handle->DDA_Handle.dy/handle->totalNumPoint;
//		//
//		handle->curX = x0;
//		handle->curY = y0;
//		//
//		handle->DDA_Handle.cnt = 0;	
//	}
//void DS_START( Interpolation_TypDef *handle, float x_b, float y_b, float x_f, float y_f, float I, float J, float F){
//	
//	
//	 handle->DS_Handle.I = I;
//	 handle->DS_Handle.J = J;
//	 handle->F = F;
//	 handle->x_b = round(x_b/handle->BLU_X);
//	 handle->y_b = round(y_b/handle->BLU_Y);
//	 handle->setX = round(x_f/handle->BLU_X);
//	 handle->setY = round(y_f/handle->BLU_Y);
//	 handle->DS_Handle.x_c = round((x_b + I)/handle->BLU_X);
//	 handle->DS_Handle.y_c = round((y_b + J)/handle->BLU_Y);
//	 handle->DS_Handle.R = sqrt(I*I + J*J);
//	handle->step1 = handle->x_b;
//	handle->step2 = handle->y_b;
//	 startPoint(handle);
//	 stopPoint(handle);
//	    if(handle->tmpMode == MODE_CW1){
//			  handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = 2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = -2*handle->y_b + 1;
//		  }else if(handle->tmpMode == MODE_CW2){
//			  handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = 2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = 2*handle->y_b + 1;
//		  }else if(handle->tmpMode == MODE_CW3){
//			  handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = -2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = 2*handle->y_b + 1;
//		  } else if(handle->tmpMode == MODE_CW4){
//			  handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = -2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = -2*handle->y_b + 1;
//		  }else if(handle->tmpMode == MODE_CCW1){
//			  handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = -2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = 2*handle->y_b + 1;
//		  }else if(handle->tmpMode == MODE_CCW2){
//			  handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = -2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = -2*handle->y_b + 1;
//		  }else if(handle->tmpMode == MODE_CCW3){
//			  handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = 2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = -2*handle->y_b + 1;
//		  }else if(handle->tmpMode == MODE_CCW4){
//			  handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = 2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = 2*handle->y_b + 1;
//		  }
//			
//			
//     DS_NumOfPointInterpolationCount(handle); 			
//	}
//void DS_Control( Interpolation_TypDef *handle){
//	uint8_t pulseX = NO_PULSE_X;
//	uint8_t pulseY = NO_PULSE_Y;
//	if(handle->mode == MODE_CW || handle->mode == MODE_CCW)
//		handle->DS_Handle.D1 = handle->DS_Handle.D + handle->DS_Handle.DeltaX;
//		handle->DS_Handle.D2 = handle->DS_Handle.D + handle->DS_Handle.DeltaY;
//		handle->DS_Handle.D3 = handle->DS_Handle.D +  handle->DS_Handle.DeltaX + handle->DS_Handle.DeltaY;
//		if ((abs(handle->DS_Handle.D1) < abs(handle->DS_Handle.D2)) && (abs(handle->DS_Handle.D1) < abs(handle->DS_Handle.D3)))
//		{    
//				handle->DS_Handle.D = handle->DS_Handle.D + handle->DS_Handle.DeltaX;
//				handle->DS_Handle.DeltaX = handle->DS_Handle.DeltaX + 2;
//				handle->pulse_X = PULSE_X;
//		}else if (abs(handle->DS_Handle.D2) < abs(handle->DS_Handle.D1) && abs(handle->DS_Handle.D2) < abs(handle->DS_Handle.D3))
//		{
//				handle->DS_Handle.D = handle->DS_Handle.D + handle->DS_Handle.DeltaY;
//				handle->DS_Handle.DeltaY = handle->DS_Handle.DeltaY + 2;
//				handle->pulse_Y = PULSE_Y;
//		}else
//		{    
//				handle->DS_Handle.D = handle->DS_Handle.D + handle->DS_Handle.DeltaX + handle->DS_Handle.DeltaY;
//				handle->DS_Handle.DeltaX = handle->DS_Handle.DeltaX + 2;
//				handle->DS_Handle.DeltaY = handle->DS_Handle.DeltaY + 2;
//				handle->pulse_X= PULSE_X;
//			  handle->pulse_Y = PULSE_Y;
//		}
//		pulse(handle, handle->pulse_X, handle->pulse_Y);
//	  tmpModeCheck(handle);
//		stopPointCheck(handle);
//}

//uint8_t tmpModeCheck(Interpolation_TypDef *handle) {
//    
//    if(handle->step1 != handle->DS_Handle.x_c && handle->step2 != handle->DS_Handle.y_c && handle->tmpModeCnt  != 0){//Most likely possible condition
//			//
//			//
//			}
//		else if(handle->step1 == handle->DS_Handle.x_c && handle->step2 > handle->DS_Handle.y_c){//less_likely_condition
//			if(handle->mode == MODE_CW){
//				handle->tmpMode = MODE_CW1;
//				handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = 2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = -2*handle->x_b + 1;
//				}
//			else if(handle->mode == MODE_CCW){
//			  handle->tmpMode = MODE_CCW2;
//				handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = -2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = -2*handle->x_b + 1;
//				}
//			}
//		else if(handle->step1 == handle->DS_Handle.x_c && handle->step2 < handle->DS_Handle.y_c){//less_likely_condition
//			if(handle->mode == MODE_CW){
//				handle->tmpMode = MODE_CW3;
//				handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = -2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = 2*handle->x_b + 1;
//				}
//			else if(handle->mode == MODE_CCW){
//				handle->tmpMode = MODE_CCW4;
//				handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = 2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = 2*handle->x_b + 1;
//				}
//			}
//		else if(handle->step2 == handle->DS_Handle.y_c && handle->step1 > handle->DS_Handle.x_c){//less_likely_condition
//      if(handle->mode == MODE_CW){
//				handle->tmpMode = MODE_CW4;
//				handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = -2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = -2*handle->x_b + 1;
//				}
//			else if(handle->mode == MODE_CCW){
//				handle->tmpMode = MODE_CCW1;
//				handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = -2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = 2*handle->x_b + 1;
//				}
//		}
//		else if(handle->step2 == handle->DS_Handle.y_c && handle->step1 < handle->DS_Handle.x_c){//less_likely_condition
//      if(handle->mode == MODE_CW){
//				handle->tmpMode = MODE_CW2;
//				handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = 2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = 2*handle->x_b + 1;
//				}
//			else if(handle->mode == MODE_CCW){
//				handle->tmpMode = MODE_CCW3;
//				handle->DS_Handle.D = 0;
//				handle->DS_Handle.DeltaX = 2*handle->x_b + 1;
//				handle->DS_Handle.DeltaY = -2*handle->x_b + 1;
//				}
//		}
//     return 0;
//}
//void pulse(Interpolation_TypDef *handle, uint8_t signalX, uint8_t signalY) {
//    if (signalX == PULSE_X) {
//        handle->step1 += pulse_table[handle->tmpMode].step1_change;
//    }
//    if (signalY == PULSE_Y) {
//        handle->step2 += pulse_table[handle->tmpMode].step2_change;
//    }
//    handle->tmpModeCnt++;
//}
////
//uint8_t startPoint(Interpolation_TypDef *handle){
//	uint8_t tmp;
//	 if(handle->mode == MODE_CW){
//		 if ((handle->x_b >= handle->DS_Handle.x_c) && (handle->y_b  > handle->DS_Handle.y_c)){//1
//			 tmp = CW_START_IN_Q1;
//			 handle->tmpMode = MODE_CW1;
//		 }
//		 else if ((handle->x_b < handle->DS_Handle.x_c) && (handle->y_b >= handle->DS_Handle.y_c)){//2
//		   tmp = CW_START_IN_Q2;
//			 handle->tmpMode = MODE_CW2;
//		 }
//		 else if ((handle->x_b <= handle->DS_Handle.x_c) && (handle->y_b < handle->DS_Handle.y_c)){//3
//			 tmp = CW_START_IN_Q3;
//			 handle->tmpMode = MODE_CW3;
//		 }
//		 else if ((handle->x_b > handle->DS_Handle.x_c) && (handle->y_b <= handle->DS_Handle.y_c)){//4
//			 tmp = CW_START_IN_Q4;
//			 handle->tmpMode = MODE_CW4;
//		 }
//	 }
//	 else if(handle->mode == MODE_CCW){
//		 if ((handle->x_b > handle->DS_Handle.x_c) && (handle->y_b >= handle->DS_Handle.y_c)){//1
//			 tmp = CCW_START_IN_Q1;
//			 handle->tmpMode = MODE_CCW1;
//		 }
//		 else if ((handle->x_b <= handle->DS_Handle.x_c) && (handle->y_b > handle->DS_Handle.y_c)){//2
//			 tmp = CCW_START_IN_Q2;
//			 handle->tmpMode = MODE_CCW2;
//		 }
//		 else if ((handle->x_b < handle->DS_Handle.x_c) && (handle->y_b <= handle->DS_Handle.y_c)){//3
//			 tmp = CCW_START_IN_Q3;
//			 handle->tmpMode = MODE_CCW3;
//		 }
//		 else if ((handle->x_b >= handle->DS_Handle.x_c) && (handle->y_b < handle->DS_Handle.y_c)){//4
//			 tmp = CCW_START_IN_Q4;
//			 handle->tmpMode = MODE_CCW4;
//		 }
//	 }
//	 handle->DS_Handle.startPoint = tmp;
//	return tmp;
//}
//uint8_t stopPoint( Interpolation_TypDef *handle){
//	uint8_t tmp = 0;
//	if(handle->mode == MODE_CW){
//	   if ((handle->setX > handle->DS_Handle.x_c) && (handle->setY  >= handle->DS_Handle.y_c)) //K?t thúc ? góc ph?n tu th? 1
//     {           
//        tmp = CW_STOP_IN_Q1;
//		 }
//		 else if ((handle->setX <= handle->DS_Handle.x_c) && (handle->setY > handle->DS_Handle.y_c)) //K?t thúc ? góc ph?n tu th? 2
//		 {   tmp = CW_STOP_IN_Q2;
//				
//		 } 
//		 else if ((handle->setX < handle->DS_Handle.x_c) && (handle->setY <= handle->DS_Handle.y_c)) //K?t thúc ? góc ph?n tu th? 3
//		 {  
//				tmp = CW_STOP_IN_Q3;
//		 }
//		 else if ((handle->setX >= handle->DS_Handle.x_c) && (handle->setY < handle->DS_Handle.y_c)) //K?t thúc ? góc ph?n tu th? 4
//		 {
//				tmp = CW_STOP_IN_Q4;
//		 }
//		}
//  else if(handle->mode == MODE_CCW){
//     if ((handle->setX >= handle->DS_Handle.x_c) && (handle->setY > handle->DS_Handle.y_c)) //K?t thúc ? góc ph?n tu th? 1
//         tmp = CCW_STOP_IN_Q1;       
//            
//		 if ((handle->setX < handle->DS_Handle.x_c) && (handle->setY >= handle->DS_Handle.y_c)) //K?t thúc ? góc ph?n tu th? 2
//		 {   
//				tmp = CCW_STOP_IN_Q2; 
//		 } 
//		 if ((handle->setX <= handle->DS_Handle.x_c) && (handle->setY < handle->DS_Handle.y_c)) //K?t thúc ? góc ph?n tu th? 3
//		 {  
//				tmp = CCW_STOP_IN_Q3; 
//		 }
//		 if ((handle->setX > handle->DS_Handle.x_c) && (handle->setY <= handle->DS_Handle.y_c)) //K?t thúc ? góc ph?n tu th? 4
//		 {
//				tmp = CCW_STOP_IN_Q4; 
//		 }
//  }
//  handle->DS_Handle.stopPoint = tmp;	
//	return tmp;
//	}
//bool stopPointCheck(Interpolation_TypDef *handle){
//	 if(handle->DS_Handle.stopPoint != handle->tmpMode){
//		 return false;
//		 }
//	 else if(handle->DS_Handle.stopPoint == handle->tmpMode){
//		 if(handle->DS_Handle.stopPoint == CW_STOP_IN_Q1 && handle->step2 < handle->setY){
//			 handle->mode = MODE_NO;
//			 return true;
//			 }
//		 else if(handle->DS_Handle.stopPoint == CCW_STOP_IN_Q1 && handle->step1 < handle->setX){
//			 handle->mode = MODE_NO;
//			 return true;
//			 }
//		 else if(handle->DS_Handle.stopPoint == CW_STOP_IN_Q2 && handle->step1 > handle->setX){
//			 handle->mode = MODE_NO;
//			 return true;
//			 }
//		 else if(handle->DS_Handle.stopPoint == CCW_STOP_IN_Q2 && handle->step2 < handle->setY){
//			 handle->mode = MODE_NO;
//			 return true;
//			 }
//		 else if(handle->DS_Handle.stopPoint == CW_STOP_IN_Q3 && handle->step2 > handle->setY){
//			 handle->mode = MODE_NO;
//			 return true;
//			 }
//		 else if(handle->DS_Handle.stopPoint == CCW_STOP_IN_Q3 && handle->step1 > handle->setX){
//			 handle->mode = MODE_NO;
//			 return true;
//			 }
//		 else if(handle->DS_Handle.stopPoint == CW_STOP_IN_Q4 && handle->step1 < handle->setX){
//			 handle->mode = MODE_NO;
//			 return true;
//			 }
//		 else if(handle->DS_Handle.stopPoint == CCW_STOP_IN_Q4 && handle->step2 > handle->setY){
//			 handle->mode = MODE_NO;
//			 return true;
//			 }
//		 }
//	 else return false;
//	}
//void DS_NumOfPointInterpolationCount(Interpolation_TypDef *handle){
//	handle->state = STATE_COUNT;
//	handle->totalNumPoint = 0;
//	int64_t tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12;
//	tmp1 = handle->x_b;
//	tmp2 = handle->y_b;
//	tmp3 = handle->setX;
//	tmp4 = handle->setY;
//	tmp5 = handle->DS_Handle.D;
//	tmp6 = handle->DS_Handle.D1;
//	tmp7 = handle->DS_Handle.D2;
//	tmp8 = handle->DS_Handle.D3;
//	tmp9 = handle->DS_Handle.DeltaX;
//	tmp10 = handle->DS_Handle.DeltaY;
//	tmp11 = handle->step1;
//	tmp12 = handle->step2;
//	//
//	while(stopPointCheck(handle) != true){
//		uint8_t pulseX = NO_PULSE_X;
//		uint8_t pulseY = NO_PULSE_Y;
//		if(handle->mode == MODE_CW || handle->mode == MODE_CCW){
//			handle->DS_Handle.D1 = handle->DS_Handle.D + handle->DS_Handle.DeltaX;
//			handle->DS_Handle.D2 = handle->DS_Handle.D + handle->DS_Handle.DeltaY;
//			handle->DS_Handle.D3 = handle->DS_Handle.D +  handle->DS_Handle.DeltaX + handle->DS_Handle.DeltaY;
//			if ((abs(handle->DS_Handle.D1) < abs(handle->DS_Handle.D2)) && (abs(handle->DS_Handle.D1) < abs(handle->DS_Handle.D3)))
//			{    
//					handle->DS_Handle.D = handle->DS_Handle.D + handle->DS_Handle.DeltaX;
//					handle->DS_Handle.DeltaX = handle->DS_Handle.DeltaX + 2;
//					handle->pulse_X = PULSE_X;
//				 handle->pulse_Y = 0;
//			}if (abs(handle->DS_Handle.D2) < abs(handle->DS_Handle.D1) && abs(handle->DS_Handle.D2) < abs(handle->DS_Handle.D3))
//			{
//					handle->DS_Handle.D = handle->DS_Handle.D + handle->DS_Handle.DeltaY;
//					handle->DS_Handle.DeltaY = handle->DS_Handle.DeltaY + 2;
//					handle->pulse_Y = PULSE_Y;
//				  handle->pulse_X = 0;
//			}else
//			{    
//					handle->DS_Handle.D = handle->DS_Handle.D + handle->DS_Handle.DeltaX + handle->DS_Handle.DeltaY;
//					handle->DS_Handle.DeltaX = handle->DS_Handle.DeltaX + 2;
//					handle->DS_Handle.DeltaY = handle->DS_Handle.DeltaY + 2;
//					handle->pulse_X= PULSE_X;
//					handle->pulse_Y = PULSE_Y;
//			}
//			pulse(handle, handle->pulse_X, handle->pulse_Y);
//			tmpModeCheck(handle);
//			handle->totalNumPoint+=1;
//			}
//    }
//	//
//	handle->x_b = tmp1;
//	handle->y_b = tmp2;
//	handle->setX = tmp3;
//	handle->setY = tmp4;
//	handle->DS_Handle.D = tmp5;
//	handle->DS_Handle.D1 = tmp6;
//	handle->DS_Handle.D2 = tmp7;
//  handle->DS_Handle.D3 = tmp8;
//	handle->DS_Handle.DeltaX = tmp9;
//	handle->DS_Handle.DeltaY = tmp10;
//	handle->step1 = tmp11;
//	handle->step2 = tmp12;
//	//
//	}
/*****************************/
void Delay_us(uint16_t us){
	TIM2->CNT = 0;
	while(TIM2->CNT < us);
		
}
void MOVE_Z( float z0, float z1,float bluz, float F){
	 float dz, z, z_inc;
    int64_t steps, timePerStep, totalTime;

    // Tính d? thay d?i
    dz = (z1 - z0);
    

    // Chi?u dài du?ng di
    float l = sqrt(dz * dz );

    // Xác d?nh s? bu?c d?a trên chi?u dài và giá tr? blu
    steps = (int64_t)(l / bluz) + 1;  // Luôn ít nh?t 1 bu?c

    // Tính t?c d?
    totalTime = (l / F) * 1000000;
    timePerStep = (uint16_t)(totalTime / steps);

    // Tính bu?c tang
    z_inc = (dz / steps);
    for (uint64_t i = 0; i <= steps; i++)
    {
        setpoint3 = round(z / bluz);   
        Delay_us(timePerStep);
        z += z_inc; 
    }
}
void MOVE_Extruder(float blue, float E, float F){
	 float de, e, e_inc;
    int64_t steps, timePerStep, totalTime;
	
    // Xác d?nh s? bu?c d?a trên chi?u dài và giá tr? blu
    steps = (int64_t)(E / blue) + 1;  // Luôn ít nh?t 1 bu?c

    // Tính t?c d?
    totalTime = (E / F) * 1000000;
    timePerStep = (uint16_t)(totalTime / steps);

    // Tính bu?c tang
    e_inc = (E / steps);
	  e = 0;
    for (uint64_t i = 0; i <= steps; i++)
    {
        setpoint3 = round(e / blue);   
        Delay_us(timePerStep);
        e += e_inc; 
    }
}
void DDA_LINE(float x_0, float y_0, float x_1, float y_1, float blu, float F, float E, float blu_e)
{
    float dx, dy, x, y, x_inc, y_inc, e, e_inc;
    int64_t steps, timePerStep, totalTime;

    // Tính d? thay d?i
    dx = (x_1 - x_0);
    dy = (y_1 - y_0);

    // Chi?u dài du?ng di
    float l = sqrt(dx * dx + dy * dy);

    // Xác d?nh s? bu?c d?a trên chi?u dài và giá tr? blu
    steps = (int64_t)(l / blu) + 1;  // Luôn ít nh?t 1 bu?c

    // Tính t?c d?
    totalTime = (l / F) * 1000000;
    timePerStep = (uint16_t)(totalTime / steps);

    // Tính bu?c tang
    x_inc = (dx / steps);
    y_inc = (dy / steps);
    e_inc = E/(float)steps;
    // Ði?m d?u
    x = x_0;
    y = y_0;
    e = current_step4*blu_e;
    for (uint64_t i = 0; i <= steps; i++)
    {
        setpoint1 = round(x / blu);
        setpoint2 = round(y / blu);
			  setpoint4 = round(e/blu_e);
        Delay_us(timePerStep);
        e +=e_inc;
        x += x_inc;
        y += y_inc;
    }
}


void drawCircleQuadrant1CW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t state)
{
	uint32_t cnt = 0;
	float I,J, L, r, pi = 3.14, totalTime, timePerStep, e = current_step4*blu_e;
	//Tinh do dai cung
	float theta0, theta1, delta_theta;
	if(x_b == x_c) theta0 = pi/2;
	if(x_b != x_c) theta0 = atan2((y_b - y_c),(x_b - x_c));
	theta1 = atan2((y_f - y_c),(x_f - x_c));
	delta_theta = fabs(theta1 - theta0);
	r = sqrt((x_b - x_c)*(x_b - x_c) + (y_b - y_c)*(y_b - y_c));
	L = (delta_theta)*r;
	totalTime = (L/F)*1000000;
	int64_t x_0,y_0,x_1,y_1, delta_X, delta_Y, D, D1, D2, D3, x, y, x_cnt, y_cnt;
	 I = x_b - x_c;
	J = y_b - y_c;
	x_0 = round(I / blu);
	y_0 = round(J / blu);
	x_1 = round(x_f/blu);
	y_1 = round(y_f/blu);
	delta_X = 2 * x_0 + 1;
	delta_Y = -2 * y_0 + 1;

	D = 0;
	x_cnt = round(x_b / blu);
	y_cnt = round(y_b / blu);
               
                /**/
	              while (y_cnt > y_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x_cnt = x_cnt + 1;
										}
                    else if ((abs(D2) < abs(D1)) && (abs(D2) < abs(D3)))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y_cnt = y_cnt - 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x_cnt = x_cnt + 1;
                        y_cnt = y_cnt - 1;
										}
										cnt++;
									}
								numOfPoint = cnt;
                /**/
								timePerStep = totalTime/cnt;
						if(state == STATE_INTER){
								delta_X = 2 * x_0 + 1;
                delta_Y = -2 * y_0 + 1;
                
                D = 0;
                x = round(x_b / blu);
                y = round(y_b / blu);
                while (y > y_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x = x + 1;
										}
                    else if ((abs(D2) < abs(D1)) && (abs(D2) < abs(D3)))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y = y - 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x = x + 1;
                        y = y - 1;
										}
										e+= e_inc;
										setpoint1 = x;
										setpoint2 = y;
										setpoint4 = round(e/blu_e);
										Delay_us(timePerStep);
									}
								}
}

void drawCircleQuadrant1CCW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t state)
{
	uint32_t cnt = 0;
	float I,J, r, L, pi  = 3.14,totalTime, timePerStep, e = current_step4*blu_e;
	//Tinh do dai cung
	float theta0, theta1, delta_theta;
	if(x_b == x_c) theta0 = pi/2;
	if(x_b != x_c) theta0 = atan2((y_b - y_c),(x_b - x_c));
	theta1 = atan2((y_f - y_c),(x_f - x_c));
	delta_theta = fabs(theta1 - theta0);
	r = sqrt((x_b - x_c)*(x_b - x_c) + (y_b - y_c)*(y_b - y_c));
	L = (delta_theta)*r;
	totalTime = (L/F)*1000000;
	int64_t x_0,y_0,x_1,y_1, delta_X, delta_Y, D, D1, D2, D3,x,y, x_cnt, y_cnt;
	 I = x_b - x_c;
                J = y_b - y_c;
                x_0 = round(I / blu);
                y_0 = round(J / blu);
                x_1 = round(x_f/blu);
                y_1 = round(y_f/blu);
                delta_X = -2 * x_0 + 1;
                delta_Y = 2 * y_0 + 1;
                
                D = 0;
                x_cnt = round(x_b / blu);
                y_cnt = round(y_b / blu);
                
             
                /**/
							 while (x_cnt > x_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x_cnt = x_cnt - 1;
										}
                    else if (abs(D2) < abs(D1) && abs(D2) < abs(D3))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y_cnt = y_cnt + 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x_cnt = x_cnt - 1;
                        y_cnt = y_cnt + 1;
										}
										cnt++;
										
									}
								/**/
									numOfPoint = cnt;
								timePerStep = totalTime/cnt;
				if(state == STATE_INTER){
								delta_X = -2 * x_0 + 1;
                delta_Y = 2 * y_0 + 1;
                
                D = 0;
                x = round(x_b / blu);
                y = round(y_b / blu);
								/**/
                while (x > x_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x = x - 1;
										}
                    else if (abs(D2) < abs(D1) && abs(D2) < abs(D3))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y = y + 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x = x - 1;
                        y = y + 1;
										}
										e+=e_inc;
										setpoint1 = x;
										setpoint2 = y;
										setpoint4 = round(e/blu_e);
										Delay_us(timePerStep);
									}
					}		
							
 }

void drawCircleQuadrant2CW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t  state)
{
	uint32_t cnt = 0;
	float I,J, r, L, pi  = 3.14,totalTime, timePerStep, e = current_step4*blu_e;
	//Tinh do dai cung
	float theta0, theta1, delta_theta;
	
	theta0 = atan2((y_b - y_c),(x_b - x_c));
	if(x_f == x_c) theta1 = pi/2;
	if(x_f != x_c) theta1 = atan2((y_f - y_c),(x_f - x_c));
	delta_theta = fabs(theta1 - theta0);
	r = sqrt((x_b - x_c)*(x_b - x_c) + (y_b - y_c)*(y_b - y_c));
	L = (delta_theta)*r;
	totalTime = (L/F)*1000000;
	int64_t x_0,y_0,x_1,y_1, delta_X, delta_Y, D, D1, D2, D3,x,y, x_cnt, y_cnt;
	 I = x_b - x_c;
                J = y_b - y_c;
                x_0 = round(I / blu);
                y_0 = round(J / blu);
                x_1 = round(x_f/blu);
                y_1 = round(y_f/blu);
                delta_X = 2 * x_0 + 1;
                delta_Y = 2 * y_0 + 1;
                
                D = 0;
	
                x_cnt = round(x_b / blu);
                y_cnt = round(y_b / blu);
                
								/**/
								 while (x_cnt < x_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x_cnt = x_cnt + 1;
										}
                    else if (abs(D2) < abs(D1) && abs(D2) < abs(D3))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y_cnt = y_cnt + 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x_cnt = x_cnt + 1;
                        y_cnt = y_cnt + 1;
										}
										cnt++;
									}
								
	              /**/
									numOfPoint = cnt;
								timePerStep = totalTime/cnt;
						if(state == STATE_INTER){
								delta_X = 2 * x_0 + 1;
                delta_Y = 2 * y_0 + 1;
                
                D = 0;
	             
                x = round(x_b / blu);
                y = round(y_b / blu);	
                /**/
                while (x < x_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x = x + 1;
										}
                    else if (abs(D2) < abs(D1) && abs(D2) < abs(D3))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y = y + 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x = x + 1;
                        y = y + 1;
										}
										e+= e_inc;
										setpoint1 = x;
										setpoint2 = y;
										setpoint4 = round(e/blu_e);
										Delay_us(timePerStep);
									}
								}
							
}

void drawCircleQuadrant2CCW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t state)
{
	 uint32_t cnt = 0;
	float I,J, r, L, pi  = 3.14,totalTime, timePerStep, e = current_step4*blu_e;
	//Tinh do dai cung
	//Tinh do dai cung
	float theta0, theta1, delta_theta;
	
	theta0 = atan2((y_b - y_c),(x_b - x_c));
	if(x_f == x_c) theta1 = pi/2;
	if(x_f != x_c) theta1 = atan2((y_f - y_c),(x_f - x_c));
	delta_theta = fabs(theta1 - theta0);
	r = sqrt((x_b - x_c)*(x_b - x_c) + (y_b - y_c)*(y_b - y_c));
	L = (delta_theta)*r;
	totalTime = (L/F)*1000000;
	int64_t x_0,y_0,x_1,y_1, delta_X, delta_Y, D, D1, D2, D3,x,y, x_cnt, y_cnt;
	 I = x_b - x_c;
                J = y_b - y_c;
                x_0 = round(I / blu);
                y_0 = round(J / blu);
                x_1 = round(x_f/blu);
                y_1 = round(y_f/blu);
                delta_X = -2 * x_0 + 1;
                delta_Y = -2 * y_0 + 1;
                
                D = 0;
                x_cnt = round(x_b / blu);
                y_cnt = round(y_b / blu);
                /**/
	              while (y_cnt > y_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x_cnt = x_cnt - 1;
										}
                    else if (abs(D2) < abs(D1) && abs(D2) < abs(D3))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y_cnt = y_cnt - 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x_cnt = x_cnt - 1;
                        y_cnt = y_cnt - 1;
										}
										cnt++;
								}
                /**/
								numOfPoint = cnt;
								timePerStep = (uint16_t)(totalTime/cnt);
					if(state == STATE_INTER){
								delta_X = -2 * x_0 + 1;
                delta_Y = -2 * y_0 + 1;
                
                D = 0;
                x = round(x_b / blu);
                y = round(y_b / blu);
                /**/
                while (y > y_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x = x - 1;
										}
                    else if (abs(D2) < abs(D1) && abs(D2) < abs(D3))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y = y - 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x = x - 1;
                        y = y - 1;
										}
										e+=e_inc;
										setpoint1 = x;
										setpoint2 = y;
										setpoint4 = round(e/blu_e);
										Delay_us(timePerStep);
								}
							}
}

void drawCircleQuadrant3CW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t state)
{
	uint32_t cnt = 0;
	float I,J, r, L, pi  = 3.14,totalTime, timePerStep, e = current_step4*blu_e;
	//Tinh do dai cung
	float theta0, theta1, delta_theta;
	
	if(x_f == x_c) theta1 = 0;
	if(x_f != x_c) theta1 = atan2(fabs(y_f - y_c),fabs(x_f - x_c));
	theta0 = atan2(fabs(y_b - y_c),fabs(x_b - x_c));
	delta_theta = fabs(theta1 - theta0);
	r = sqrt((x_b - x_c)*(x_b - x_c) + (y_b - y_c)*(y_b - y_c));
	L = (delta_theta)*r;
	totalTime = (L/F)*1000000;
	int64_t x_0,y_0,x_1,y_1, delta_X, delta_Y, D, D1, D2, D3,x,y, x_cnt, y_cnt;
	 I = x_b - x_c;
                J = y_b - y_c;
                x_0 = round(I / blu);
                y_0 = round(J / blu);
                x_1 = round(x_f/blu);
                y_1 = round(y_f/blu);
                delta_X = -2 * x_0 + 1;
                delta_Y = 2 * y_0 + 1;
                
                D = 0;
                x_cnt = round(x_b / blu);
                y_cnt = round(y_b / blu);
                
                /**/
								while (y_cnt < y_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x_cnt = x_cnt - 1;
										}
                    else if ((abs(D2) < abs(D1)) && (abs(D2) < abs(D3)))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y_cnt = y_cnt + 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x_cnt = x_cnt - 1;
                        y_cnt = y_cnt + 1;
										}
										cnt++;
									}
                /**/
									numOfPoint = cnt;
								timePerStep = totalTime/cnt;
							if(state == STATE_INTER){
								delta_X = -2 * x_0 + 1;
                delta_Y = 2 * y_0 + 1;
                
                D = 0;
                x = round(x_b / blu);
                y = round(y_b / blu);
								/**/
                while (y < y_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x = x - 1;
										}
                    else if ((abs(D2) < abs(D1)) && (abs(D2) < abs(D3)))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y = y + 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x = x - 1;
                        y = y + 1;
										}
										e+= e_inc;
										setpoint1 = x;
										setpoint2 = y;
										setpoint4 = round(e/blu_e);
										Delay_us(timePerStep);
									}
								}
}

void drawCircleQuadrant3CCW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t state)
{
	uint32_t cnt = 0;
	float I,J, r, L, pi  = 3.14,totalTime, timePerStep, e = current_step4*blu_e;
	//Tinh do dai cung
	float theta0, theta1, delta_theta;
	
	if(x_b == x_c) theta0 = 0;
	if(x_b != x_c) theta0 = atan2(fabs(y_b - y_c),fabs(x_b - x_c));
	theta1 = atan2(fabs(y_f - y_c),fabs(x_f - x_c));
	delta_theta = fabs(theta1 - theta0);
	r = sqrt((x_b - x_c)*(x_b - x_c) + (y_b - y_c)*(y_b - y_c));
	L = (delta_theta)*r;
	totalTime = (L/F)*1000000;
	int64_t x_0,y_0,x_1,y_1, delta_X, delta_Y, D, D1, D2, D3,x,y, x_cnt, y_cnt;
	 I = x_b - x_c;
                J = y_b - y_c;
                x_0 = round(I / blu);
                y_0 = round(J / blu);
                x_1 = round(x_f/blu);
                y_1 = round(y_f/blu);
                delta_X =  2 * x_0 + 1;
                delta_Y = -2 * y_0 + 1;
                
                D = 0;
                x_cnt = round(x_b / blu);
                y_cnt = round(y_b / blu);
                
                while (x_cnt < x_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x_cnt = x_cnt + 1;
										}
                    else if (abs(D2) < abs(D1) && abs(D2) < abs(D3))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y_cnt = y_cnt - 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x_cnt = x_cnt + 1;
                        y_cnt = y_cnt - 1;
										}
										cnt++;
									}
                /**/
									numOfPoint = cnt;
								timePerStep = totalTime/cnt;
						if(state == STATE_INTER){
								delta_X =  2 * x_0 + 1;
                delta_Y = -2 * y_0 + 1;
                
                D = 0;
                x = round(x_b / blu);
                y = round(y_b / blu);
							  /**/
                while (x < x_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x = x + 1;
										}
                    else if (abs(D2) < abs(D1) && abs(D2) < abs(D3))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y = y - 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x = x + 1;
                        y = y - 1;
										}
										e+=e_inc;
										setpoint1 = x;
										setpoint2 = y;
										setpoint4 = round(e/blu_e);
										Delay_us(timePerStep);
									}
								}
}

void drawCircleQuadrant4CW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t state)
{
	uint32_t cnt = 0;
	float I,J, r, L, pi  = 3.14,totalTime, timePerStep, e = current_step4*blu_e;
	//Tinh do dai cung
	float theta0, theta1, delta_theta;
	
	theta0 = atan2(fabs(y_b - y_c),fabs(x_b - x_c));
	if(x_f == x_c) theta1 = pi/2;
	if(x_f != x_c) theta1 = atan2(fabs(y_f - y_c),fabs(x_f - x_c));
	delta_theta = fabs(theta1 - theta0);
	r = sqrt((x_b - x_c)*(x_b - x_c) + (y_b - y_c)*(y_b - y_c));
	L = (delta_theta)*r;
	totalTime = (L/F)*1000000;
	int64_t x_0,y_0,x_1,y_1, delta_X, delta_Y, D, D1, D2, D3,x,y, x_cnt, y_cnt;
	 I = x_b - x_c;
                J = y_b - y_c;
                x_0 = round(I / blu);
                y_0 = round(J / blu);
                x_1 = round(x_f/blu);
                y_1 = round(y_f/blu);
                delta_X = -2 * x_0 + 1;
                delta_Y = -2 * y_0 + 1;
                
                D = 0;
                x_cnt = round(x_b / blu);
                y_cnt = round(y_b / blu);
                
                /**/
								 while (x_cnt > x_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x_cnt = x_cnt - 1;
										}
                    else if (abs(D2) < abs(D1) && abs(D2) < abs(D3))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y_cnt = y_cnt - 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x_cnt = x_cnt - 1;
                        y_cnt = y_cnt - 1;
										}
										cnt++;
									}
								
									/**/
									numOfPoint = cnt;
									timePerStep = totalTime/cnt;
						if(state == STATE_INTER){
								delta_X = -2 * x_0 + 1;
                delta_Y = -2 * y_0 + 1;
                
                D = 0;
                x = round(x_b / blu);
                y = round(y_b / blu);
								setpoint1 = x;
								setpoint2 = y;
                /**/
                while (x > x_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x = x - 1;
										}
                    else if (abs(D2) < abs(D1) && abs(D2) < abs(D3))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y = y - 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x = x - 1;
                        y = y - 1;
										}
										e+= e_inc;
										setpoint1 = x;
										setpoint2 = y;
										setpoint4 = round(e/blu_e);
										Delay_us(timePerStep);
									}
								}
}

void drawCircleQuadrant4CCW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t state)
	{
	uint32_t cnt = 0;
	float I,J, r, L, pi  = 3.14,totalTime, timePerStep, e = current_step4*blu_e;
	//Tinh do dai cung
	float theta0, theta1, delta_theta;
	
	theta1 = atan2(fabs(y_f - y_c),fabs(x_f - x_c));
	if(x_b == x_c) theta0 = pi/2;
	if(x_b != x_c) theta0 = atan2(fabs(y_b - y_c),fabs(x_b - x_c));
	delta_theta = fabs(theta1 - theta0);
	r = sqrt((x_b - x_c)*(x_b - x_c) + (y_b - y_c)*(y_b - y_c));
	L = (delta_theta)*r;
	totalTime = (L/F)*1000000;
	int64_t x_0,y_0,x_1,y_1, delta_X, delta_Y, D, D1, D2, D3,x,y, x_cnt, y_cnt;
	 I = x_b - x_c;
                J = y_b - y_c;
                x_0 = round(I / blu);
                y_0 = round(J / blu);
                x_1 = round(x_f/blu);
                y_1 = round(y_f/blu);
                delta_X =  2 * x_0 + 1;
                delta_Y =  2 * y_0 + 1;
                
                D = 0;
                x_cnt = round(x_b / blu);
                y_cnt = round(y_b / blu);
                
             
                /**/
							  while (y_cnt < y_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x_cnt = x_cnt + 1;
										}
                    else if (abs(D2) < abs(D1) && abs(D2) < abs(D3))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y_cnt = y_cnt + 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x_cnt = x_cnt + 1;
                        y_cnt = y_cnt + 1;
										}
										cnt++;
									}
								/**/
									numOfPoint = cnt;
									timePerStep = totalTime/cnt;
						if(state == STATE_INTER){
									delta_X =  2 * x_0 + 1;
                delta_Y =  2 * y_0 + 1;
                
                D = 0;
                x = round(x_b / blu);
                y = round(y_b / blu);
								/**/
                while (y < y_1)
								{
                    D1 = D + delta_X;
                    D2 = D + delta_Y;
                    D3 = D + delta_X + delta_Y;
            
                    if ((abs(D1) < abs(D2)) && (abs(D1) < abs(D3)))
                    {    
												D = D + delta_X;
                        delta_X = delta_X + 2;
                        x = x + 1;
										}
                    else if (abs(D2) < abs(D1) && abs(D2) < abs(D3))
										{
                        D = D + delta_Y;
                        delta_Y = delta_Y + 2;
                        y = y + 1;
										}
                    else
                    {    
												D = D + delta_X + delta_Y;
                        delta_X = delta_X + 2;
                        delta_Y = delta_Y + 2;
                        x = x + 1;
                        y = y + 1;
										}
										e+=e_inc;
										setpoint1 = x;
										setpoint2 = y;
										setpoint4 = round(e/blu_e);
										Delay_us(timePerStep);
									}
								}
}

void drawCircleCW(float x_b,float y_b,float x_f,float y_f,float I,float J,float blu, float F, float E, float blu_e)
{
	float x_c,y_c, r, e_inc = 0;
	uint32_t totalNumOfPoint = 0;
	 x_c = x_b + I;
	y_c = y_b + J;
            //bán kính r
            r = sqrt((x_b - x_c)*(x_b - x_c) + (y_b - y_c)*(y_b - y_c));
            if ((x_b >= x_c) && (y_b > y_c))
						{							//B?t d?u ? góc ph?n tu th? 1
               if ((x_f > x_c) && (y_f >= y_c)) //K?t thúc ? góc ph?n tu th? 1
                  drawCircleQuadrant1CW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
							    totalNumOfPoint += numOfPoint;
							    e_inc = E/(float)totalNumOfPoint;
							    drawCircleQuadrant1CW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
            
               if ((x_f <= x_c) && (y_f > y_c)) //K?t thúc ? góc ph?n tu th? 2
               {   
									drawCircleQuadrant1CW(x_b, y_b, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant4CW(x_c + r, y_c, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant3CW(x_c, y_c - r, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant2CW(x_c - r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
								 e_inc = E/(float)totalNumOfPoint;
								 drawCircleQuadrant1CW(x_b, y_b, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
								 drawCircleQuadrant4CW(x_c + r, y_c, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
								 drawCircleQuadrant3CW(x_c, y_c - r, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
								 drawCircleQuadrant2CW(x_c - r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
								 
               } 
               if ((x_f < x_c) && (y_f <= y_c)) //K?t thúc ? góc ph?n tu th? 3
               {  
									drawCircleQuadrant1CW(x_b, y_b, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant4CW(x_c + r, y_c, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant3CW(x_c , y_c - r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
								 //
								 e_inc = E/(float)totalNumOfPoint;
								 //
								 drawCircleQuadrant1CW(x_b, y_b, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant4CW(x_c + r, y_c, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant3CW(x_c , y_c - r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
               if ((x_f >= x_c) && (y_f < y_c)) //K?t thúc ? góc ph?n tu th? 4
							 {
                  drawCircleQuadrant1CW(x_b, y_b, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant4CW(x_c + r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
								 //
								 e_inc = E/(float)totalNumOfPoint;
								 //
								  drawCircleQuadrant1CW(x_b, y_b, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant4CW(x_c + r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
						 }
            
            if ((x_b < x_c) && (y_b >= y_c)) //B?t d?u ? góc ph?n tu th? 2
						{ 
							if ((x_f > x_c) && (y_f >= y_c)) //K?t thúc ? góc ph?n tu th? 1
              {    
									drawCircleQuadrant2CW(x_b, y_b, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant1CW(x_c, y_c + r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
								  //
								e_inc = E/(float)totalNumOfPoint;
								  //
								  drawCircleQuadrant2CW(x_b, y_b, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant1CW(x_c, y_c + r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
							}
               if ((x_f <= x_c) && (y_f > y_c)) //K?t thúc ? góc ph?n tu th? 2
               {   
								drawCircleQuadrant2CW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumOfPoint += numOfPoint;
								 //
								 e_inc = E/(float)totalNumOfPoint;
								 //
								 drawCircleQuadrant2CW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F , e_inc, blu_e, STATE_INTER);
							 }
               if ((x_f < x_c) && (y_f <= y_c)) //K?t thúc ? góc ph?n tu th? 3
               {   
									drawCircleQuadrant2CW(x_b, y_b, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant1CW(x_c, y_c + r, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant4CW(x_c + r, y_c, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant3CW(x_c, y_c - r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumOfPoint += numOfPoint;
								 //
								 e_inc = E/(float)totalNumOfPoint;
								 //
								 drawCircleQuadrant2CW(x_b, y_b, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant1CW(x_c, y_c + r, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant4CW(x_c + r, y_c, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant3CW(x_c, y_c - r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
               if ((x_f >= x_c) && (y_f < y_c)) //K?t thúc ? góc ph?n tu th? 4
               {   
									drawCircleQuadrant2CW(x_b, y_b, x_c, y_c + r, x_c, y_c, blu, F,  e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant1CW(x_c, y_c + r, x_c + r, y_c, x_c, y_c, blu, F,  e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant4CW(x_c + r, y_c, x_f, y_f, x_c, y_c, blu, F,  e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
								 //
								 e_inc = E/(float)totalNumOfPoint;
								 //
								 drawCircleQuadrant2CW(x_b, y_b, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant1CW(x_c, y_c + r, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant4CW(x_c + r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
						 }
            //
            if ((x_b <= x_c) && (y_b < y_c)) //B?t d?u ?w góc ph?n tu th? 3
						{  
							if ((x_f > x_c) && (y_f >= y_c)) //K?t thúc ? góc ph?n tu th? 1
               {   
									drawCircleQuadrant3CW(x_b, y_b, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant2CW(x_c - r, y_c, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant1CW(x_c, y_c + r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
								 //
								  e_inc = E/(float)totalNumOfPoint;
								 //
								 drawCircleQuadrant3CW(x_b, y_b, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant2CW(x_c - r, y_c, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant1CW(x_c, y_c + r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
               
               if ((x_f <= x_c) && (y_f > y_c)) //K?t thúc ? góc ph?n tu th? 2
               {   
									drawCircleQuadrant3CW(x_b, y_b, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
									drawCircleQuadrant2CW(x_c - r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
								 //
								   e_inc = E/(float)totalNumOfPoint;
								 //
								 drawCircleQuadrant3CW(x_b, y_b, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
									drawCircleQuadrant2CW(x_c - r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
               if ((x_f < x_c) && (y_f <= y_c)) //K?t thúc ? góc ph?n tu th? 3
               {   
									drawCircleQuadrant3CW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F,  e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
								 //
								 e_inc = E/(float)totalNumOfPoint;
								 //
								 drawCircleQuadrant3CW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F,  e_inc, blu_e, STATE_INTER);
               }
               if ((x_f >= x_c) && (y_f < y_c)) //K?t thúc ? góc ph?n tu th? 4
               {
									drawCircleQuadrant3CW(x_b, y_b, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant2CW(x_c - r, y_c, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant1CW(x_c, y_c + r, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant4CW(x_c + r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
								 //
								  e_inc = E/(float)totalNumOfPoint;
								 //
								 drawCircleQuadrant3CW(x_b, y_b, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant2CW(x_c - r, y_c, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant1CW(x_c, y_c + r, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant4CW(x_c + r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
						 }
						//
            if ((x_b > x_c) && (y_b <= y_c)) //B?t d?u ? góc ph?n tu th? 4
            {   
							 if ((x_f > x_c) && (y_f >= y_c)) //K?t thúc ? góc ph?n tu th? 1
               {
									drawCircleQuadrant4CW(x_b, y_b, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumOfPoint+= numOfPoint;
                  drawCircleQuadrant3CW(x_c, y_c - r, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant2CW(x_c - r, y_c, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant1CW(x_c, y_c + r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
								 //
								 e_inc = E/(float)totalNumOfPoint;
								 //
								 drawCircleQuadrant4CW(x_b, y_b, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant3CW(x_c, y_c - r, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant2CW(x_c - r, y_c, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant1CW(x_c, y_c + r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
							 }
               if ((x_f <= x_c) && (y_f > y_c)) //K?t thúc ? góc ph?n tu th? 2
               {   
									drawCircleQuadrant4CW(x_b, y_b, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant3CW(x_c, y_c - r, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant2CW(x_c - r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
								 //
								  e_inc = E/(float)totalNumOfPoint;
								 //
								 drawCircleQuadrant4CW(x_b, y_b, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant3CW(x_c, y_c - r, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant2CW(x_c - r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
							 }
               if ((x_f < x_c) && (y_f <= y_c)) //K?t thúc ? góc ph?n tu th? 3
               {   
									drawCircleQuadrant4CW(x_b, y_b, x_c, y_c - r, x_c, y_c, blu, F,  e_inc, blu_e, STATE_CNT);
								  totalNumOfPoint += numOfPoint;
                  drawCircleQuadrant3CW(x_c, y_c - r, x_f, y_f, x_c, y_c, blu, F,  e_inc, blu_e, STATE_CNT);
								  totalNumOfPoint += numOfPoint;
								 //
								  e_inc = E/(float)totalNumOfPoint;
								 //
								 drawCircleQuadrant4CW(x_b, y_b, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant3CW(x_c, y_c - r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
               if ((x_f >= x_c) && (y_f < y_c)) //K?t thúc ? góc ph?n tu th? 4
               {   
								drawCircleQuadrant4CW(x_b, y_b, x_f, y_f, x_c, y_c, blu,F, e_inc, blu_e, STATE_CNT);
								 totalNumOfPoint += numOfPoint;
								 //
								 e_inc = E/(float)totalNumOfPoint;
								 //
								 drawCircleQuadrant4CW(x_b, y_b, x_f, y_f, x_c, y_c, blu,F,e_inc, blu_e, STATE_INTER);
               }
						 }
}

void drawCircleCCW(float x_b,float y_b,float x_f,float y_f,float I,float J,float blu, float F, float E, float blu_e)
{
	float x_c,y_c, r, e_inc = 0;
	uint32_t totalNumPoint = 0;
	x_c = x_b + I;
	y_c = y_b + J;
	 //bán kính r
            r = sqrt((x_b - x_c)*(x_b - x_c) + (y_b - y_c)*(y_b - y_c));
	          
            if ((x_b > x_c) && (y_b >= y_c))
						{							//B?t d?u ? góc ph?n tu th? 1
               if ((x_f >= x_c) && (y_f > y_c)){ //K?t thúc ? góc ph?n tu th? 1
                  drawCircleQuadrant1CCW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F,  e_inc, blu_e, STATE_CNT);
							    totalNumPoint+= numOfPoint;
							    e_inc = E/(float)totalNumPoint;
							    drawCircleQuadrant1CCW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F,  e_inc, blu_e, STATE_INTER); 
							 }
               if ((x_f < x_c) && (y_f >= y_c)) //K?t thúc ? góc ph?n tu th? 2
               {   
									drawCircleQuadrant1CCW(x_b, y_b, x_c, y_c + r, x_c, y_c, blu, F,  e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
                  drawCircleQuadrant2CCW(x_c, y_c + r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
								  e_inc = E/(float)totalNumPoint;
								  drawCircleQuadrant1CCW(x_b, y_b, x_c, y_c + r, x_c, y_c, blu, F,  e_inc, blu_e, STATE_INTER);
								  drawCircleQuadrant2CCW(x_c, y_c + r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               } 
               if ((x_f <= x_c) && (y_f < y_c)) //K?t thúc ? góc ph?n tu th? 3
               {  
									drawCircleQuadrant1CCW(x_b, y_b, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
                  drawCircleQuadrant2CCW(x_c, y_c + r, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
                  drawCircleQuadrant3CCW(x_c - r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
								  e_inc = E/(float)totalNumPoint;
								  drawCircleQuadrant1CCW(x_b, y_b, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
								 drawCircleQuadrant2CCW(x_c, y_c + r, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
								 drawCircleQuadrant3CCW(x_c - r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
								 
               }
               if ((x_f > x_c) && (y_f <= y_c)) //K?t thúc ? góc ph?n tu th? 4
							 {
                  drawCircleQuadrant1CCW(x_b, y_b, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
                  drawCircleQuadrant2CCW(x_c, y_c + r, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
                  drawCircleQuadrant3CCW(x_c - r, y_c, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
                  drawCircleQuadrant4CCW(x_c, y_c - r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
								 e_inc = E/(float)totalNumPoint;
								 
								 drawCircleQuadrant1CCW(x_b, y_b, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant2CCW(x_c, y_c + r, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant3CCW(x_c - r, y_c, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant4CCW(x_c, y_c - r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
						 }
            
            if ((x_b <= x_c) && (y_b > y_c)) //B?t d?u ? góc ph?n tu th? 2
						{ 
							if ((x_f >= x_c) && (y_f > y_c)) //K?t thúc ? góc ph?n tu th? 1
              {    
									drawCircleQuadrant2CCW(x_b, y_b, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
                  drawCircleQuadrant3CCW(x_c - r, y_c, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
                  drawCircleQuadrant4CCW(x_c, y_c - r, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
                  drawCircleQuadrant1CCW(x_c + r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
								  e_inc = E/(float)totalNumPoint;
								
								  drawCircleQuadrant2CCW(x_b, y_b, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant3CCW(x_c - r, y_c, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant4CCW(x_c, y_c - r, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant1CCW(x_c + r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
							}
               if ((x_f < x_c) && (y_f >= y_c)) //K?t thúc ? góc ph?n tu th? 2
               {   
									drawCircleQuadrant2CCW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
								  e_inc = E/(float)totalNumPoint;
								 drawCircleQuadrant2CCW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
							 }
               if ((x_f <= x_c) && (y_f < y_c)) //K?t thúc ? góc ph?n tu th? 3
               {   
									drawCircleQuadrant2CCW(x_b, y_b, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
                  drawCircleQuadrant3CCW(x_c - r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
								 e_inc = E/(float)totalNumPoint;
								 
								 drawCircleQuadrant2CCW(x_b, y_b, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant3CCW(x_c - r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
               if ((x_f > x_c) && (y_f <= y_c)) //K?t thúc ? góc ph?n tu th? 4
               {   
									drawCircleQuadrant2CCW(x_b, y_b, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
                  drawCircleQuadrant3CCW(x_c - r, y_c, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
                  drawCircleQuadrant4CCW(x_c, y_c - r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
								  e_inc = E/(float)totalNumPoint;
								 
                  drawCircleQuadrant2CCW(x_b, y_b, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant3CCW(x_c - r, y_c, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant4CCW(x_c, y_c - r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
						 }
            
            if ((x_b < x_c) && (y_b <= y_c)) //B?t d?u ?w góc ph?n tu th? 3
						{  
							if ((x_f >= x_c) && (y_f > y_c)) //K?t thúc ? góc ph?n tu th? 1
               {   
									drawCircleQuadrant3CCW(x_b, y_b, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
                  drawCircleQuadrant4CCW(x_c, y_c - r, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
                  drawCircleQuadrant1CCW(x_c + r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
								  e_inc = E/(float)totalNumPoint;
								 
								  drawCircleQuadrant3CCW(x_b, y_b, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant4CCW(x_c, y_c - r, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant1CCW(x_c + r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
               if ((x_f < x_c) && (y_f >= y_c)) //K?t thúc ? góc ph?n tu th? 2
               {   
									drawCircleQuadrant3CCW(x_b, y_b, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
                  drawCircleQuadrant4CCW(x_c, y_c - 2, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
                  drawCircleQuadrant1CCW(x_c + r, y_c, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
                  drawCircleQuadrant2CCW(x_c, y_c + r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
								 e_inc = E/(float)totalNumPoint;
								 
                  drawCircleQuadrant3CCW(x_b, y_b, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant4CCW(x_c, y_c - 2, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant1CCW(x_c + r, y_c, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant2CCW(x_c, y_c + r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
               if ((x_f <= x_c) && (y_f < y_c)) //K?t thúc ? góc ph?n tu th? 3
               {   
									drawCircleQuadrant3CCW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
								 e_inc = E/(float)totalNumPoint;
								 drawCircleQuadrant3CCW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
               if ((x_f > x_c) && (y_f <= y_c)) //K?t thúc ? góc ph?n tu th? 4
               {
									drawCircleQuadrant3CCW(x_b, y_b, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
                  drawCircleQuadrant4CCW(x_c, y_c - r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
								  e_inc = E/(float)totalNumPoint;
								 
								 drawCircleQuadrant3CCW(x_b, y_b, x_c, y_c - r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant4CCW(x_c, y_c - r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
               }
						 }
						//
            if ((x_b >= x_c) && (y_b < y_c)) //B?t d?u ? góc ph?n tu th? 4
            {   
							 if ((x_f >= x_c) && (y_f > y_c)) //K?t thúc ? góc ph?n tu th? 1
               {
									drawCircleQuadrant4CCW(x_b, y_b, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
                  drawCircleQuadrant1CCW(x_c + r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
								 e_inc = E/(float)totalNumPoint;
								 
								 drawCircleQuadrant4CCW(x_b, y_b, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant1CCW(x_c + r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
								 
      				 }
               if ((x_f < x_c) && (y_f >= y_c)) //K?t thúc ? góc ph?n tu th? 2
               {   
									drawCircleQuadrant4CCW(x_b, y_b, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
                  drawCircleQuadrant1CCW(x_c + r, y_c, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
                  drawCircleQuadrant2CCW(x_c, y_c + r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								  totalNumPoint+= numOfPoint;
								  e_inc = E/(float)totalNumPoint;
								 
								 drawCircleQuadrant4CCW(x_b, y_b, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant1CCW(x_c + r, y_c, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                  drawCircleQuadrant2CCW(x_c, y_c + r, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);

							 }
							 
               if ((x_f <= x_c) && (y_f < y_c)) //K?t thúc ? góc ph?n tu th? 3
               {   
									 drawCircleQuadrant4CCW(x_b, y_b, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
									 drawCircleQuadrant1CCW(x_c + r, y_c, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
                   drawCircleQuadrant2CCW(x_c, y_c + r, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
                   drawCircleQuadrant3CCW(x_c - r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT);
								 totalNumPoint+= numOfPoint;
								 e_inc = E/(float)totalNumPoint;
								 
								 drawCircleQuadrant4CCW(x_b, y_b, x_c + r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
									 drawCircleQuadrant1CCW(x_c + r, y_c, x_c, y_c + r, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                   drawCircleQuadrant2CCW(x_c, y_c + r, x_c - r, y_c, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
                   drawCircleQuadrant3CCW(x_c - r, y_c, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER);
							 }
							 if ((x_f > x_c) && (y_f <= y_c)) //K?t thúc ? góc ph?n tu th? 4
                  
									drawCircleQuadrant4CCW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_CNT );
							    totalNumPoint+= numOfPoint;
								 e_inc = E/(float)totalNumPoint;
							 
                  drawCircleQuadrant4CCW(x_b, y_b, x_f, y_f, x_c, y_c, blu, F, e_inc, blu_e, STATE_INTER );
						 }
						
}
