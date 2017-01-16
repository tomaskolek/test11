/*
 * submenu.c
 *
 *  Created on: 3. 12. 2016
 *      Author: ja
 */

#include <funkcie.h>
#include "spi.h"
#include "ili9163.h"
#include <stddef.h>
#include "stm32l1xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float plynMIN=883, plynMAX=3135, vyskovkaMIN=568, vyskovkaMAX=3705, smerovkaMIN=487, smerovkaMAX=3128, kridielkoMIN=496, kridielkoMAX=3408;
float plynNORM, plynNORMdiff, NORM, NORMdiff;
int plynNORMint, plynNORMint2, NORMint, NORMint2;
float kridielkoNORM, vyskovkaNORM, smerovkaNORM, plynNORM, MIX, trim=0;

float normalizuj(float hodnota,float hodnotaMIN,float hodnotaMAX){ //normalizovanie hodnot od -1 do 1
	NORM =(((hodnota-hodnotaMIN)/(hodnotaMAX-hodnotaMIN))*(1-(-1)))+(-1);
	return NORM;
}

char *FloatToString(float NORM){
	NORMint=(int)NORM;				// zaokruhli vstupne cislo
	NORMdiff=NORM-(float)NORMint;	// cislo za desatinnou ciarkou
	NORMint2=(int)(NORMdiff*100);	// cislo za desatiinnou ciarkou vynasobene 100 (posunute o 2 miesta vlavo)
	if (NORM < 0){	// ak je cislo zaporne prida minusy
		static char str[6];
		if(NORMint2 < 10) sprintf(str,"-%d.0%d", NORMint, abs(NORMint2));	// ak je cislo za desatinnou ciarkou mensie ako .10 tak prida 0
		else sprintf(str,"-%d.%d", NORMint, abs(NORMint2));
			return str;
		}
	else{
		static char str[5];
		if(NORMint2 < 10) sprintf(str,"%d.0%d", NORMint, abs(NORMint2));
		else sprintf(str,"%d.%d", NORMint, NORMint2);
			return str;
		}

}

char *FloatToStringReverz(float NORM){	//to iste ako floattostring, opacne znamienka
	NORMint=(int)NORM;
	NORMdiff=NORM-(float)NORMint;
	NORMint2=(int)(NORMdiff*100);
	if (NORM < 0){
		static char str[6];
		if(NORMint2 < 10) sprintf(str,"%d.%d", NORMint, abs(NORMint2));
		else sprintf(str,"%d.%d", NORMint, abs(NORMint2));
			return str;
		}
	else{
		static char str[5];
		if(NORMint2 < 10) sprintf(str,"-%d.0%d", NORMint, abs(NORMint2));
		else sprintf(str,"-%d.%d", NORMint, NORMint2);
			return str;
		}

}

void otvorInfo(){	//funkcia, ktora po vyvolani zobrazi jednotlive kanaly, hodnoty su normalizovane, rozsah -1 az 1
	subMenu = 1;
	char str[5];
	lcdClearDisplay(decodeRgbValue(255, 255, 255));			//"vycisti" display
	while(subMenu==1){
		if((klavesnica >= 3200) && (klavesnica <= 3440)){	//-
				break;
			}
		else{
		lcdPutS("HODNOTY KANALOV",23, 17, 0x0000, 0xFFFF);	//zobrazi na LCD

		kridielkoNORM = normalizuj((float)kridielko,kridielkoMIN,kridielkoMAX);	//normalizovanie hodnoty z analogu
		sprintf(str,"kridielko: %s", FloatToString(kridielkoNORM));	//prevod cisla na char
		lcdPutS(str, 23, 37, 0x0000, 0xFFFF);	//vypis na displej

		vyskovkaNORM = normalizuj((float)vyskovka,vyskovkaMIN,vyskovkaMAX);
		sprintf(str,"Vyskovka: %s", FloatToString(vyskovkaNORM));
		lcdPutS(str, 23, 47, 0x0000, 0xFFFF);

		plynNORM = normalizuj((float)plyn,plynMIN,plynMAX);
		sprintf(str,"Plyn: %s", FloatToString(plynNORM));
		lcdPutS(str, 23, 57, 0x0000, 0xFFFF);

		smerovkaNORM = normalizuj((float)smerovka,smerovkaMIN,smerovkaMAX);
		sprintf(str,"Smerovka: %s", FloatToString(smerovkaNORM));
		lcdPutS(str, 23, 67, 0x0000, 0xFFFF);
		}
	}
	return;
}

