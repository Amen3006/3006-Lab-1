/*
 * 816029955
 *
 * Created: 10/12/2023
 * Author : Amen a skher Cudjoe
 */ 


#include "sam.h"
#include <atmel_start.h>
#include <stdlib.h>
#define WAIT4CHAR 0
#define WAIT4500MS 1
#define TOGGLELED  2


	


//struct tms n;
void init_TC3();
void enable_interrupts();
// Global error flag for TC3
volatile uint8_t TC3_error = 0;

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	SystemInit(); // Initialize the SAM system
	enable_interrupts();
	init_TC3();
	REG_PORT_DIR0 |= 1 << 21;
	int state,now,led;
	state = 0;
	led=0;
	
	
	while(1){
		uint8_t *input0 = malloc(10);
		uint8_t *input1 = malloc(10);
		switch(state){
			case WAIT4CHAR:
				scanf("%s", input0);
				if(sizeof(input0)/sizeof(uint8_t)==1){
					state = WAIT4500MS;
					
				}
			case WAIT4500MS:
				now = REG_TC3_COUNT16_COUNT;
				while((REG_TC3_COUNT16_COUNT-now)/488 < 1){
					scanf("%s", input1);
					if(input0==input1){
						state = WAIT4CHAR;
						
					}
					else{
						state = TOGGLELED;
						
					}
			case TOGGLELED:
				
				if(led==0){
					REG_PORT_OUT0 |= 1 << 21;
					led =1;
				}
				else{
					REG_PORT_OUT0 &= (~(1 << 21));
					led = 0;
				}
				state = WAIT4CHAR;
				
			}
		}
		
	}
	
		
}

void init_TC3()
{
	
	// Configure Clocks
	REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3;
	while (GCLK->STATUS.bit.SYNCBUSY);
	
	TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
	while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
	while (TC3->COUNT16.CTRLA.bit.SWRST);
	
	REG_PM_APBCMASK |= PM_APBCMASK_TC3; // Enable TC3 bus clock
	
	// Configure TC3 (16 bit counter by default)
	REG_TC3_CTRLA  = TC_CTRLA_MODE_COUNT16 |TC_CTRLA_WAVEGEN_MFRQ | TC_CTRLA_PRESCALER_DIV1024 ;
	//REG_TC3_COUNT16_CC0 = 488;
	
	// Enable interrupts
	REG_TC3_INTENSET = TC_INTENSET_OVF | TC_INTENSET_ERR;
	
	// Enable TC3
	REG_TC3_CTRLA |= TC_CTRLA_ENABLE;
	while ( TC3->COUNT16.STATUS.bit.SYNCBUSY == 1 ){} // wait for TC3 to be enabled
}


void TC3_Handler()
{
	// Overflow interrupt triggered
	if ( TC3->COUNT16.INTFLAG.bit.OVF == 1 )
	{
		REG_TC3_INTFLAG = TC_INTFLAG_OVF;
	}
	
	// Error interrupt triggered
	else if ( TC3->COUNT16.INTFLAG.bit.ERR == 1 )
	{
		TC3_error = 1;
		REG_TC3_INTFLAG = TC_INTFLAG_ERR;
	}
}


void enable_interrupts()
{
	NVIC_EnableIRQ( TC3_IRQn );
}
