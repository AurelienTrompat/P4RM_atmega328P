/*
 * PSecurite.h
 *
 * Created: 23/12/2016 22:11:41
 *  Author: Aurelien
 */ 


#ifndef PSECURITE_H_
#define PSECURITE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#include "Pmoteur.hpp"

extern bool flagBatterie;
extern uint8_t compteurTimeOut;
class PSecurite
{
//Variables
	public:
		enum class Nature
		{
			Batterie,
			DriverMoteur,
			TimeOut
		};
	
	private:
		bool mDefaultBatterie;
		bool mDefaultDriver;
		bool mDefaultTimeOut;
	
//Fonctions
	public:
		PSecurite();
		~PSecurite();
		void DefautDetecte(Nature nature, PMoteur &Moteur); //met a jour les flag de défaut et etteint le moteur
		uint8_t EtatDefaut(); //renvoie l'etat des flag de défaut
		void RAZDefaut(); //réinitialise les flag de défaut
		bool VerifierBatterie(PMoteur &Moteur); //vérifie la tension des batteries
		
	private:
		void ConfigCANFreeRunning(); //configure le CAN en mode free running sans interruption
		void ConfigInterruptExt(); //configure l'interruption externe sur la broche PC0
		uint8_t cptBat;

};


#endif /* PSECURITE_H_ */