void otvorRevers(){	//funkcia na revers hodnot analogov
	subMenu = 1;
	char str[5];
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	while(subMenu==1){
		if((klavesnica >= 3200) && (klavesnica <= 3440)){	//-
				break;
			}
		else{
	lcdPutS("REVERS",23, 17, 0x0000, 0xFFFF);

	kridielkoNORM = normalizuj((float)kridielko,kridielkoMIN,kridielkoMAX);
	sprintf(str,"kridielko: %s", FloatToStringReverz(kridielkoNORM));
	lcdPutS(str, 23, 37, 0x0000, 0xFFFF);

	vyskovkaNORM = normalizuj((float)vyskovka,vyskovkaMIN,vyskovkaMAX);
	sprintf(str,"Vyskovka: %s", FloatToStringReverz(vyskovkaNORM));
	lcdPutS(str, 23, 47, 0x0000, 0xFFFF);

	plynNORM = normalizuj((float)plyn,plynMIN,plynMAX);
	sprintf(str,"Plyn: %s", FloatToStringReverz(plynNORM));
	lcdPutS(str, 23, 57, 0x0000, 0xFFFF);

	smerovkaNORM = normalizuj((float)smerovka,smerovkaMIN,smerovkaMAX);
	sprintf(str,"Smerovka: %s", FloatToStringReverz(smerovkaNORM));
	lcdPutS(str, 23, 67, 0x0000, 0xFFFF);
		}
	}
}

void otvorExpo(){
	subMenu = 1;
	char str[5];
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	while(subMenu==1){
		if((klavesnica >= 3200) && (klavesnica <= 3440)){	//-
				break;
			}
		else{
	lcdPutS("Aktivne EXPO",23, 17, 0x0000, 0xFFFF);

	kridielkoNORM = normalizuj((float)kridielko,kridielkoMIN,kridielkoMAX);
	if(kridielkoNORM<0){
		kridielkoNORM = exp2(kridielkoNORM*(-1))-1;		//vypocet exponencialnej funkcii 2^x; x je ciselny vstup do funkcie
		kridielkoNORM = kridielkoNORM*(-1);
	}
	else{kridielkoNORM = exp2(kridielkoNORM)-1;}
	sprintf(str,"kridielko: %s", FloatToString(kridielkoNORM));
	lcdPutS(str, 23, 37, 0x0000, 0xFFFF);

	vyskovkaNORM = normalizuj((float)vyskovka,vyskovkaMIN,vyskovkaMAX);
	if(vyskovkaNORM<0){
		vyskovkaNORM = exp2(vyskovkaNORM*(-1))-1;
		vyskovkaNORM = vyskovkaNORM*(-1);
	}
	else{vyskovkaNORM = exp2(vyskovkaNORM)-1;
	kridielkoNORM = kridielkoNORM*(-1);}
	sprintf(str,"Vyskovka: %s", FloatToString(vyskovkaNORM));
	lcdPutS(str, 23, 47, 0x0000, 0xFFFF);

	plynNORM = normalizuj((float)plyn,plynMIN,plynMAX);
	if(plynNORM<0){
		plynNORM = exp2(plynNORM*(-1))-1;
		plynNORM = plynNORM*(-1);
	}
	else{plynNORM = exp2(plynNORM)-1;}
	sprintf(str,"Plyn: %s", FloatToString(plynNORM));
	lcdPutS(str, 23, 57, 0x0000, 0xFFFF);

	smerovkaNORM = normalizuj((float)smerovka,smerovkaMIN,smerovkaMAX);
	if(smerovkaNORM<0){
		smerovkaNORM = exp2(smerovkaNORM*(-1))-1;
		smerovkaNORM = smerovkaNORM*(-1);
	}
	else{smerovkaNORM = exp2(smerovkaNORM)-1;}
	sprintf(str,"Smerovka: %s", FloatToString(smerovkaNORM));
	lcdPutS(str, 23, 67, 0x0000, 0xFFFF);
			}
	}
}

