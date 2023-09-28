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
	/* Replace with your application code */
	REG_PORT_DIR1 |= 1 << 8;
	
	while (1)
	{
		REG_PORT_OUT1 |= 1 << 8;
		for(i = 0; i < 100000; i++){}
		
		REG_PORT_OUT1 &= (~(1 << 8));
		for(i = 0; i < 100000; i++){}
	}
}