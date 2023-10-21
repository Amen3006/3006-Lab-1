/*
 * Lab1.c
 *
 * Created: 9/22/2023 5:17:02 PM
 * Author : Amen a skher Cudjoe
 */ 


#include "sam.h"

//Amen a skher Cudjoe - 816029955
int main(void)
{
	int i;
	SystemInit();
	
	//configure porta21 as output
	REG_PORT_DIR0 |= 1 << 21;
	
	while (1)
	{
		//output high on porta21
		REG_PORT_OUT0 |= 1 << 21;
		
		//wait
		for(i = 0; i < 100000; i++){}
			
		//output low on porta21
		REG_PORT_OUT0 &= (~(1 << 21));
		
		//wait
		for(i = 0; i < 100000; i++){}
	}
}