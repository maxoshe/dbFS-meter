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


uint32_t ui32ADC0Value[1]; //data array to store samples from ADC SS1
volatile uint32_t ui32TempAvg;	//avaraged measurement from temp sensor

void PinInit()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 + GPIO_PIN_1 + GPIO_PIN_2 + GPIO_PIN_3 + GPIO_PIN_4 + GPIO_PIN_5 + GPIO_PIN_6 + GPIO_PIN_7); //set pins as output
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 + GPIO_PIN_5 + GPIO_PIN_6 + GPIO_PIN_7);
}

//ADC0 initializaiton
void ADC0_Init(void)
{
		SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); // configure the system clock to be 40MHz
		SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);	//activate the clock of ADC0
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
		GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_2);
		
		SysCtlDelay(2);	//insert a few cycles after enabling the peripheral to allow the clock to be fully activated.

		ADCSequenceDisable(ADC0_BASE, 1); //disable ADC0 before the configuration is complete
		ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0); // will use ADC0, SS3, processor-trigger, priority 0
		ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);  	//ADC0 SS3 Step 0 sample

		IntPrioritySet(INT_ADC0SS1, 0x00);	// configure ADC0 SS1 interrupt priority as 0
		IntEnable(INT_ADC0SS1);	// enable interrupt 31 in NVIC (ADC0 SS1)
		ADCIntEnableEx(ADC0_BASE, ADC_INT_SS1);	// arm interrupt of ADC0 SS1
	
		ADCSequenceEnable(ADC0_BASE, 1);	//enable ADC0
}
		
//interrupt handler
void ADC0_Handler(void)
{
		ADCIntClear(ADC0_BASE, 1);
		ADCProcessorTrigger(ADC0_BASE, 1);
		ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
		ui32TempAvg = ui32ADC0Value[0];
}

int main(void)
{
	PinInit();	//initialize GPIO pins
	ADC0_Init();
	IntMasterEnable();       		// globally enable interrupt
	ADCProcessorTrigger(ADC0_BASE, 1);
	const int delay = 100;
	const int thresh = 4095/12;
	while (1)
	{
		if (ui32TempAvg<thresh)
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
//			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32TempAvg<2*thresh)
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
//			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32TempAvg<3*thresh)
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
//			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32TempAvg<4*thresh)
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
//			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32TempAvg<5*thresh)
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
//			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32TempAvg<6*thresh)
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
//			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32TempAvg<7*thresh)
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
//			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32TempAvg<8*thresh)
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
//			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32TempAvg<9*thresh)
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
//			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32TempAvg<10*thresh)
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
//			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32TempAvg<11*thresh)
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
//			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
			SysCtlDelay(delay);
		}
		else if (ui32TempAvg<12*thresh)
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
//			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
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
