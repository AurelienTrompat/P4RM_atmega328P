/* 
* PRouesCodeuses.h
*
* Created: 30/12/2016 11:21:58
* Author: Aurelien
*/


#ifndef __PROUESCODEUSES_H__
#define __PROUESCODEUSES_H__

#include <avr/io.h>
#include <string.h>

class PRouesCodeuses
{
//fonctions
	public:
		PRouesCodeuses();
		~PRouesCodeuses();
		
	private:
		void ConfigCompteur(); //configure le timer 0 en compteur sur son entrée de comptage externe

}; //PRouesCodeuses

#endif //__PROUESCODEUSES_H__
