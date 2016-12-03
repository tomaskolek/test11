/*
 * submenu.c
 *
 *  Created on: 3. 12. 2016
 *      Author: tomas
 */


#include "test1.h"
#include "spi.h"
#include "ili9163.h"
#include <stddef.h>
#include "stm32l1xx.h"
#include <stdio.h>
#include <stdlib.h>


void otvorInfo(){
	subMenu = 1;
	char str[4];
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	//while(1){

	/*if ((klavesnica >= 3440) && (klavesnica <= 3480)){
		break;
	}*/
	sprintf(str,"%d", hodnota1);
	lcdPutS(str, 23, 17, 0x0000, 0xFFFF);
	sprintf(str,"%d", hodnota2);
	lcdPutS(str, 23, 27, 0x0000, 0xFFFF);
	sprintf(str,"%d", hodnota3);
	lcdPutS(str, 23, 37, 0x0000, 0xFFFF);
	sprintf(str,"%d", hodnota4);
	lcdPutS(str, 23, 47, 0x0000, 0xFFFF);
	//lcdFilledRectangle(20, 15, 20, 40, decodeRgbValue(31, 31, 31));
	//}
}

void otvorRevers(){
	subMenu = 1;
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	lcdPutS("Otvoril som Revers",23, 17, 0x0000, 0xFFFF);

	lcdCircle(64, 84, 22, decodeRgbValue(31, 0, 0));
	lcdCircle(64, 84, 30, decodeRgbValue(0, 31, 0));
	lcdCircle(64, 84, 38, decodeRgbValue(0, 0, 31));
}

void otvorExpo(){
	subMenu = 1;
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	lcdPutS("Otvoril som Expo",23, 17, 0x0000, 0xFFFF);
}

void otvorMix(){
	subMenu = 1;
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	lcdPutS("Otvoril som Mix",23, 17, 0x0000, 0xFFFF);
}

void otvorEPA(){
	subMenu = 1;
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	lcdPutS("Otvoril som EPA",23, 17, 0x0000, 0xFFFF);
}


char * prevodNaChar(uint16_t hodnota){
	static char str[4];
	int c1 = hodnota/1000;
	int c2 = (hodnota-(1000*c1))/100;
	int c3 = (hodnota-(1000*c1 + c2*100))/10;
	int c4 = (hodnota-(1000*c1 + c2*100 + c3*10));
	str[0] = c1 + '0';
	str[1] = c2 + '0';
	str[2] = c3 + '0';
	str[3] = c4 + '0';
	str[4] = '\0';

	return str;
}
