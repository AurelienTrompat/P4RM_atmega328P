/* 
* PI2C.cpp
*
* Created: 27/12/2016 23:20:08
* Author: Aurelien
*/


#include "PI2C.h"

PI2C::PI2C(uint8_t adresse) : mAdresse(adresse)
{
	Init();
} //PI2C


PI2C::~PI2C()
{
	
} //~PI2C

void PI2C::Init()
{
	TWAR = (mAdresse<<TWA0); //adresse en mode esclave 
	TWAR |= (1<<TWGCE); //reconnaissance de l'adresse générale d'appel (0x00)
	
	//active l'interruption du TWI et clear le flag qui déclenche la fonction d'interruption (non fait par l'hardware)
	TWCR = (1<<TWIE)| (1<<TWINT);
}

void PI2C::Start()
{
	//active l'interface TWI et connecte virtuellement le µC au bus I2C
	TWCR |= (1<<TWEN)| (1<<TWEA);
}

void PI2C::Stop()
{
	//désactive l'interface TWI et déconnecte virtuellement le µC au bus I2C
	TWCR &= ~(1<<TWEN)& ~(1<<TWEA);
}