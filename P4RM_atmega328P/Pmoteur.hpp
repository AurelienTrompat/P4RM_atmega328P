/*
 * Pmoteur.hpp
 *
 * Created: 12/11/2016 11:44:08
 *  Author: Aurelien
 */ 


#ifndef PMOTEUR_H_
#define PMOTEUR_H_


#define F_CPU 16000000

#include<avr/io.h>
#include <stdint-gcc.h>

#include "PRouesCodeuses.h"

#define FREQUENCE_PWM 20000

extern bool vitesseProgressive;
class PMoteur
{
	public:
		PMoteur();
		~PMoteur();
		void ChangerCommande(uint16_t nouvComande); //change la commande du moteur
		void DemarrerMoteur(); //démarre le moteur
		void EteindreMoteur(); //etteint le moteur
		void ChangerDirection(bool nouvDirection); //Change le sens de rotation des 3 moteurs
		void ChangerConsigne(uint8_t nouvConsigne); //Change la consigne du système
		void CalculCommande(); //calcul le nouveau rapport cyclique de la PWM pour le driver moteur
		
	private:
		uint16_t mNombrePeriode;
		int32_t mCommande;
		double mConsigne;
		bool mPwmConfigurer;
		bool mDirection;	
	
	private:
		void GenererPwm(); //génère la PWM

};



#endif /* PMOTEUR_H_ */