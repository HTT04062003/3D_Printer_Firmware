/**
  ******************************************************************************
  * @file           : i2clcd.h
  * @brief          : Library for I2C LCD display with STM32
  ******************************************************************************
  * @author controllerstech, 2019-05-29
  * https://controllerstech.com/lcd-20x4-using-i2c-with-stm32/
  * @author (modified) Ayleen Weiss, 2021-12-01
  *
  * For I2C SERIAL 20X4 LCD MODULE (SBC-LCD20x4 / 2004A) 
  * with PCF8574 I2C adapter chip
  * Capable of displaying German "Umlauts" ä, ö, ü and ß (sz)
  *
  ******************************************************************************
  */

#ifndef __I2CLCD_H
#define __I2CLCD_H

#include "stm32f4xx_hal.h"
extern const uint8_t DO[8];
extern const uint8_t Hotend1[8];
extern const uint8_t HedBed[8];
void lcd_init(void); // initialize lcd

void lcd_send_cmd(char cmd); // send command to the lcd

void lcd_send_data(char data); // send data to the lcd

void lcd_send_string(const char *str); // send string to the lcd

void lcd_write(const char *txt, uint8_t line, uint8_t column); // write string to specific line

void lcd_clear(void);

void lcd_clear_line(uint8_t line, uint8_t column);
void lcd_create_char(uint8_t location, uint8_t *pattern, uint8_t line,uint8_t colum);
#endif /* __I2CLCD_H */