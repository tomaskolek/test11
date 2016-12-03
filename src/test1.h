/*
 * test1.h
 *
 *  Created on: 2. 12. 2016
 *      Author: tomas
 */

#ifndef TEST1_H_
#define TEST1_H_


#include <stdint.h>
volatile uint16_t klavesnica;
int16_t defaultA;
int16_t defaultB;
int16_t aktualneA;
int16_t aktualneB;

void initMenu();
void Delay(uint16_t n);
void nvic_init();
void adc_init(void);
void initGPIO();
void posunSipkyDole();
void posunSipkyHore();
void pohybMenu(uint16_t);
void otvorInfo();
void otvorRevers();
void otvorExpo();
void otvorMix();
void otvorEPA();

#endif /* TEST1_H_ */
