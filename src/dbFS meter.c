#include <stdint.h>
#include <stdbool.h>
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
	PinInit();	//initialize output pins
	ADC0_Init();	//initialize ADC
	IntMasterEnable();	//globally enable interrupt
	ADCProcessorTrigger(ADC0_BASE, 1);
	const int delay = 100;
	const int thresh = 4095/12;
	while (1)
	{
		if (ui32Sample<thresh)
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
		else if (ui32Sample<2*thresh)
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
		else if (ui32Sample<3*thresh)
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
		else if (ui32Sample<4*thresh)
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
		else if (ui32Sample<5*thresh)
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
		else if (ui32Sample<6*thresh)
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
		else if (ui32Sample<7*thresh)
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
		else if (ui32Sample<8*thresh)
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
		else if (ui32Sample<9*thresh)
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
		else if (ui32Sample<10*thresh)
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
		else if (ui32Sample<11*thresh)
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
		else if (ui32Sample<12*thresh)
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
