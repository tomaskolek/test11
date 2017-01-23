/*
 * test1.c
 *
 *  Created on: 2. 12. 2016
 *      Author: ja
 */

#include <funkcie.h>
#include "spi.h"
#include "ili9163.h"
#include <stddef.h>
#include "stm32l1xx.h"

void Delay(uint16_t n)  //funkcia pre spomalenie
{
	uint32_t nl = n*2;
	while(nl--);
	return;
}

void pohybMenu(uint16_t klavesnica){  //funkcia, ktora sleduje zvolene polozky v menu uzivatelom
	if (subMenu == 0){  //ochrana aby sa nedalo pohybovat v menu ked uz je otvorene niektore submenu
		if ((klavesnica >= 1800) && (klavesnica <= 2100)){ //posun v menu smerom nahor sipkou na klavesnici
			posunSipkyHore(); //posuvanie sipky (trojuholniku) na displeji smerom hore
		}
		else if ((klavesnica >= 2600) && (klavesnica <= 3000)){  //vstup do submenu
			if ((aktualneA == 0 && defaultA == 12) || aktualneA == 12){
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
		else if ((klavesnica >= 3450) && (klavesnica <= 3600)){
			posunSipkyDole(); //posuvanie sipky (trojuholniku) na displeji smerom dole
		}
	}
		if ((klavesnica >= 3200) && (klavesnica <= 3440)){  	//opustenie submenu a navrat do hlavneho menu (klavesa minus)
			if (subMenu == 1){
				lcdClearDisplay(decodeRgbValue(255, 255, 255));
				lcdPutS("Info", 20, 17, 0x0000, 0xFFFF); 		//aktualneA 12, aktualneB 27
				lcdPutS("Revers", 20, 37, 0x0000, 0xFFFF); 		//aktualneA 32, aktualneB 47
				lcdPutS("Expo", 20, 57, 0x0000, 0xFFFF); 		//aktualneA 52, aktualneB 67
				lcdPutS("Mix", 20, 77, 0x0000, 0xFFFF); 		//aktualneA 72, aktualneB 87
				lcdPutS("TRIM", 20, 97, 0x0000, 0xFFFF); 		//aktualneA 92, aktualneB 107
				lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
				subMenu = 0;
			}
		 }
}

void posunSipkyDole(){		//posuvanie trojuholniku po displeju smerom dole, aktivne len pri stlaceni sipky dole na klavesnici
	if (aktualneA == 0){	//ak je sipka v povodnej polohe
		lcdPlnyTrojuholnik(5, defaultA, defaultB, decodeRgbValue(31, 31, 31)); //funkcia, ktora vymaze trojuholnik
		aktualneA = defaultA + 20;
		aktualneB = defaultB + 20;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));  //nakresli sa rovnoramenny trojuholnik na novu poziciu
	}
	else if (aktualneB >= 100){  //ak je trojuholnik na konci displeja tak sa vykresli hore k prvej polozke v menu
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(31, 31, 31));	//funkcia, ktora vymaze trojuholnik
		lcdPlnyTrojuholnik(5, defaultA, defaultB, decodeRgbValue(0, 0, 0)); 		//nakresli sa rovnoramenny trojuholnik na novu poziciu
		aktualneA = defaultA;
		aktualneB = defaultB;
	}
	else{
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(31, 31, 31));//funkcia, ktora vymaze trojuholnik
		aktualneA = aktualneA + 20;
		aktualneB = aktualneB + 20;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));	//nakresli sa rovnoramenny trojuholnik na novu poziciu
	}
}

void posunSipkyDole2(){  //posuvanie trojuholniku po displeju smerom dole, aktivne len pri stlaceni sipky dole na klavesnici
	if (aktualneA == 0){ //ak je sipka v povodnej polohe
		lcdPlnyTrojuholnik(5, 37, 47, decodeRgbValue(31, 31, 31)); //funkcia, ktora vymaze trojuholnik
		aktualneA = 37 + 10;
		aktualneB = 45 + 10;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));  //nakresli sa rovnoramenny trojuholnik na novu poziciu
	}
	else if (aktualneB >= 70){  //ak je trojuholnik na konci displeja tak sa vykresli hore k prvej polozke v menu
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(31, 31, 31));  //funkcia, ktora vymaze trojuholnik
		lcdPlnyTrojuholnik(5, 37, 45, decodeRgbValue(0, 0, 0)); //nakresli sa rovnoramenny trojuholnik na novu poziciu
		aktualneA = 37;
		aktualneB = 45;
	}
	else{
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(31, 31, 31));  //funkcia, ktora vymaze trojuholnik
		aktualneA = aktualneA + 10;
		aktualneB = aktualneB + 10;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));  //nakresli sa rovnoramenny trojuholnik na novu poziciu
	}
}

void posunSipkyHore(){	//posuvanie trojuholniku po displeju smerom hore, aktivne len pri stlaceni sipky hore na klavesnici
	if (aktualneA == 0){
		lcdPlnyTrojuholnik(5, defaultA, defaultB, decodeRgbValue(31, 31, 31));	//funkcia, ktora vymaze trojuholnik
		aktualneA = 92;
		aktualneB = 107;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));	//nakresli sa rovnoramenny trojuholnik na novu poziciu
	}
	else if (aktualneB <= 30){	//ak sa uzivatel pohne sipkou smerom nahor, tak sa sipka nakresli k poslednej polozke v menu
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(31, 31, 31));//funkcia, ktora vymaze trojuholnik
		aktualneA = 92;
		aktualneB = 107;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));	//nakresli sa rovnoramenny trojuholnik na novu poziciu
	}
	else{
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(31, 31, 31));
		aktualneA = aktualneA - 20;
		aktualneB = aktualneB - 20;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
	}

}

