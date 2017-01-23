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
#include <stdbool.h>

float plynMIN=883, plynMAX=3135, vyskovkaMIN=568, vyskovkaMAX=3705, smerovkaMIN=487, smerovkaMAX=3128, kridielkoMIN=496, kridielkoMAX=3408;
float plynNORM, plynNORMdiff, NORM, NORMdiff;
int plynNORMint, plynNORMint2, NORMint, NORMint2;
float kridielkoNORM, vyskovkaNORM, smerovkaNORM, plynNORM, MIX, hodnotaMIX, trim=0;
float kridielkoTRIM = 0, vyskovkaTRIM = 0, smerovkaTRIM = 0, plynTRIM = 0, hodnotaTRIM = 0;
bool plynREV = false, vyskovkaREV = false, smerovkaREV = false, kridielkoREV = false;
bool plynEXP = false, vyskovkaEXP = false, smerovkaEXP = false, kridielkoEXP = false;
float vahaVyskovky, vahaKlapky;

float normalizuj(float hodnota,float hodnotaMIN,float hodnotaMAX,bool hodnotaREV, bool hodnotaEXP, float hodnotaTRIM){ //normalizovanie hodnot od -1 do 1
	NORM =(((hodnota-hodnotaMIN)/(hodnotaMAX-hodnotaMIN))*(1-(-1)))+(-1);
	if (hodnotaREV == true){
		NORM = NORM*(-1);
	}
	if (hodnotaEXP == true){
				if(NORM<0){
					NORM = exp2(NORM*(-1))-1;
					NORM = NORM*(-1);
				}
				else{
					NORM = exp2(NORM)-1;
					NORM = NORM*(-1);
				}
	}
	NORM = NORM + hodnotaTRIM;
	return NORM;
}

char *ReverzOutput(bool ReverzValue){
	static char str[6];
	if (ReverzValue == true)
			sprintf(str,"ON ");
	else sprintf(str,"OFF");
	return str;
}

int FloatToInt(float NORM){
	NORMint=(int)NORM;				// zaokruhli vstupne cislo
	NORMint = abs(NORMint);
	return NORMint;
}

int FloatToInt2(float NORM){
	NORMint=(int)NORM;				// zaokruhli vstupne cislo
	NORMdiff=NORM-(float)NORMint;	// cislo za desatinnou ciarkou
	NORMint2=(int)(NORMdiff*100);	// cislo za desatiinnou ciarkou vynasobene 100 (posunute o 2 miesta vlavo)
	NORMint2 = abs(NORMint2);
	return NORMint2;
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
		if(NORMint2 < 10) sprintf(str,"+%d.0%d", NORMint, abs(NORMint2));
		else sprintf(str,"+%d.%d", NORMint, NORMint2);
			return str;
		}

}

