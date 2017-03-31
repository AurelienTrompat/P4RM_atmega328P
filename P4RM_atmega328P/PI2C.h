/* 
* PI2C.h
*
* Created: 27/12/2016 23:20:09
* Author: Aurelien
*/


#ifndef __PI2C_H__
#define __PI2C_H__

#include <util/twi.h>

class PI2C //classe g�rant le bus I2C
{

	public:
		PI2C(uint8_t adresse);
		~PI2C();
		void Start(); //connecte le �C au bus I2C
		void Stop(); //d�connecte le �C du bus I2C
		void Init(); //configure le composant I2C
	private:
		
		uint8_t mAdresse;
		
}; //PI2C

#endif //__PI2C_H__
