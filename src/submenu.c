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
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	lcdPutS("Otvoril som Info",23, 17, 0x0000, 0xFFFF);
}

void otvorRevers(){
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	lcdPutS("Otvoril som Revers",23, 17, 0x0000, 0xFFFF);
}

void otvorExpo(){
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	lcdPutS("Otvoril som Expo",23, 17, 0x0000, 0xFFFF);
}

void otvorMix(){
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	lcdPutS("Otvoril som Mix",23, 17, 0x0000, 0xFFFF);
}

void otvorEPA(){
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	lcdPutS("Otvoril som EPA",23, 17, 0x0000, 0xFFFF);
}