void otvorInfo(){	//funkcia, ktora po vyvolani zobrazi jednotlive kanaly, hodnoty su normalizovane, rozsah -1 az 1
	subMenu = 1;
	char str[16];
	lcdClearDisplay(decodeRgbValue(255, 255, 255));			//"vycisti" display
	while(subMenu==1){
			lcdPutS("HODNOTY KANALOV",23, 17, 0x0000, 0xFFFF);	//zobrazi na LCD

			kridielkoNORM = normalizuj((float)kridielko,kridielkoMIN,kridielkoMAX,kridielkoREV,kridielkoEXP,kridielkoTRIM);	//normalizovanie hodnoty z analogu
			if (kridielkoNORM < 0){	// ak je cislo zaporne prida minusy
				if((kridielkoNORM > -0.10) || ((kridielkoNORM < -1.00) && (kridielkoNORM > -1.10))){
					sprintf(str,"kridielko: -%d.0%d", FloatToInt(kridielkoNORM), FloatToInt2(kridielkoNORM));
				}
				else {
					sprintf(str,"kridielko: -%d.%d", FloatToInt(kridielkoNORM), FloatToInt2(kridielkoNORM));
				}
			}
			else{
				if ((kridielkoNORM < 0.10) || ((kridielkoNORM > 1.00) && (kridielkoNORM < 1.10))){
					sprintf(str, "kridielko: +%d.0%d", FloatToInt(kridielkoNORM),FloatToInt2(kridielkoNORM));
				}
				else {
					sprintf(str,"kridielko: +%d.%d", FloatToInt(kridielkoNORM), FloatToInt2(kridielkoNORM));
				}
			}
			lcdPutS(str, 23, 37, 0x0000, 0xFFFF);	//vypis na displej

			vyskovkaNORM = normalizuj((float)vyskovka,vyskovkaMIN,vyskovkaMAX,vyskovkaREV,vyskovkaEXP,vyskovkaTRIM);
			if (vyskovkaNORM < 0) {	// ak je cislo zaporne prida minusy
				if ((vyskovkaNORM > -0.10)	|| ((vyskovkaNORM < -1.00) && (vyskovkaNORM > -1.10))) {
					sprintf(str, "vyskovka: -%d.0%d", FloatToInt(vyskovkaNORM),FloatToInt2(vyskovkaNORM));
				}
				else {
					sprintf(str, "vyskovka: -%d.%d", FloatToInt(vyskovkaNORM),FloatToInt2(vyskovkaNORM));
				}
			}
			else {
				if ((vyskovkaNORM < 0.10) || ((vyskovkaNORM > 1.00) && (vyskovkaNORM < 1.10))) {
					sprintf(str, "vyskovka: +%d.0%d", FloatToInt(vyskovkaNORM),FloatToInt2(vyskovkaNORM));
				}
				else {
					sprintf(str, "vyskovka: +%d.%d", FloatToInt(vyskovkaNORM),FloatToInt2(vyskovkaNORM));
				}
			}
			lcdPutS(str, 23, 47, 0x0000, 0xFFFF);

			plynNORM = normalizuj((float)plyn,plynMIN,plynMAX,plynREV,plynEXP,plynTRIM);
			if (plynNORM < 0) {	// ak je cislo zaporne prida minusy
				if ((plynNORM > -0.10)	|| ((plynNORM < -1.00) && (plynNORM > -1.10))) {
					sprintf(str, "plyn: -%d.0%d", FloatToInt(plynNORM),FloatToInt2(plynNORM));
				}
				else {
					sprintf(str, "plyn: -%d.%d", FloatToInt(plynNORM),FloatToInt2(plynNORM));
				}
			}
			else {
				if ((plynNORM < 0.10) || ((plynNORM > 1.00) && (plynNORM < 1.10))) {
					sprintf(str, "plyn: +%d.0%d ", FloatToInt(plynNORM),FloatToInt2(plynNORM));
				}
				else {
					sprintf(str, "plyn: +%d.%d", FloatToInt(plynNORM),FloatToInt2(plynNORM));
				}
			}
			lcdPutS(str, 23, 57, 0x0000, 0xFFFF);

			smerovkaNORM = normalizuj((float)smerovka,smerovkaMIN,smerovkaMAX,smerovkaREV,smerovkaEXP,smerovkaTRIM);
			if (smerovkaNORM < 0) {	// ak je cislo zaporne prida minusy
				if ((smerovkaNORM > -0.10)	|| ((smerovkaNORM < -1.00) && (smerovkaNORM > -1.10))) {
					sprintf(str, "smerovka: -%d.0%d", FloatToInt(smerovkaNORM),FloatToInt2(smerovkaNORM));
				}
				else {
					sprintf(str, "smerovka: -%d.%d", FloatToInt(smerovkaNORM),FloatToInt2(smerovkaNORM));
				}
			}
			else {
				if ((smerovkaNORM < 0.10) || ((smerovkaNORM > 1.00) && (smerovkaNORM < 1.10))) {
					sprintf(str, "smerovka: +%d.0%d", FloatToInt(smerovkaNORM),FloatToInt2(smerovkaNORM));
				}
				else {
					sprintf(str, "smerovka: +%d.%d", FloatToInt(smerovkaNORM),FloatToInt2(smerovkaNORM));
				}
			}
			lcdPutS(str, 23, 67, 0x0000, 0xFFFF);

			if (zapnutyMix == 1){
				sprintf(str,"Mix v. %d%% k. %d%%", (int)(vahaVyskovky*100), (int)(vahaKlapky*100)); //nechce vypisovat integer
				lcdPutS(str, 23, 80, 0x0000, 0xFFFF);
				hodnotaMIX = mixujem(kridielkoNORM, vyskovkaNORM);
				if (hodnotaMIX < 0) {	// ak je cislo zaporne prida minusy
					if ((hodnotaMIX > -0.10)	|| ((hodnotaMIX < -1.00) && (hodnotaMIX > -1.10))) {
						sprintf(str, "Mix: -%d.0%d", FloatToInt(hodnotaMIX),FloatToInt2(hodnotaMIX));
					}
					else {
						sprintf(str, "Mix: -%d.%d", FloatToInt(hodnotaMIX),FloatToInt2(hodnotaMIX));
					}
				}
				else {
					if ((hodnotaMIX < 0.10) || ((hodnotaMIX > 1.00) && (hodnotaMIX < 1.10))) {
						sprintf(str, "Mix: +%d.0%d", FloatToInt(hodnotaMIX),FloatToInt2(hodnotaMIX));
					}
					else {
						sprintf(str, "Mix: +%d.%d", FloatToInt(hodnotaMIX),FloatToInt2(hodnotaMIX));
					}
				}
				lcdPutS(str, 23, 90, 0x0000, 0xFFFF);
			}
			if ((klavesnica >= 3200) && (klavesnica <= 3440)) { //opustenie submenu a navrat do hlavneho menu (klavesa minus)
				lcdClearDisplay(decodeRgbValue(255, 255, 255));
				lcdPutS("Info", 20, 17, 0x0000, 0xFFFF); //aktualneA 12, aktualneB 27
				lcdPutS("Revers", 20, 37, 0x0000, 0xFFFF); //aktualneA 32, aktualneB 47
				lcdPutS("Expo", 20, 57, 0x0000, 0xFFFF); //aktualneA 52, aktualneB 67
				lcdPutS("Mix", 20, 77, 0x0000, 0xFFFF); //aktualneA 72, aktualneB 87
				lcdPutS("TRIM", 20, 97, 0x0000, 0xFFFF); //aktualneA 92, aktualneB 107
				lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
				subMenu = 0;
			}
//		}
	}
	return;
}

