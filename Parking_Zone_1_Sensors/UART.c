#include "UART.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h> 

#define NVIC_EN0_UART0 0x20
#define NVIC_EN0_UART1 0x40
#define NVIC_EN1_UART2 0x02
//#define NVIC_EN1_UART3 0x??

void UART0_Init(bool RxInt, bool TxInt){
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; // activate UART0
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port A
  UART0_CTL_R = 0;                      // disable UART
  UART0_IBRD_R = 54;                    // IBRD = int(50,000,000 / (16 * 38400 )) = int(81.3802)
  UART0_FBRD_R = 16;                     // FBRD = int(0.3802 * 64 + 0.5) = 16
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
 // UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
 UART0_LCRH_R = UART_LCRH_WLEN_8;
	
	// take care of interrupt setup
	if ( RxInt | TxInt) {
		NVIC_PRI1_R = (NVIC_PRI1_R&~0x0000E000)|0x0000A000; // bits 15-13, priority 5
		NVIC_EN0_R = NVIC_EN0_UART0;           // enable UART0 interrupt in NVIC
		if (RxInt) {
			UART0_IM_R |= UART_IM_RXIM;         // Enable RX interrupt
		}
		
		if (TxInt) {
			UART0_IM_R |= UART_IM_TXIM;         // Enable TX interrupt
		}
	}

  UART0_CTL_R |= UART_CTL_RXE|UART_CTL_TXE|UART_CTL_UARTEN;// enable Tx, RX and UART
  GPIO_PORTA_AFSEL_R |= 0x03;           // enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x03;             // enable digital I/O on PA1-0
                                        // configure PA1-0 as UART
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTA_AMSEL_R &= ~0x03;          // disable analog functionality on PA
}

void UART2_Init(bool RxInt, bool TxInt){
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART2;  // Activate UART2
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;  // Activate port D
  while((SYSCTL_RCGC2_R & SYSCTL_RCGC2_GPIOD) == 0); // Wait for port D to be ready
  
  GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;  // Unlock GPIO Port D
  GPIO_PORTD_CR_R |= 0xC0;  // Allow changes to PD7 and PD6
  GPIO_PORTD_LOCK_R = 0x00;  // Lock GPIO Port D

  UART2_CTL_R = 0;  // Reset UART1
  UART2_IBRD_R = 8;  // IBRD = int(16,000,000 / (16 * 115,200)) = int(8.68)
  UART2_FBRD_R = 44;  // Baud rate: 38400, FBRD = int(0.68 * 64 + 0.5) = 44
  UART2_LCRH_R = UART_LCRH_WLEN_8;  // 8 bit word length (no parity bits, one stop bit, FIFOs)
  
  if (RxInt) {
    UART2_IM_R |= UART_IM_RXIM;  // Enable RX interrupt
  }
  
  if (TxInt) {
    UART2_IM_R |= UART_IM_TXIM;  // Enable TX interrupt
  }
  
  UART2_CTL_R |= UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE;  // Enable UART, Rx, Tx
  
  if (RxInt || TxInt) {
    NVIC_PRI8_R = (NVIC_PRI8_R & 0xFFFF1FFF) | 0x0000A000;  // Bits 15-13, priority 5 (bit |15|14|13| = 101
    NVIC_EN1_R = NVIC_EN1_UART2;  // Enable interrupt 5 in NVIC
  }
  
  GPIO_PORTD_AFSEL_R |= 0xC0;  // Enable alternate function on PD7-6
  GPIO_PORTD_DEN_R |= 0xC0;    // Enable digital I/O on PD7-6
  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R & 0x00FFFFFF) + 0x11000000;  // Configure PD7-6 as UART
  GPIO_PORTD_AMSEL_R &= ~0xC0;  // Disable analog functionality on PD7-6
}


uint8_t UART_InChar(void){
  while((UART0_FR_R&UART_FR_RXFE) != 0); // wait until the receiving FIFO is not empty
  return((uint8_t)(UART0_DR_R&0xFF));
}

void UART_OutChar(uint8_t data){
  while((UART0_FR_R&UART_FR_TXFF) != 0);
  UART0_DR_R = data;
}

uint8_t UART2_InChar(void){
  while((UART2_FR_R&UART_FR_RXFE) != 0); // wait until the receiving FIFO is not empty
  return((uint8_t)(UART2_DR_R&0xFF));
}

void UART2_OutChar(uint8_t data){
  while((UART2_FR_R&UART_FR_TXFF) != 0);
  UART2_DR_R = data;
}