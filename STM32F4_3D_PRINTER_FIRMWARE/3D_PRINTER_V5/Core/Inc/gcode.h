#ifndef _GCODE_H_
#define _GCODE_H_
/************Include**************/
#include <stdint.h>
/**************************/
#define G_FLAG_G0 0
#define G_FLAG_G1 1
#define G_FLAG_G2 2
#define G_FLAG_G3 3
#define G_FLAG_G28 6 //Tat ca cac truc ve toa do goc
#define G_FLAG_G29 7 //Can ban tu dong
#define G_FLAG_G90 8//Toa do tuyet doi
#define G_FLAG_G91 9 //Toa do tuong doi
#define G_FLAG_G92 10 //Dat toa do hien tai la 0
#define G_FLAG_NONE 4
#define G_COMMAND_M104 11 //Dat nhiet do dau in ma khong cho
#define G_COMMAND_M109 12 //Dat nhiet do dau in va cho cho den khi dat gia tri nhiet do
#define G_COMMAND_M140 13 // Ðat nhiet do ban nhiet ma khong cho
#define G_COMMAND_M190 14 //Dat nhiet do ban nhiet va cho
#define G_COMMAND_M106 15 //Bat quat lam mat
#define G_COMMAND_M107 16 //Tat quat
#define G_COMMAND_M82 17 //Dat che do dun tuyet doi
#define G_COMMAND_M83 18 //Dat che do dun tuong doi
#define G_COMMAND_M115 19 //Lay thong tim firmware
#define G_COMMAND_M500 20 //Luu cai dat vao FLASH
#define G_COMMAND_M501 21 //Tai lai cai dat tu FLASH
#define G_COMMAND_M502 22 //Khoi phuc cai dat mac dinh
#define G_COMAND_MOVE_X 23
#define G_COMAND_MOVE_Y 24
#define G_COMAND_MOVE_Z 25
#define G_COMAND_MOVE_E 27
extern volatile uint8_t rxBuffer[48];
/***************Prototype***************/
uint8_t readGcodeLine(char *lineGcodeBuffer, float *X, float *Y, float *Z, float *I, float *J, float *F, float *THotend, float *THedbed, float *E);
#endif