void otvorRevers(){	//funkcia na revers hodnot analogov
	aktualneA = 37;
	aktualneB = 45;
	subMenu = 1;
	char str[5];

	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));	//vykreslim sipku v prvej polozke
	while(subMenu==1){
		if((klavesnica >= 3200) && (klavesnica <= 3440)){	//-
			break;
			}
		else{
			lcdPutS("REVERS",23, 17, 0x0000, 0xFFFF);
			if (subMenu == 1){  //ochrana aby sa nedalo pohybovat v menu ked uz je otvorene niektore submenu
					if ((klavesnica >= 1800) && (klavesnica <= 2100)){ //posun v menu smerom nahor sipkou na klavesnici
						posunSipkyHore2(); //posuvanie sipky (trojuholniku) na displeji smerom hore
					}
					else if ((klavesnica >= 2600) && (klavesnica <= 3000)){  //vstup do submenu
						if (aktualneA == 37){
							kridielkoREV ^= 1;
						}
						else if (aktualneA == 47){
							vyskovkaREV ^= 1;
						}
						else if (aktualneA == 57){
							plynREV ^= 1;
						}
						else if (aktualneA == 67){
							smerovkaREV ^= 1;
						}
					}
					else if ((klavesnica >= 3450) && (klavesnica <= 3600)){
						posunSipkyDole2(); //posuvanie sipky (trojuholniku) na displeji smerom dole
					}
				}
					if ((klavesnica >= 3200) && (klavesnica <= 3440)){  //opustenie submenu a navrat do hlavneho menu (klavesa minus)
						lcdClearDisplay(decodeRgbValue(255, 255, 255));
						lcdPutS("Info", 20, 17, 0x0000, 0xFFFF); 		//aktualneA 12, aktualneB 27
						lcdPutS("Revers", 20, 37, 0x0000, 0xFFFF); 	//aktualneA 32, aktualneB 47
						lcdPutS("Expo", 20, 57, 0x0000, 0xFFFF); 		//aktualneA 52, aktualneB 67
						lcdPutS("Mix", 20, 77, 0x0000, 0xFFFF); 		//aktualneA 72, aktualneB 87
						lcdPutS("TRIM", 20, 97, 0x0000, 0xFFFF); 		//aktualneA 92, aktualneB 107
						aktualneA = 0;
						aktualneB = 0;
						lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
						subMenu = 0;
					 }
			sprintf(str,"kridielko: %s", ReverzOutput(kridielkoREV));
			lcdPutS(str, 23, 37, 0x0000, 0xFFFF);
			sprintf(str,"vyskovka: %s", ReverzOutput(vyskovkaREV));
			lcdPutS(str, 23, 47, 0x0000, 0xFFFF);
			sprintf(str,"plyn: %s", ReverzOutput(plynREV));
			lcdPutS(str, 23, 57, 0x0000, 0xFFFF);
			sprintf(str,"smerovka: %s", ReverzOutput(smerovkaREV));
			lcdPutS(str, 23, 67, 0x0000, 0xFFFF);
		}
	}
}

