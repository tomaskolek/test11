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


void otvorInfo(){
	subMenu = 1;
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	lcdPutS("Otvoril som Info",23, 17, 0x0000, 0xFFFF);
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
