/* 
* PI2C.h
*
* Created: 27/12/2016 23:20:09
* Author: Aurelien
*/


#ifndef __PI2C_H__
#define __PI2C_H__

#include <util/twi.h>

class PI2C //classe gérant le bus I2C
{

	public:
		PI2C(uint8_t adresse);
		~PI2C();
		void Start(); //connecte le µC au bus I2C
		void Stop(); //déconnecte le µC du bus I2C
		void Init(); //configure le composant I2C
	private:
		
		uint8_t mAdresse;
		
}; //PI2C

#endif //__PI2C_H__
