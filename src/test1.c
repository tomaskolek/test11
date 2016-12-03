/*
 * test1.c
 *
 *  Created on: 2. 12. 2016
 *      Author: tomas
 */

#include "test1.h"
#include "spi.h"
#include "ili9163.h"
#include <stddef.h>
#include "stm32l1xx.h"

void Delay(uint16_t n)
{
	uint32_t nl = n*2;
	while(nl--);
	return;
}

void pohybMenu(uint16_t klavesnica){

	  if ((klavesnica >= 1952) && (klavesnica <= 2100)){
		  posunSipkyHore();
	  }
	  else if ((klavesnica >= 2800) && (klavesnica <= 2940)){
		  if (aktualneA == 12){
			  otvorInfo();
		  }
		  else if (aktualneA == 32){
			  otvorRevers();
		  }
		  else if (aktualneA == 52){
			  otvorExpo();
		  }
		  else if (aktualneA == 72){
			  otvorMix();
		  }
		  else if (aktualneA == 92){
			  otvorEPA();
		  }
	  }
	  else if ((klavesnica >= 3440) && (klavesnica <= 3480)){
		  lcdClearDisplay(decodeRgbValue(255, 255, 255));
		  lcdPutS("Info", 20, 17, 0x0000, 0xFFFF); 		//aktualneA 12, aktualneB 27
		  lcdPutS("Revers", 20, 37, 0x0000, 0xFFFF); 	//aktualneA 32, aktualneB 47
		  lcdPutS("Expo", 20, 57, 0x0000, 0xFFFF); 		//aktualneA 52, aktualneB 67
		  lcdPutS("Mix", 20, 77, 0x0000, 0xFFFF); 		//aktualneA 72, aktualneB 87
		  lcdPutS("EPA", 20, 97, 0x0000, 0xFFFF); 		//aktualneA 92, aktualneB 107
		  lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
	  }
	  else if ((klavesnica >= 3630) && (klavesnica <= 3690)){
		  posunSipkyDole();
	  }
}

void posunSipkyDole(){
	if (aktualneA == 0){
		lcdPlnyTrojuholnik(5, defaultA, defaultB, decodeRgbValue(31, 31, 31));
		aktualneA = defaultA + 20;
		aktualneB = defaultB + 20;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
	}
	else if (aktualneB >= 100){
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(31, 31, 31));
		lcdPlnyTrojuholnik(5, defaultA, defaultB, decodeRgbValue(0, 0, 0));
		aktualneA = defaultA;
		aktualneB = defaultB;
	}
	else{
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(31, 31, 31));
		aktualneA = aktualneA + 20;
		aktualneB = aktualneB + 20;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
	}
}

void posunSipkyHore(){
	if (aktualneA == 0){
		lcdPlnyTrojuholnik(5, defaultA, defaultB, decodeRgbValue(31, 31, 31));
		aktualneA = 92;
		aktualneB = 107;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
	}
	else if (aktualneB <= 30){
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(31, 31, 31));
		aktualneA = 92;
		aktualneB = 107;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
	}
	else{
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(31, 31, 31));
		aktualneA = aktualneA - 20;
		aktualneB = aktualneB - 20;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
	}

}
void initMenu(){
	defaultA = 12;
	defaultB = 27;
	aktualneA = 0;
	aktualneB = 0;

	lcdInitialise(LCD_ORIENTATION2);
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	  lcdPutS("Info", 20, 17, 0x0000, 0xFFFF); 		//aktualneA 12, aktualneB 27
	  lcdPutS("Revers", 20, 37, 0x0000, 0xFFFF); 	//aktualneA 32, aktualneB 47
	  lcdPutS("Expo", 20, 57, 0x0000, 0xFFFF); 		//aktualneA 52, aktualneB 67
	  lcdPutS("Mix", 20, 77, 0x0000, 0xFFFF); 		//aktualneA 72, aktualneB 87
	  lcdPutS("EPA", 20, 97, 0x0000, 0xFFFF); 		//aktualneA 92, aktualneB 107
	lcdPlnyTrojuholnik(5, defaultA, defaultB, decodeRgbValue(0, 0, 0));
}

void initGPIO(){			// inicalizujem GPIO piny

  	GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
  		/////
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
  	RCC_HSICmd(ENABLE);
  	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

}

void adc_init(void)			// inicalizujem ADC
  {
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  	//RCC_HSICmd(ENABLE);
  	ADC_InitTypeDef ADC_InitStructure;
  	//while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  	ADC_StructInit(&ADC_InitStructure);
  	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  	ADC_InitStructure.ADC_NbrOfConversion = 1;
  	ADC_Init(ADC1, &ADC_InitStructure);
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_16Cycles);
  	ADC_Cmd(ADC1, ENABLE);
  	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
  	/*ADC_ITConfig(ADC1,ADC_IT_OVR,ENABLE);
	ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC);
	ADC_GetFlagStatus(ADC1, ADC_FLAG_OVR);*/
  	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET){}
  	ADC_SoftwareStartConv(ADC1);
  }

void nvic_init(){			// inicializacia prerusenia pre ADC a USART2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
}

