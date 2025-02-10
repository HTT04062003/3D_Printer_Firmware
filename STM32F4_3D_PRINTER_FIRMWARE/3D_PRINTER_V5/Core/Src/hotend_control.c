#include "hotend_control.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//
extern ADC_HandleTypeDef hadc1;
#define myadc &hadc1
#define tempHotend_Channel ADC_CHANNEL_14
#define tempHedbed_Channel ADC_CHANNEL_15
//
#define ADC_Resolution (float)(4095)
#define V_Ref          (float)(3.3)
#define R_PULLUP (float)(10000)
#define BETA (float)(3950)
#define R0   (float)(95000)
#define T0   (float)(298)

//
volatile uint16_t Val[2] = {0,0};
float U1= 0;
float U2= 0;
float R1 = 0.0;
float R2 = 0.0;
float TempHotend = 0;
float TempHedbed = 0;
float Kp_THotend = 21.36;
float Ki_THotend = 4.26;
float Kd_THotend = 15.87;
float Kb_THotend = 2;
float setTempHotend = 0;
float Kp_THedbed = 21.36;
float Ki_THedbed = 4.26;
float Kd_THedbed = 15.87;
float Kb_THedbed = 2;
float setTempHedbed = 0;
//
LowPassFilter filterTempHotend;
LowPassFilter filterTempHedbed;
//
void ADC_Channel_config(uint32_t Channel){
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = Channel;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	HAL_ADC_ConfigChannel(myadc, &sConfig);
}
//
void readTempValue(float *tempHotendVar, float *tempHedbedVar){
	//
	float adcValue = 0;
	float U = 0;
	float R = 0;
	*myadc.Instance->DR = 0;
	ADC_Channel_config(tempHotend_Channel);
	for(uint8_t i = 0; i<16; i++){
		HAL_ADC_Start(myadc);
		HAL_ADC_PollForConversion(myadc, 1000);
		adcValue += HAL_ADC_GetValue(myadc);
	}
	adcValue = adcValue/16.0;
	U = (float )V_Ref*adcValue/ADC_Resolution;
	R = (R_PULLUP*U)/(V_Ref - U);
	*tempHotendVar = (BETA/(log(R/R0) + BETA/T0) - 273);
	*tempHotendVar = LowPassFilter_Apply(&filterTempHotend,*tempHotendVar);
	HAL_ADC_Stop(&hadc1);
	//
	adcValue = 0;
	*myadc.Instance->DR = 0;
	ADC_Channel_config(tempHedbed_Channel);
	for(uint8_t i = 0; i<16; i++){
		HAL_ADC_Start(myadc);
		HAL_ADC_PollForConversion(myadc, 1000);
		adcValue += HAL_ADC_GetValue(myadc);
	}
	adcValue = adcValue/16.0;
	U = (float )V_Ref*adcValue/ADC_Resolution;
	R = (R_PULLUP*U)/(V_Ref - U);
	*tempHedbedVar = (BETA/(log(R/R0) + BETA/T0) - 273);
	*tempHedbedVar = LowPassFilter_Apply(&filterTempHedbed,*tempHedbedVar);
	HAL_ADC_Stop(&hadc1);
}
//
float LowPassFilter_Apply(LowPassFilter *filter, float input) {
    float output = filter->alpha * input + (1.0f - filter->alpha) * filter->prev_output;
    filter->prev_output = output; // Luu d?u ra d? dùng cho l?n sau
    return output;
}
void LowPassFilter_Init(LowPassFilter *filter, float alpha) {
    filter->alpha = alpha;
    filter->prev_output = 0.0f; // Ban d?u giá tr? b?ng 0
}
int Hotend_Temperatrure_PIDCaculating( float designPos, float currentPos){
	static float err_p= 0 ;
	static float ui_p = 0;
	static float u_sat = 0;
	float err , up, ud, ui;
	int PWMout;
	
	err = designPos - currentPos;
	
	up = Kp_THotend*err;
	ud = Kd_THotend*(err - err_p)/SAMPLE_TIME;
	ui = ui_p + Ki_THotend*err*SAMPLE_TIME  + Kb_THotend*SAMPLE_TIME*u_sat;
	 // Tính toán dau ra PWM truoc khi gioi han
    float raw_PWMout = up + ud + ui;
	// Gioi han dau ra PWM và áp dung anti-windup
    if (raw_PWMout > HILIM) {
        PWMout = HILIM;
        //ui -= raw_PWMout - HILIM_POS;  // Ði?u ch?nh thành ph?n tích phân
    } else if (raw_PWMout < LOLIM) {
        PWMout = LOLIM;
        //ui -= raw_PWMout - LOLIM_POS;  // Ði?u ch?nh thành ph?n tích phân
    } else {
        PWMout = (int)raw_PWMout;
    } 
    u_sat = PWMout - raw_PWMout; 		
  	err_p = err;
	ui_p = ui;
	return PWMout;
	}
int Hedbed_Temperatrure_PIDControl( float designTemp, float currentTemp){
	static float err_p= 0 ;
	static float ui_p = 0;
	static float u_sat = 0;
	float err , up, ud, ui;
	int PWMout;
	
	err = designTemp - currentTemp;
	
	up = Kp_THedbed*err;
	ud = Kd_THedbed*(err - err_p)/SAMPLE_TIME;
	ui = ui_p + Ki_THedbed*err*SAMPLE_TIME  + Kb_THedbed*SAMPLE_TIME*u_sat;
	 // Tính toán dau ra PWM truoc khi gioi han
    float raw_PWMout = up + ud + ui;
	// Gioi han dau ra PWM và áp dung anti-windup
    if (raw_PWMout > HILIM) {
        PWMout = HILIM;
        //ui -= raw_PWMout - HILIM_POS;  // Ði?u ch?nh thành ph?n tích phân
    } else if (raw_PWMout < LOLIM) {
        PWMout = LOLIM;
        //ui -= raw_PWMout - LOLIM_POS;  // Ði?u ch?nh thành ph?n tích phân
    } else {
        PWMout = (int)raw_PWMout;
    } 
    u_sat = PWMout - raw_PWMout; 		
  	err_p = err;
	ui_p = ui;
	return PWMout;
	
	}
//