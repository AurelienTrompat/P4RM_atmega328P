/* 
* PRobot.h
*
* Created: 02/12/2016 03:49:15
* Author: Aurelien
*/


#ifndef __PROBOT_H__
#define __PROBOT_H__
#define F_CPU 16000000

#include<avr/io.h>
#include <util/delay.h>

#include "Pmoteur.hpp"
#include "PSecurite.h"
#include "PI2C.h"
#include "PRouesCodeuses.h"

extern uint8_t vitesseMoteur;
extern bool actualiserVitesse;

extern bool actualiserOrdre;
extern bool direction;
extern bool enableMoteur;
extern bool renvoieDistance;

extern bool flagInitVitesseTransmission;
extern bool flagAsservisement;
extern bool flagBatterie;
extern bool razDefault;
extern bool razI2C;

class PRobot
{
//Fonction
	public:
		PRobot();
		~PRobot();
		void Run(); // boucle principale
		void RDefautDetecter(PSecurite::Nature nature);
		uint8_t REtatDefaut(); // retourne l'etat des differents defauts
		void EnableMoteur(bool enableMoteur); // logique pour la mise en fonctionement des moteurs 
		
	private:
		void ConfigTimer10ms(); //configure le timer 2
//	Classe
	private:
		PSecurite mSecurite;
		PI2C mI2C;
		PMoteur mMoteur;
		PRouesCodeuses mRouesCodeuses;
		
//Variable
	private:
		bool menableMoteur;
		char temp[15];
};


#endif //__PROBOT_H__
