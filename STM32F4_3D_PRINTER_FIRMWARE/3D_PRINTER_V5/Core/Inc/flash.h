#ifndef _FLASH_H_
#define _FLASH_H_
#include <stdint.h>

/********************struct typdef***********************/
typedef struct {
	float currentX;
	float currentY;
	float currentZ;
	float Kp_Hotend;
	float Ki_Hotend;
	float Kd_Hotend;
	float Kp_Hedbed;
	float Ki_Hedbed;
	float Kd_Hedbed;
	float setHotendTemperature;
	float setHedbedTemperature;
	uint8_t fanSpeedPower;
	uint8_t Positionning;
	}Printer_Init_TypeDef;
#endif