void otvorExpo(){	//funkcia na revers hodnot analogov
	aktualneA = 37;
	aktualneB = 45;
	subMenu = 1;
	char str[5];

	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));	//vykreslim sipku v prvej polozke
	while(subMenu==1){
		if((klavesnica >= 3200) && (klavesnica <= 3440)){	//-
			break;
			}
		else{
			lcdPutS("EXPO",23, 17, 0x0000, 0xFFFF);
			if (subMenu == 1){  //ochrana aby sa nedalo pohybovat v menu ked uz je otvorene niektore submenu
					if ((klavesnica >= 1800) && (klavesnica <= 2100)){ //posun v menu smerom nahor sipkou na klavesnici
						posunSipkyHore2(); //posuvanie sipky (trojuholniku) na displeji smerom hore
					}
					else if ((klavesnica >= 2600) && (klavesnica <= 3000)){  //vstup do submenu
						if (aktualneA == 37){
							kridielkoEXP ^= 1;
						}
						else if (aktualneA == 47){
							vyskovkaEXP ^= 1;
						}
						else if (aktualneA == 57){
							plynEXP ^= 1;
						}
						else if (aktualneA == 67){
							smerovkaEXP ^= 1;
						}
					}
					else if ((klavesnica >= 3450) && (klavesnica <= 3600)){
						posunSipkyDole2(); //posuvanie sipky (trojuholniku) na displeji smerom dole
					}
				}
					if ((klavesnica >= 3200) && (klavesnica <= 3440)){  //opustenie submenu a navrat do hlavneho menu (klavesa minus)
						lcdClearDisplay(decodeRgbValue(255, 255, 255));
						lcdPutS("Info", 20, 17, 0x0000, 0xFFFF); 		//aktualneA 12, aktualneB 27
						lcdPutS("Revers", 20, 37, 0x0000, 0xFFFF); 	//aktualneA 32, aktualneB 47
						lcdPutS("Expo", 20, 57, 0x0000, 0xFFFF); 		//aktualneA 52, aktualneB 67
						lcdPutS("Mix", 20, 77, 0x0000, 0xFFFF); 		//aktualneA 72, aktualneB 87
						lcdPutS("TRIM", 20, 97, 0x0000, 0xFFFF); 		//aktualneA 92, aktualneB 107
						aktualneA = 0;
						aktualneB = 0;
						lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
						subMenu = 0;
					 }
			sprintf(str,"kridielko: %s", ReverzOutput(kridielkoEXP));
			lcdPutS(str, 23, 37, 0x0000, 0xFFFF);
			sprintf(str,"vyskovka: %s", ReverzOutput(vyskovkaEXP));
			lcdPutS(str, 23, 47, 0x0000, 0xFFFF);
			sprintf(str,"plyn: %s", ReverzOutput(plynEXP));
			lcdPutS(str, 23, 57, 0x0000, 0xFFFF);
			sprintf(str,"smerovka: %s", ReverzOutput(smerovkaEXP));
			lcdPutS(str, 23, 67, 0x0000, 0xFFFF);
		}
	}
}

void otvorMix(){	//funkcia na mixovanie kanalov, vahy: vyskovka 50%, klapky 50%
	subMenu = 1;
	//char str[5];
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	aktualneA = defaultA + 20;
	aktualneB = defaultB + 20;
	lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));	//vykreslim sipku v prvej polozke

	while(subMenu == 1){
		if((klavesnica >= 3200) && (klavesnica <= 3440)){	//-
				break;
		}
		else{
			lcdPutS("Vyskovka  Klapky ",20, 17, 0x0000, 0xFFFF);
			lcdPutS("MIX:  50%   50%",20, 37, 0x0000, 0xFFFF);
			lcdPutS("MIX:  25%   75%",20, 57, 0x0000, 0xFFFF);
			lcdPutS("MIX:  75%   25%",20, 77, 0x0000, 0xFFFF);
			lcdPutS("Vypnut Mix",20, 97, 0x0000, 0xFFFF);

			//lcdPlnyTrojuholnik(5, defaultA, defaultB, decodeRgbValue(0, 0, 0));	//vykreslim sipku v prvej polozke
			if ((klavesnica >= 1800) && (klavesnica <= 2100)){ //posun v menu smerom nahor sipkou na klavesnici
						posunSipkyHore(); //posuvanie sipky (trojuholniku) na displeji smerom hore
					}
			else if ((klavesnica >= 3450) && (klavesnica <= 3600)){
				posunSipkyDole(); //posuvanie sipky (trojuholniku) na displeji smerom dole
			}
			else if ((klavesnica >= 2600) && (klavesnica <= 3000)){  //volba vah
				if (aktualneA == 32){
					vahaVyskovky = 0.5;
					vahaKlapky = 0.5;
					zapnutyMix = 1;
					break;
				}
				else if (aktualneA == 52){
					vahaVyskovky = 0.25;
					vahaKlapky = 0.75;
					zapnutyMix = 1;
					break;
				}
				else if (aktualneA == 72){
					vahaVyskovky = 0.75;
					vahaKlapky = 0.25;
					zapnutyMix = 1;
					break;
				}
				else if (aktualneA == 92){
					vahaVyskovky = 1;
					vahaKlapky = 1;
					zapnutyMix = 0;
					break;
				}
			}
		}
	}
}

