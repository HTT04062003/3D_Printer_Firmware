#include "gcode.h"
#include <stdint.h>
/**********************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
volatile uint8_t rxBuffer[48];
uint8_t readGcodeLine(char *lineGcodeBuffer, float *X, float *Y, float *Z, float *I, float *J, float *F, float *THotend, float *THedbed, float *E) {
	uint8_t G_type = G_FLAG_NONE;
    // Bi?n t?m d? gi? giá tr? s? khi g?p X, Y, F
    char *p = lineGcodeBuffer;
    
    // Duy?t qua t?ng ký t? trong chu?i
    while (*p != '\0') {
        // Ki?m tra ký t? và tách giá tr? d?a trên các ký t? X, Y, F
        if (*p == 'X') {
            *X = atof(p + 1); // Chuy?n giá tr? sau 'X' thành s? float
        } else if (*p == 'Y') {
            *Y = atof(p + 1);
        } else if (*p == 'P') {
            *F = atof(p + 1);
        } else if (*p == 'I'){
					  *I = atof(p + 1);
				} else if(*p == 'J'){
				    *J = atof(p + 1);	
				}
				else if (*p == 'Z') {
            *Z = atof(p + 1);
				}
				else if (*p == 'F') {
            *F = atof(p + 1);
				}
				else if(*p == 'G'){
					 if((*(p + 1) == '0' && *(p+2) == '0') )  G_type = G_FLAG_G0; 
				   else if((*(p + 1) == '0' && *(p+2) == '1') || (*(p + 1) == '1'))  G_type = G_FLAG_G1; 
					 else if((*(p + 1) == '0' && *(p+2) == '2') || (*(p + 1) == '2'))  G_type = G_FLAG_G2;
           else if ((*(p + 1) == '0' && *(p+2) == '3') || (*(p + 1) == '3'))  G_type = G_FLAG_G3;
           else if(*(p + 1) == '2' && *(p+2) == '8') G_type = G_FLAG_G28;
           else if(*(p + 1) == '2' && *(p+2) == '9') G_type = G_FLAG_G29;
           else if(*(p + 1) == '9' && *(p+2) == '0') G_type = G_FLAG_G90;
           else if(*(p + 1) == '9' && *(p+2) == '1') G_type = G_FLAG_G91;
           else if(*(p + 1) == '9' && *(p+2) == '2') G_type = G_FLAG_G92;						
				}
				else if (*p == 'M'){
					 if(*(p + 1) == '1' && *(p+2) == '0' && (*(p+3) == '4') && (*(p+4) == ' ' && (*(p+5) == 'S'))){
						*THotend = atof(p+6);
            G_type = G_COMMAND_M104;					 
				 }
					 else if(*(p + 1) == '1' && *(p+2) == '0' && (*(p+3) == '9') && (*(p+4) == ' ' && (*(p+5) == 'S'))){
						 *THotend = atof(p+6);
						 G_type = G_COMMAND_M109;
					 
					 }
					 else if(*(p + 1) == '1' && *(p+2) == '4' && (*(p+3) == '0') && (*(p+4) == ' ' && (*(p+5) == 'S'))){
						 *THedbed = atof(p+6);
						 G_type = G_COMMAND_M140;
					 }
					 else if(*(p + 1) == '1' && *(p+2) == '9' && (*(p+3) == '0') && (*(p+4) == ' ' && (*(p+5) == 'S'))){
						 *THedbed = atof(p+6);
						 G_type = G_COMMAND_M190;
					 }
					 
				}
				else if(*p == 'S'){
					if(*(p + 1) == 'E' && *(p+ 2) == 'T' && *(p + 3) == 'X' && *(p + 4) == 'm' && *(p + 5) == 'm'){
						 G_type= G_COMAND_MOVE_X;
					   *X = atof(p + 6);
             p = p + 6;					
				  }
					else if( *(p + 1) == 'E' && *(p + 2) == 'T' && *( p + 3) == 'Y' && *(p + 4) == 'm' && *(p + 5) == 'm'){
						G_type = G_COMAND_MOVE_Y;
						*Y = atof(p + 6);
					   p = p + 6;
				  }
					else if(*(p + 1) == 'E' && *(p + 2) == 'T' && *( p + 3) == 'Z' && *(p + 4) == 'm' && *(p + 5) == 'm'){
						G_type = G_COMAND_MOVE_Z;
					 *Z = atof(p + 6);
					  p = p + 6;
				  }
					else if(*(p + 1) == 'E' && *(p + 2) == 'T' && *( p + 3) == 'E' && *(p + 4) == 'm' && *(p + 5) == 'm'){
						G_type = G_COMAND_MOVE_E;
					  *E = atof(p + 6);
				  }
				}
        // Tang con tr? d? ki?m tra ký t? ti?p theo
        p++;
    }
		
		return G_type;
}