void posunSipkyHore2(){   //posuvanie trojuholniku po displeju smerom hore, aktivne len pri stlaceni sipky hore na klavesnici
	if (aktualneA == 37){
		lcdPlnyTrojuholnik(5, 37, 45, decodeRgbValue(31, 31, 31)); //funkcia, ktora vymaze trojuholnik
		aktualneA = 67;
		aktualneB = 75;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));   //nakresli sa rovnoramenny trojuholnik na novu poziciu
	}
	else if (aktualneB <= 30){ //ak sa uzivatel pohne sipkou smerom nahor, tak sa sipka nakresli k poslednej polozke v menu
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(31, 31, 31));  //funkcia, ktora vymaze trojuholnik
		aktualneA = 67;
		aktualneB = 75;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));   //nakresli sa rovnoramenny trojuholnik na novu poziciu
	}
	else{
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(31, 31, 31));
		aktualneA = aktualneA - 10;
		aktualneB = aktualneB - 10;
		lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
	}

}
void initMenu(){	//zobrazi sa hlavne menu a vypisu sa polozky
	defaultA = 12;
	defaultB = 27;
	aktualneA = 0;
	aktualneB = 0;
	subMenu = 0;
	zapnutyMix = 0;
	lcdInitialise(LCD_ORIENTATION2);	//menim orientaciu displeja
	lcdClearDisplay(decodeRgbValue(255, 255, 255)); 	//mazem obrazovku a nastavim biele pozadie
	  lcdPutS("Info", 20, 17, 0x0000, 0xFFFF);			//aktualneA 12, aktualneB 27
	  lcdPutS("Revers", 20, 37, 0x0000, 0xFFFF);		//aktualneA 32, aktualneB 47
	  lcdPutS("Expo", 20, 57, 0x0000, 0xFFFF);			//aktualneA 52, aktualneB 67
	  lcdPutS("Mix", 20, 77, 0x0000, 0xFFFF);			//aktualneA 72, aktualneB 87
	  lcdPutS("TRIM", 20, 97, 0x0000, 0xFFFF);			//aktualneA 92, aktualneB 107
	lcdPlnyTrojuholnik(5, defaultA, defaultB, decodeRgbValue(0, 0, 0));	//vykreslim sipku v prvej polozke
}

void initGPIO(){	// inicalizujem GPIO piny

  	GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
  		/////
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4;// PA0, PA1, PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//The pins are configured in analog mode
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//We don't need any pull up or pull down
	GPIO_Init(GPIOA, &GPIO_InitStructure);//Initialize GPIOC pins with the configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//The pins are configured in analog mode
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//We don't need any pull up or pull down
	GPIO_Init(GPIOB, &GPIO_InitStructure);//Initialize GPIOA pins with the configuration

  	RCC_HSICmd(ENABLE);
  	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

}

void dma_init(){  	//inicializujem DMA1
	DMA_InitTypeDef       DMA_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* DMA1 Stream0 channel0 configuration **************************************/
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//ADC1's data register
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&ADC1ConvertedValue[0];
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize = 5;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//Reads 16 bit values
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//Stores 16 bit values
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);
	DMA_Cmd(DMA1_Channel1, ENABLE); //Enable the DMA1 - Channel1
}

void adc_init(void)		// inicalizujem ADC
  {
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  	//RCC_HSICmd(ENABLE);
  	ADC_InitTypeDef ADC_InitStructure;
  	//while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  	ADC_StructInit(&ADC_InitStructure);
  	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//The scan is configured in multiple channels
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  	ADC_InitStructure.ADC_NbrOfConversion = 5;
  	ADC_InitStructure.ADC_ExternalTrigConv = 0;
  	ADC_Init(ADC1, &ADC_InitStructure);
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_9Cycles);//PC0
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_9Cycles);
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_9Cycles);
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 4, ADC_SampleTime_9Cycles);
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 5, ADC_SampleTime_9Cycles);
  	ADC_Cmd(ADC1, ENABLE);
  	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
  	/* Enable DMA request after last transfer (Single-ADC mode) */
  	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
  	/*ADC_ITConfig(ADC1,ADC_IT_OVR,ENABLE);
	ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC);
	ADC_GetFlagStatus(ADC1, ADC_FLAG_OVR);*/
  	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET){}
  	ADC_SoftwareStartConv(ADC1);
  }

void nvic_init(){		// inicializacia prerusenia pre ADC
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitTypeDef NVIC_InitStructure;
	//NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);

}

float mixujem(float kridielkoNORM, float vyskovkaNORM){
	float MIX;
	if(vyskovkaNORM>0){
		if(kridielkoNORM>0){
			MIX = kridielkoNORM*vahaKlapky + vyskovkaNORM*vahaVyskovky;		//vypocet mixov
		}
		else{
			MIX = vyskovkaNORM*vahaVyskovky + kridielkoNORM*vahaKlapky;
		}
	}
	else{
		if(kridielkoNORM>0){
			MIX = kridielkoNORM*vahaKlapky + vyskovkaNORM*vahaVyskovky;
		}
		else{
			MIX = kridielkoNORM*vahaKlapky + vyskovkaNORM*vahaVyskovky;
		}
	}
	return MIX;
}


