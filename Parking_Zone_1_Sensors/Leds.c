#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "leds.h"

void LEDS_Sensor1_And_2_Init(void){
	
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;  // 1) activate clock for Port A
	while((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOA)!=SYSCTL_RCGC2_GPIOA){}; // wait for clock to start

	GPIO_PORTA_PCTL_R &= ~0xFFFFFF00;
	GPIO_PORTA_AMSEL_R &= (uint32_t)~0xFC;      // 4) disable analog function 
  GPIO_PORTA_DIR_R |= 0xFC;         // 5) LEDS, output
  GPIO_PORTA_AFSEL_R &= ~0xFC;      // 6) regular port function
  GPIO_PORTA_DEN_R |= 0xFC;         // 7) enable digital port

}

void LEDS_Sensor3_Init(void){
	
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;  // 1) activate clock for Port E
	while((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOE)!=SYSCTL_RCGC2_GPIOE){}; // wait for clock to start

	GPIO_PORTE_PCTL_R &= ~0x00000FFF;
	GPIO_PORTE_AMSEL_R &= (uint32_t)~0x07;      // 4) disable analog function 
  GPIO_PORTE_DIR_R |= 0x07;         // 5) LEDS, output
  GPIO_PORTE_AFSEL_R &= ~0x07;      // 6) regular port function
  GPIO_PORTE_DEN_R |= 0x07;         // 7) enable digital port

}



void On_Board_LEDS_Init(void){
	
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;  // 1) activate clock for Port F
	while((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)!=SYSCTL_RCGC2_GPIOF){}; // wait for clock to start

	GPIO_PORTF_PCTL_R &= ~0x0FFF0;
	GPIO_PORTF_AMSEL_R &= (uint32_t)~0x0E;      // 4) disable analog function on P123
  GPIO_PORTF_DIR_R |= 0x0E;         // 5) PB123:LEDS, output
  GPIO_PORTF_AFSEL_R &= ~0x0E;      // 6) regular port function
  GPIO_PORTF_DEN_R |= 0x0E;         // 7) enable digital port

}