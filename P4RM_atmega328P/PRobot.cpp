/* 
* PRobot.cpp
*
* Created: 02/12/2016 03:49:15
* Author: Aurelien
*/


#include "PRobot.h"

// default constructor
PRobot::PRobot(): mI2C(0x20), menableMoteur(false) //i2c 0x20 = coté intérupteur (droite)
{
	DDRD=0;//Port D en Entrée
	DDRB = 0b00000111;//PB0 a PB2 en sortie
	DDRC = 0b00000010;

	ConfigTimer10ms();
	mI2C.Start();
	
} //PRobot

// default destructor
PRobot::~PRobot()
{
	mMoteur.EteindreMoteur();
	mI2C.Stop();
	
} //~PRobot


void PRobot::Run()
{
	while (true) //boucle principale du programme : gère les 4 autres classes  
	{
		//appelle les fonctions voulu en fonction des différents flag et état du sytèmes.
		if ((actualiserVitesse) && (mSecurite.EtatDefaut() == 0))
		{
			mMoteur.ChangerConsigne(vitesseMoteur);
			actualiserVitesse = false;
		}
		if ((actualiserOrdre) && (mSecurite.EtatDefaut() == 0))
		{
			EnableMoteur(enableMoteur);
			mMoteur.ChangerDirection(direction);
			actualiserOrdre = false;
		}
		
		if (flagAsservisement && menableMoteur)
		{
			mMoteur.CalculCommande();
			flagAsservisement = false;
		}
		
		if (flagBatterie)
			mSecurite.VerifierBatterie(mMoteur);

		if (razDefault)
		{
			mSecurite.RAZDefaut();
			razDefault=0;
		}
		if (razI2C)
		{
			mI2C.Stop();
			_delay_ms(1);
			mI2C.Init();
			mI2C.Start();
			razI2C = false;
		}
	}
}

void PRobot::ConfigTimer10ms() //configure le timer 2 qui permet de cadencer certain appel de fonction
{
	ASSR &= (~(1<<EXCLK)) & (~(1<<AS2));

	TCCR2A &= (~(1<<COM2A1))& (~(1<<COM2A0))& (~(1<<COM2B1))& (~(1<<COM2B0)); //OC0A et OC0B non connecté au timer0
	//mode CTC TOP=OCRA
	TCCR2A &= ~(1<<WGM20);
	TCCR2A |= (1<<WGM21);
	TCCR2B = ~(1<<WGM22);
	
	//predescaler 1024  -> clk
	TCCR2B = (1<<CS20) | (1<<CS21) | (1<<CS22);
	OCR2A =157;
	TIMSK2 = (1<<OCIE2A);
	
}

void PRobot::RDefautDetecter(PSecurite::Nature nature) //transmet a Psecurite d'un nouveau défaut survenu dans une fonction d'interruption
{
	mSecurite.DefautDetecte(nature, mMoteur);
}

uint8_t PRobot::REtatDefaut() //recupère l'etat des defauts dans Psecurite et le transmet à la fonction d'interruption qui en a fait la demmande
{
	return mSecurite.EtatDefaut();
}

void PRobot::EnableMoteur(bool enableMoteur) //gère la mise en marche ou non des moteurs
{
	if (menableMoteur == false && enableMoteur == true)
	{
		mMoteur.DemarrerMoteur();
		menableMoteur=true;
	}
	else if (menableMoteur == true && enableMoteur == false)
	{
		mMoteur.EteindreMoteur();
		menableMoteur=false;
	}
}