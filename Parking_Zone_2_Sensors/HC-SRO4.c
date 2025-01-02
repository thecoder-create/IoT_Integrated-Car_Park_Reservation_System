#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "HC-SR04.h"

void HCSR04_Init_1(void) { 
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // 1) Activate clock for Port B
    while ((SYSCTL_RCGC2_R & SYSCTL_RCGC2_GPIOB) == 0) {}; // Wait for clock to start
    
    // Configure PB5 as GPIO for Trigger
    GPIO_PORTB_PCTL_R &= ~0x00FFF000; // Clear PCTL for PB5
    GPIO_PORTB_AFSEL_R &= ~(TRIGGER_VALUE_PB5 | TRIGGER_VALUE_PB4 | TRIGGER_VALUE_PB3); // Regular function for PB5
    GPIO_PORTB_DIR_R |= (TRIGGER_VALUE_PB5 | TRIGGER_VALUE_PB4 | TRIGGER_VALUE_PB3); // PB5 as output for Trigger
    GPIO_PORTB_DEN_R |= (TRIGGER_VALUE_PB5 | TRIGGER_VALUE_PB4 | TRIGGER_VALUE_PB3); // Enable digital function for PB5
			
	  GPIO_PORTB_AMSEL_R &= (uint32_t)~0xF9;             // disable analog function
    
    // Configure PB6 for Echo with interrupts
    //GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & ~0x0F000000) | (0x07000000); // Set PCTL for PB6 to Timer0CCP
		GPIO_PORTB_PCTL_R &= ~0xFF00000F; // Clear PCTL for PB6, PB7 and PB0
		GPIO_PORTB_AFSEL_R &= ~(ECHO_VALUE_Bit6 | ECHO_VALUE_Bit7 | ECHO_VALUE_Bit0); // Regular function for PB6
    //GPIO_PORTB_AFSEL_R |= ECHO_VALUE_Bit6; // Alternate function for PB6
    GPIO_PORTB_DIR_R &= ~(ECHO_VALUE_Bit6 | ECHO_VALUE_Bit7 | ECHO_VALUE_Bit0); // PB6, PB7, PB0 as input for Echo
    GPIO_PORTB_DEN_R |= (ECHO_VALUE_Bit6 | ECHO_VALUE_Bit7 | ECHO_VALUE_Bit0); // Enable digital function for PB6, PB7 AND PB0
		
		GPIO_PORTB_IS_R &= ~(ECHO_VALUE_Bit6 | ECHO_VALUE_Bit7 | ECHO_VALUE_Bit0);                          // PB6, PB7 AND PB0 is edge-sensitive
		GPIO_PORTB_IBE_R |= (ECHO_VALUE_Bit6 | ECHO_VALUE_Bit7 | ECHO_VALUE_Bit0);                          // PB6, PB7 AND PB0 is both edges
		GPIO_PORTB_IEV_R &= ~(ECHO_VALUE_Bit6 | ECHO_VALUE_Bit7 | ECHO_VALUE_Bit0);                         // PB6, PB7 AND PB0 both edge event
		GPIO_PORTB_ICR_R = (ECHO_VALUE_Bit6 | ECHO_VALUE_Bit7 | ECHO_VALUE_Bit0);                           // clear flag 6, 7 and 0
		GPIO_PORTB_IM_R |= (ECHO_VALUE_Bit6 | ECHO_VALUE_Bit7 | ECHO_VALUE_Bit0);                           // arm interrupt on PB6, PB7 AND PB0
		NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF1FFF)|0x00006000; // (g) priority 3
		NVIC_EN0_R = 0x00000002;                           // (h) enable Port B edge interrupt
			
}

