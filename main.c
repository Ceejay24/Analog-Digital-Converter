#include "TM4C123GH6PM.h"
#include <stdint.h>
#include <stdio.h>

//Declare a function for millisecond delay
void delayMs(int n);

int main(void)
{
	//declare an integer for storing the ADC value
	volatile int ADC_VAL;
	//declare a float for storing the converted ADC value in RPM
	volatile float RPM;
	//declare a constant float for storing RPM step size = 4095.0/2500
	const float y = 4095.0/2500.0;
	
	//Step 1. enable clock for GPIOD for the configuration of 
	//AIN6 (RCGCGPIO p340)
	SYSCTL->RCGCGPIO |= 0x08;
	//Step 2. enable the clock to ADC6(RCGCADC p352)
	SYSCTL->RCGCADC |= 0x01; //generally used
	
	/*initialise PD1 for AIN6 input*/
	//step3. enable alternate function for GPIOD pin 0 (GPIOAFSEL p675)
	GPIOD->AFSEL |= 0x02;
	//step 4. disable digital function for GPIOD Pin 1 (GPIODEN p683)
	GPIOD->DEN &= ~0x02;
	//sep5. enable analog function for GPIOD pin1 (GPIOAMSEL p687)
	GPIOD -> AMSEL |= 0x02;
	
	//step6. disable SS3 during configuration (ADCACTSS p821)
	//this is for sample sequencer 3 according to the question
	ADC0->ACTSS &= ~0x08;
	//step7. configure ADC for a software trigger 
	//or continuous sampling (ADCEMUX p833)
	ADC0-> EMUX |= 0xF000;
	//step8. get input from channel 6 (AIN6) PD1 (ADCSSMUX2 p875/p801)
	//input is gotten from the input number on the table
	ADC0-> SSMUX3 |= 0x6;
	//step 9. take one sample at a time, set flag at ist sample (ADCSSCTL3 p876)
	ADC0->SSCTL3 |= 0x6;
	//step 10. enable ADC0 sequencer 3 (ADCACTSS p821)
	ADC0->ACTSS |= 0x08;
	
	while(1)
	{
		//step 11a. start a conversion sequence on SS3 (ADCPSSI p845)
		ADC0->PSSI |= 0x08;
		//step11b. wait for conversion complete by polling the correct 
		//bit 3 (SS3 interrupt bit) of ADCRIS register (p823)
		while((ADC0->RIS & 8)==0);
		//step 11c. read the conversion result and store (SSFIFO3 p828)
		ADC_VAL = ADC0->SSFIFO3;
		//step 11d. clear the ADC SS3 interrupt flag (ADCISC bit 3 p828)
		ADC0->ISC = 0x00;
		//step 11e. wait 1 second before starting loop again
		//and taking another reading.
		//delayMs(100);
		RPM = ADC_VAL/y;
		delayMs(500);
	}
}

void delayMs(int n)
{
	int i, j;
	for (i=0; i<n; i++)
		for (j=0;j<8300;j++)
			{
			}
}