void otvorMix(){	//funkcia na mixovanie kanalov, vahy: vyskovka 50%, klapky 50%
	subMenu = 1;
	float vahaVyskovky = 0.5;
	float vahaKlapky = 0.5;
	char str[5];
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	while(subMenu==1){
		if((klavesnica >= 3200) && (klavesnica <= 3440)){	//-
				break;
			}
		else{
	lcdPutS("MIX 50%",23, 17, 0x0000, 0xFFFF);

	kridielkoNORM = normalizuj((float)kridielko,kridielkoMIN,kridielkoMAX);
	vyskovkaNORM = normalizuj((float)vyskovka,vyskovkaMIN,vyskovkaMAX);

	if(vyskovkaNORM>0){
		if(kridielkoNORM>0){
			MIX = kridielkoNORM*vahaKlapky + vyskovkaNORM*vahaVyskovky;		//vypocet mixov
			sprintf(str,"Servo: %s", FloatToString(MIX));
			lcdPutS(str, 23, 37, 0x0000, 0xFFFF);
		}
		else{
		MIX = vyskovkaNORM*vahaVyskovky + kridielkoNORM*vahaKlapky;
		sprintf(str,"Servo: %s", FloatToString(MIX));
		lcdPutS(str, 23, 37, 0x0000, 0xFFFF);
		}
	}
	else{
		if(kridielkoNORM>0){
			MIX = kridielkoNORM*vahaKlapky + vyskovkaNORM*vahaVyskovky;
			sprintf(str,"Servo: %s", FloatToString(MIX));
			lcdPutS(str, 23, 37, 0x0000, 0xFFFF);
		}
		else{
		MIX = kridielkoNORM*vahaKlapky + vyskovkaNORM*vahaVyskovky;
		sprintf(str,"Servo: %s", FloatToString(MIX));
		lcdPutS(str, 23, 37, 0x0000, 0xFFFF);
		}
	}
	plynNORM = normalizuj((float)plyn,plynMIN,plynMAX);
	sprintf(str,"kridielko: %s", FloatToString(kridielkoNORM));
	lcdPutS(str, 23, 47, 0x0000, 0xFFFF);
	sprintf(str,"Vyskovka: %s", FloatToString(vyskovkaNORM));
	lcdPutS(str, 23, 57, 0x0000, 0xFFFF);
	sprintf(str,"Plyn: %s", FloatToString(plynNORM));
	lcdPutS(str, 23, 67, 0x0000, 0xFFFF);
			}
	}
}

void otvorEPA(){
	subMenu = 1;
	char str[5];
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	while(subMenu==1){
		if((klavesnica >= 3200) && (klavesnica <= 3440)){	//-
				break;
			}
		else{
	lcdPutS("TRIMOVANIE",23, 17, 0x0000, 0xFFFF);	//zobrazi na LCD

	if((klavesnica >= 1800) && (klavesnica <= 2100)){	//hore
		trim += 0.01;
	}
	if((klavesnica >= 3450) && (klavesnica <= 3600)){	//dole
		trim -= 0.01;
	}

	plynNORM = normalizuj((float)plyn,plynMIN,plynMAX) + trim;
	sprintf(str,"Plyn: %s", FloatToString(plynNORM));
	lcdPutS(str, 23, 57, 0x0000, 0xFFFF);
	sprintf(str,"Trim: %s", FloatToString(trim));
	lcdPutS(str, 23, 67, 0x0000, 0xFFFF);
			}
	}
}

char * prevodNaChar(uint16_t hodnota){	//funkcia prevadza cislo na char, vracia pole znakov
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
