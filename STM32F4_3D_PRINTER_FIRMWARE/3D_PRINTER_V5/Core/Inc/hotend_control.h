#ifndef _HOTEND_CONTROL_H_
#define _HOTEND_CONTROL_H_
//
#define SAMPLE_TIME 0.3
#define HILIM 100
#define LOLIM 0
//
#include <stdint.h>
//
typedef struct {
    float alpha;          // H? s? l?c (0 < alpha <= 1)
    float prev_output;    // Giá tr? d?u ra tru?c dó (dã l?c)
} LowPassFilter;
//
extern volatile uint16_t Val[2];
extern float U1;
extern float U2;
extern float R1;
extern float R2;
extern float TempHotend;
extern float Kp_THotend;
extern float Ki_THotend;
extern float Kd_THotend;
extern float Kb_THotend;
extern LowPassFilter filterTempHotend;
extern LowPassFilter filterTempHedbed;
extern float setTempHotend ;
extern float TempHedbed;
extern float Kp_THedbed;
extern float Ki_THedbed;
extern float Kd_THedbed;
extern float Kb_THedbed;
extern float setTempHedbed;
//

//
void readTempValue(float *tempHotendVar, float *tempHedbedVar);
//
float LowPassFilter_Apply(LowPassFilter *filter, float input);
void LowPassFilter_Init(LowPassFilter *filter, float alpha);
//int PositionPIDCaculating( float designPos, float currentPos);
int Hotend_Temperatrure_PIDCaculating( float designPos, float currentPos);
int Hedbed_Temperatrure_PIDControl( float designTemp, float currentTemp);
//
#endif