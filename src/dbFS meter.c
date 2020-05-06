#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"

//global variables for ADC samples
uint32_t ui32ADC0Value[1]; //data array to store samples from ADC SS1
volatile uint32_t ui32Sample; //sample to be used in meter

//output pin initialization
void PinInit()
{
	//enable clock for GPIO ports B and C
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	//configures output pins
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 + GPIO_PIN_1 + GPIO_PIN_2 + GPIO_PIN_3 + GPIO_PIN_4 + GPIO_PIN_5 + GPIO_PIN_6 + GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 + GPIO_PIN_5 + GPIO_PIN_6 + GPIO_PIN_7);
}

//ADC initializaiton
void ADC0_Init(void)
{
	// configure the system clock to be 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	//enable clock for ADC0 and GPIO port E
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	//configure ADC pin
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_2);
	//allow the clock to be fully activated
	SysCtlDelay(2);

	//disable ADC0 before the configuration is complete
	ADCSequenceDisable(ADC0_BASE, 1);
	//configure the sequence to use (ADC0, SS1, processor-trigger, priority 0)
	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	//configure step 0 of the sequence to be the last
	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);

	//configure ADC0 SS1 interrupt priority as 0
	IntPrioritySet(INT_ADC0SS1, 0x00);
	//enable interrupt 31 in NVIC (ADC0 SS1)
	IntEnable(INT_ADC0SS1);	
	//arm interrupt of ADC0 SS1
	ADCIntEnableEx(ADC0_BASE, ADC_INT_SS1);
	//enable ADC0
	ADCSequenceEnable(ADC0_BASE, 1);
}
		
//interrupt handler
void ADC0_Handler(void)
{
	//clear interrupt
	ADCIntClear(ADC0_BASE, 1);
	//trigger sample seqeuence
	ADCProcessorTrigger(ADC0_BASE, 1);
	//get data from sample sequence
	ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
	//put step 0 of the sample sequence into ui32Sample
	ui32Sample = ui32ADC0Value[0];
}

int main(void)
{
	//calculate thresholds based on smalleset db increment
	float threshold[12];
	threshold[0] = 4095;
	float dbValue = -3;
	float multiplier = 10^(dbValue/20);
	for (int i = 1; i < 11; i++)
	{
		threshold[i] = multiplier*threshold[i-1]);
	}
	
	PinInit();	//initialize output pins
	ADC0_Init();	//initialize ADC
	IntMasterEnable();	//globally enable interrupt
	ADCProcessorTrigger(ADC0_BASE, 1);
	const int delay = 100;
	const int  = 1/2;
	while (1)
	{
		if (ui32Sample<threshold[11])
		{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32Sample<threshold[10])
		{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32Sample<threshold[9])
		{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32Sample<threshold[8])
		{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32Sample<threshold[7])
		{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32Sample<threshold[6])
		{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32Sample<threshold[5])
		{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32Sample<threshold[4])
		{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32Sample<threshold[3])
		{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0xFF);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32Sample<threshold[2])
		{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0xFF);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0xFF);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x00);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32Sample<threshold[1])
		{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0xFF);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0xFF);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0xFF);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32Sample<threshold[0])
		{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0xFF);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0xFF);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0xFF);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0xFF);
			SysCtlDelay(delay);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0xFF);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0xFF);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0xFF);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0xFF);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0xFF);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0xFF);
			SysCtlDelay(delay);
		}
	}
}
