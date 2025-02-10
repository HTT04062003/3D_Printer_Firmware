#ifndef _NOISUY_H_
#define _NOISUY_H_
#include <stdint.h>
#include <stdbool.h>
#define FLASH_PAGE31_startAddress  0x08007C00 
#define current_step1_startAddress FLASH_PAGE31_startAddress
#define current_step2_startAddress (FLASH_PAGE31_startAddress + 8)
#define setpoint1_startAddress  (FLASH_PAGE31_startAddress + 16)
#define setpoint2_startAddress (FLASH_PAGE31_startAddress + 	24)
//
#define X_STEP 0.0125
#define Y_STEP 0.0125
#define Z_STEP 0.000625
#define E_STEP 0.01
//
#define MODE_NO 0
#define MODE_COUNT 50
#define MODE_DDA 1
#define MODE_CW 2
#define MODE_CCW 3
//
#define MODE_CW1 4
#define MODE_CW2 5
#define MODE_CW3 6
#define MODE_CW4 7
#define MODE_CCW1 8
#define MODE_CCW2 9
#define MODE_CCW3 10
#define MODE_CCW4 11
//
//
#define CW_STOP_IN_Q1 4
#define CW_STOP_IN_Q2 5
#define CW_STOP_IN_Q3 6
#define CW_STOP_IN_Q4 7
#define CCW_STOP_IN_Q1 8
#define CCW_STOP_IN_Q2 9
#define CCW_STOP_IN_Q3 10
#define CCW_STOP_IN_Q4 11
//
#define CW_START_IN_Q1 12
#define CW_START_IN_Q2 13
#define CW_START_IN_Q3 14
#define CW_START_IN_Q4 15
#define CCW_START_IN_Q1 16
#define CCW_START_IN_Q2 17
#define CCW_START_IN_Q3 18
#define CCW_START_IN_Q4 19
//
#define TMP_MODE_INIT 0
#define TMP_MODE_RUN  1
//
#define PULSE_X 1
#define NO_PULSE_X 0
#define PULSE_Y 1
#define NO_PULSE_Y 0
//
#define STATE_COUNT 0xFF
#define STATE_RUN   0x00
#define STATE_STOP  0x01
//
typedef struct{
	float dx;
	float dy;
	float x_inc;
	float y_inc;
	uint32_t cnt;
}DDA_Parameter_TypeDef;
typedef struct {
	float I;
	float J;
	float R;
  int64_t D;
  int64_t D1;
  int64_t D2;
  int64_t D3;
  int64_t DeltaX;
  int64_t DeltaY;	
	int64_t x_c;// blu
	int64_t y_c;//blu
	uint8_t stopPoint;
	uint8_t startPoint;
}DS_Parameter_TypdeDef;
typedef struct{
	float E0;
	float e;
	float e_inc;
	}Extruder_Parameter_TypeDef;

//
#define STATE_CNT 0xFF
#define STATE_INTER 0x00
extern int64_t setpoint1;
extern int64_t current_step1;
extern int64_t setpoint2;
extern int64_t current_step2;
extern int64_t setpoint3;
extern int64_t current_step3;
extern int64_t setpoint4;
extern int64_t current_step4;
extern uint32_t numOfPoint;
extern uint32_t timePerStep ;
extern volatile float X, Y, Z, F, I, J, E, BLU_E;
extern volatile uint8_t  G_FLAG;
extern float BLU;
extern float BLU_Z;
//extern	Interpolation_TypDef Interpolation_handle;
///*****Prototype****/
//	void Init(void);
//	uint8_t startPoint(Interpolation_TypDef *handle);
//	uint8_t stopPoint( Interpolation_TypDef *handle);
//	bool stopPointCheck(Interpolation_TypDef *handle);
//	void pulse(Interpolation_TypDef *handle,  uint8_t signalX, uint8_t signalY);
//	void DS_START( Interpolation_TypDef *handle, float x_b, float y_b, float x_f, float y_f, float I, float J, float F);
//	void DS_Control( Interpolation_TypDef *handle);
//	uint8_t tmpModeCheck(Interpolation_TypDef *handle);
//	void DS_NumOfPointInterpolationCount(Interpolation_TypDef *handle);
void MOVE_Z( float z0, float z1,float bluz, float F);
void MOVE_Extruder(float blue, float E, float F);
void DDA_LINE(float x_0, float y_0, float x_1, float y_1, float blu, float F, float E, float blu_e);
//void drawCircleQuadrant1CW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F);
void drawCircleQuadrant1CW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t state);
void drawCircleQuadrant1CCW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F , float e_inc, float blu_e, uint8_t state);
void drawCircleQuadrant2CW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu,float F, float e_inc, float blu_e, uint8_t state);
void drawCircleQuadrant2CCW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t state);
void drawCircleQuadrant3CW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t state);
void drawCircleQuadrant3CCW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t state);
void drawCircleQuadrant4CW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t state);
void drawCircleQuadrant4CCW(float x_b,float y_b,float x_f,float y_f,float x_c,float y_c,float blu, float F, float e_inc, float blu_e, uint8_t state);
//void drawCircleCW(float x_b,float y_b,float x_f,float y_f,float I,float J,float blu, float F);
//void drawCircleCCW(float x_b,float y_b,float x_f,float y_f,float I,float J,float blu, float F);
void drawCircleCW(float x_b,float y_b,float x_f,float y_f,float I,float J,float blu, float F, float E, float blu_e);
void drawCircleCCW(float x_b,float y_b,float x_f,float y_f,float I,float J,float blu, float F, float E, float blu_e);
void Delay_us(uint16_t us);
void test(void);

#endif