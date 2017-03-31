/*
 * PSecurite.cpp
 *
 * Created: 23/12/2016 22:11:18
 *  Author: Aurelien
 */ 
#include "PSecurite.h"

PSecurite::PSecurite() :cptBat(0)
{
	ConfigInterruptExt();
	ConfigCANFreeRunning();
}

PSecurite::~PSecurite()
{
	
}

void PSecurite::ConfigInterruptExt()
{
	PCMSK1 = (1<<PCINT8);// interruption uniquement sur PCINT8
	PCICR = (1<<PCIE1);//activation de l'interruption sur les pins PCINT[14:8]
}

void PSecurite::ConfigCANFreeRunning()
{
	//Configuration du CAN
	
	//VREF=Tension sur broche AVCC donc 5V
	ADMUX = (1<<REFS0); 
	ADMUX &= ~(1<<REFS1);
	// Canal 3 du CAN broche PC3
	ADMUX |= (1<<MUX0)|(1<<MUX1); 
	ADMUX &= ~(1<<MUX2)& ~(1<<MUX3);
	
	ADCSRA = (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);//Horloge CAN = Quartz / 128 =125kHz
	ADCSRA |= (1 << ADEN)|(1<<ADATE);//CAN ON et mode Free-running
	ADCSRA &= ~(1 << ADIE);//pas d'interruption
	
	//ADSC=1 Lancement Conversion CAN
	ADCSRA |= (1 << ADSC);

}

void PSecurite::DefautDetecte(Nature nature,PMoteur &Moteur)
{
	//si on detecte un default
	
	//on etteint le moteur
	Moteur.EteindreMoteur();
	
	//on met le flag de default approprié
	if (nature == Nature::DriverMoteur)
		mDefaultDriver = 1;
		
	else if	(nature == Nature::Batterie)
			mDefaultBatterie = 1;
			
	else if (nature == Nature::TimeOut)
			mDefaultTimeOut = 1;
}


uint8_t PSecurite::EtatDefaut()
{
	//on renvoie les flag de défault mise en forme pour la transmission
	return ((mDefaultDriver<<2) + (mDefaultBatterie<<1) + mDefaultTimeOut); 
}

void PSecurite::RAZDefaut()
{
	//si pas de défault alors on réinitialise les flag de défault
	if (ADC>730)
	{
		mDefaultBatterie = false;
	}
	if ((PINC & 1) == 0)
	{
		mDefaultDriver = false;
	}
	mDefaultTimeOut = false;
	TCNT0 = 0;
	compteurTimeOut = 0;
}

bool PSecurite::VerifierBatterie(PMoteur &Moteur)
{
	//vérifie la tension aux borne de la batterie
	flagBatterie = false;
	if (ADC<=710) //si la tension est inferieure à 10.5V
	{
		cptBat++;
		
		if (cptBat>=10)
		{
		//alors on déclenche le défault batterie
		DefautDetecte(PSecurite::Nature::Batterie, Moteur);
		 return false;
		 cptBat=0;
		}
		else 
			return true;
	}
	else
		{
			cptBat=0;
		 return true;
		}
}