void otvorEPA(){
	aktualneA = 37;
	aktualneB = 45;
	subMenu = 1;
	char str[5];
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));	//vykreslim sipku v prvej polozke
	while(subMenu==1){
		if((klavesnica >= 3200) && (klavesnica <= 3440)){	//-
			break;
			}
		else{
			lcdPutS("TRIM",23, 17, 0x0000, 0xFFFF);
			if (subMenu == 1){  //ochrana aby sa nedalo pohybovat v menu ked uz je otvorene niektore submenu
					if ((klavesnica >= 1800) && (klavesnica <= 2100)){ //posun v menu smerom nahor sipkou na klavesnici
						posunSipkyHore2(); //posuvanie sipky (trojuholniku) na displeji smerom hore
					}
					else if ((klavesnica >= 2600) && (klavesnica <= 3000)){  //vstup do submenu
						if (aktualneA == 37){
							kridielkoTRIM = TRIMfunc(kridielkoTRIM,"kridielko");
						}
						else if (aktualneA == 47){
							vyskovkaTRIM = TRIMfunc(vyskovkaTRIM,"vyskovka");
						}
						else if (aktualneA == 57){
							plynTRIM = TRIMfunc(plynTRIM,"plyn");
						}
						else if (aktualneA == 67){
							smerovkaTRIM = TRIMfunc(smerovkaTRIM,"smerovka");
						}
					}
					else if ((klavesnica >= 3450) && (klavesnica <= 3600)){
						posunSipkyDole2(); //posuvanie sipky (trojuholniku) na displeji smerom dole
					}
				}
					if ((klavesnica >= 3200) && (klavesnica <= 3440)){  //opustenie submenu a navrat do hlavneho menu (klavesa minus)
						lcdClearDisplay(decodeRgbValue(255, 255, 255));
						lcdPutS("Info", 20, 17, 0x0000, 0xFFFF); 		//aktualneA 12, aktualneB 27
						lcdPutS("Revers", 20, 37, 0x0000, 0xFFFF); 	//aktualneA 32, aktualneB 47
						lcdPutS("Expo", 20, 57, 0x0000, 0xFFFF); 		//aktualneA 52, aktualneB 67
						lcdPutS("Mix", 20, 77, 0x0000, 0xFFFF); 		//aktualneA 72, aktualneB 87
						lcdPutS("TRIM", 20, 97, 0x0000, 0xFFFF); 		//aktualneA 92, aktualneB 107
						aktualneA = 0;
						aktualneB = 0;
						lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
						subMenu = 0;
					 }
			sprintf(str,"kridielko->");
			lcdPutS(str, 23, 37, 0x0000, 0xFFFF);
			sprintf(str,"vyskovka->");
			lcdPutS(str, 23, 47, 0x0000, 0xFFFF);
			sprintf(str,"plyn->");
			lcdPutS(str, 23, 57, 0x0000, 0xFFFF);
			sprintf(str,"smerovka->");
			lcdPutS(str, 23, 67, 0x0000, 0xFFFF);
		}
	}
}

float TRIMfunc(float trim, char *channelName){
	char str[6];
	subMenu = 2;
	lcdClearDisplay(decodeRgbValue(255, 255, 255));
	while(subMenu == 2){
		lcdPutS("TRIMOVANIE",23, 17, 0x0000, 0xFFFF);	//zobrazi na LCD
		if((klavesnica >= 1800) && (klavesnica <= 2100)){	//hore
			trim += 0.01;
		}
		if((klavesnica >= 3450) && (klavesnica <= 3600)){	//dole
			trim -= 0.01;
		}
		sprintf(str,"trim %s", FloatToString(trim));
		lcdPutS(str, 23, 67, 0x0000, 0xFFFF);
		if ((klavesnica >= 3200) && (klavesnica <= 3440)) { //opustenie submenu a navrat do hlavneho menu (klavesa minus)
			lcdClearDisplay(decodeRgbValue(255, 255, 255));
			lcdPlnyTrojuholnik(5, aktualneA, aktualneB, decodeRgbValue(0, 0, 0));
			subMenu = 1;
		}
	}
	return trim;
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
