/*
 * test1.h
 *
 *  Created on: 2. 12. 2016
 *      Author: ja
 */

#ifndef TEST1_H_
#define TEST1_H_


#include <stdint.h>
volatile uint16_t klavesnica;
int16_t defaultA;
int16_t defaultB;
int16_t aktualneA;
int16_t aktualneB;
int subMenu;
volatile uint16_t ADC1ConvertedValue[5]; //Stores converted vals
volatile uint16_t kridielko, vyskovka, plyn, smerovka;

void initMenu();
void Delay(uint16_t n);
void nvic_init();
void adc_init();
void initGPIO();
void dma_init();
void posunSipkyDole();
void posunSipkyHore();
void pohybMenu(uint16_t);
void otvorInfo();
void otvorRevers();
void otvorExpo();
void otvorMix();
void otvorEPA();
void adc2_init();
char * prevodNaChar(uint16_t);
char * FloatToString(float);
char *FloatToStringReverz(float);
float normalizuj(float ,float ,float );//normalizovanie hodnot od -1 do 1

#endif /* TEST1_H_ */
