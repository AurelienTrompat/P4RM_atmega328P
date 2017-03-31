/*
 * P4RM_atmega328P.cpp
 *
 * Created: 11/11/2016 12:35:01
 *  Author: Aurelien
 */
#include "P4RM_atmega328P.h"

int main(void)
{
	sei(); //autorise les interruptions
	_delay_ms(1000);
	Robot.Run();
}

ISR(TIMER2_COMPA_vect)
{
	compteur1s++;
	if (compteur1s == 100)
	{
		compteur1s = 0;
		flagBatterie = true; //flag qui lance la v�rification de l'�tat de la batterie
	}
	
	flagAsservisement = true; // flag pour la mise a jour du rapport cyclique de la PWM moteur	
}

 ISR(PCINT1_vect) //Broche d�fault du driveur moteur � 1 
 {  
	 // Alors d�clenchement du d�fault driveur moteur
	 Robot.RDefautDetecter(PSecurite::Nature::DriverMoteur);
 }
 
 ISR(TWI_vect) //interuption g�n�rer par le composant I2C
 {
/*
	Trame re�u :
	 -premier octet : 
		- poids fort :	0
						0 
		- 2eme bit :	0 
		- 3eme bit :	0 -> Demande de donn�es
						1 -> Nouvelle Consigne
		- 4eme bit :	0 -> Rien
						1 -> RAZ d�fault
		- 5eme bit :	0 -> pas de renvoie de distance
						1 -> renvoie de distance timer TimeOut en fonctionement (il faut demander la distance au minimun tout les 74ms
		- 6eme bit :	0 -> Moteur eteint driver moteur en veille
						1 -> Moteur allum�
		- 7eme bit :	0 -> Marche Arriere
						1 -> Marche Avant
		- poids faible  0 -> pas de vitesse progressive
						1 -> vitesse Progressive
	-Octet Deux : Consigne de Vitesse entre 0 (0%) et 255 (100%)
*/	 
	 uint8_t donnees = 0;
	 
	 if (TW_STATUS == TW_SR_SLA_ACK) //si Adresse slave re�u + W et ACK retourn�
	 {
		 //d�but de r�ception
		 premieroctet = true;
	 } 
	 else if (TW_STATUS == TW_SR_DATA_ACK) // si donnees re�u + ACK retourn�
	 {
		 if (premieroctet) //si premier octet
		 {
			 donnees = TWDR;
			 if ((donnees & 32) == 1 << 5 ) // si bit 3 = 1 -> consigne sinon on ignore 
			 {
				 //si donnees re�u diff�rents des donnees enregistrer
				 if (donnees != (uint8_t)( 32 + vitesseProgressive + (direction<<1) + (enableMoteur<<2) + (renvoieDistance<<3) + (razDefault<<4)))
				 { //alors on actualise
					 actualiserOrdre = true;
					 vitesseProgressive = (donnees & 1);
					 direction = (donnees & 2)>>1;
					 enableMoteur = (donnees & 4)>>2;
					 razDefault = (donnees & 16)>>4;
					 
					 //si on renvoie distance passe a 1
					 if ((renvoieDistance == false) && ((donnees & 8)>>3 == true) && Robot.REtatDefaut()==0)
					 { // alors initialisation du compteur timeOut et de la distance � renvoyer
						TCNT0 =0;
						compteurTimeOut = 0;	
					 }
					 renvoieDistance = ((donnees & 8)>>3);
				 }
				 
			 }
			premieroctet = false;
		 }
		 else //si premier octet d�ja re�u alors on re�oit la vitesse
		 { 
			 // actualisation de la consigne de vitesse
			 vitesseMoteur=TWDR;
			 actualiserVitesse = true;
		 }
	  }
	  else if (TW_STATUS == TW_ST_SLA_ACK) //si Adresse slave + R re�u et ACK retourn�
	  { //envoie des donn�es
		  donnees = Robot.REtatDefaut(); //lecture  et envoie de l'�tat des d�faults
		  if (donnees != 0) //si d�fault pr�sent
		  {
			  // alors envoie des bits de poids fort de la distance d'arret
			  donnees += (compteurTimeOut & 0x07)<<3;
		  }
		  if (renvoieDistance == true || donnees != 0)
		  {
			 tempDistance=TCNT0;
			 if (donnees==0)
				TCNT0=0;
			 donnees += (tempDistance & 0x80) >>1; 
		  }
		  
		  TWDR = donnees;
	  }
	  else if (TW_STATUS== TW_ST_DATA_ACK) //si donn�es envoy�e et ACK re�u
	  {
		 //envoie de la distance
		 TWDR = tempDistance & 0x7f;
	  }
	  else if (TW_STATUS == TW_BUS_ERROR) //si erreur sur le bus on r�initialise le peripherique I2C
		razI2C = true;
	TWCR |= (1<<TWINT); //clear le flag qui d�clenche la fonction d'interruption (non fait par l'hardware)
 }
 
 ISR(TIMER0_OVF_vect) //interruption d'overflow du timer 0 (compteur des pas des roues codeuses)
 {
	if (renvoieDistance) // si renvoie distance
	{
		Robot.RDefautDetecter(PSecurite::Nature::TimeOut); //alors on d�clenche l'erreur
		compteurTimeOut++;
	}
 }