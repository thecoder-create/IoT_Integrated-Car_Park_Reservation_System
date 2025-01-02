//Ultrasonic_Sensor.c
/*
...------:...:-----:...............................
..:--#@@@%   %@@@*--...............................
.....--%@@@@@@@=-:.................................
......:-@@@@@@--...................................
.......-=@@@@=--...................................
.......--@@@@-@*--:................................
... ...:-@@@@--#@@--:..::----:...... .. .... .... .
.......:-@@@@---*@@%--+#%%%%%#*=---................
.......--@@@@-::-%@@@@@@@@@@@@@@@@%--:.............
... ...-+@@@@--.--@@@@@+=====+@@@@@@@--..... .... .
......--@@@@@*-::-%@@@*-:....::--=@@@@#--..........
....--+@@@@@@@#---%@@@%-:.......:--+@@@@=--........
....:--*@@@@@@=---%@@@@-:.......--=%@@@@@=-:.......
... ..:-%@@@@--.--@@@@%-:.....::--*@@@@=-:.. .... .
.......-=@@@@-::-#@@@@%--------%@@@@@@--...........
.......-=@@@*-:-+@@@@@@@@@@@@@@@@@@#--:... ........
.......--@@@+--%@@@@@*#%@@@@@@@%==----.............
.......-=@@@#---#@@@@@@@@@@@@@@@@@@@@#---..........
......:-*@@@@----@@@@@@@@@@@%%%@@@@@@@@@=-:........
......--@@@@@@=--=@@@@*------@------%@@@@%--.......
....:-=@@@@@@@@@@%@@@@#*##%@@%-:...:--+@@@@=-:... .
.:-=*@@@%   %@@@@@@@@@@@@@@@@--....:-=%@@@@@%=-:...
..-------...------@@@@==+%@@@%-:....---@@@@%---....
................-#@@@%=-----+@--:---+@@@@@=-:......
... .... .. ...--@@@@@@@@@@#+++*%@@@@@@@#--. .... .
..............--@@@*@@@@@@@@@@@@@@@@@@+--..........
............:-=@@*--:---+#%%@@@@%#+---:............
...........--%%---.......:-------:.................
...........:---:...................................
Team Name: Beach Park
*/

#include <stdint.h>
#include <stdbool.h> 
#include "tm4c123gh6pm.h"
#include "Timer0A.h"
#include "HC-SR04.h"
#include "leds.h"
#include "PLL.h"
#include "UART.h"

#define Sensor4 0x04
#define Sensor5 0x05
#define Sensor6 0x06

#define Reserve_Sensor4 0x2A 
#define Reserve_Sensor5 0x34
#define Reserve_Sensor6 0x3E

#define Unreserve_Sensor4 0x2B 
#define Unreserve_Sensor5 0x35
#define Unreserve_Sensor6 0x3F 

// External functions for interrupt control defined in startup.s
extern void DisableInterrupts(void); // Disable interrupts
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // low power mode


static volatile uint32_t distance1 = 0;
static volatile uint32_t distance2 = 0;
static volatile uint32_t distance3 = 0;
static volatile uint8_t done=0;

static volatile uint32_t sensor4_status = 0x00;
static volatile uint32_t sensor5_status = 0x00;
static volatile uint32_t sensor6_status = 0x00;
static volatile uint32_t recieved_data = 0x00;


bool HCSR04_1 = false;
bool HCSR04_2 = false;
bool HCSR04_3 = false;


bool reserved_sensor4 = false;
bool reserved_sensor5 = false;
bool reserved_sensor6 = false;


bool send_data = false;

 int main(void){
	DisableInterrupts();
	PLL_Init(); 
	UART2_Init(true, false);
  Timer1A_Init(); //Activate perodic countdown mode.
	Timer0A_Init();
	Timer2A_Init();
	Wait_us(1000000); // To start the system fully, give at least 1 second delay.
	LEDS_Sensor1_And_2_Init();
  LEDS_Sensor3_Init();
  On_Board_LEDS_Init();
	HCSR04_Init_1();
	EnableInterrupts();

	 
  while(1){
		done = 0; 
		
		//Get data for first sensor
		TRIGGER_PIN_PB5 &= ~TRIGGER_VALUE_PB5; // Send low to end trigger pulse
    Wait_us(2); 

    TRIGGER_PIN_PB5 |= TRIGGER_VALUE_PB5;  // Send high to trigger
    Wait_us(10);                   // Wait 10 us for trigger pulse
    TRIGGER_PIN_PB5 &= ~TRIGGER_VALUE_PB5; // Send low to end trigger pulse
		while (!done); //Wait until square wave is done.
		
    Wait_us(60000); //Wait 60 ms to get data for next sensor
		done = 0;
		
		//Get data for second sensor
		TRIGGER_PIN_PB4 &= ~TRIGGER_VALUE_PB4; // Send low to end trigger pulse
    Wait_us(2); 

    TRIGGER_PIN_PB4 |= TRIGGER_VALUE_PB4;  // Send high to trigger
    Wait_us(10);                   // Wait 10 us for trigger pulse
    TRIGGER_PIN_PB4 &= ~TRIGGER_VALUE_PB4; // Send low to end trigger pulse
    while (!done); //Wait until square wave is done.
	
		Wait_us(60000); //Wait 60 ms to get data for next sensor
		done = 0;
		
		//Get data for third sensor
		TRIGGER_PIN_PB3 &= ~TRIGGER_VALUE_PB3; // Send low to end trigger pulse
    Wait_us(2); 

    TRIGGER_PIN_PB3 |= TRIGGER_VALUE_PB3;  // Send high to trigger
    Wait_us(10);                   // Wait 10 us for trigger pulse
    TRIGGER_PIN_PB3 &= ~TRIGGER_VALUE_PB3; // Send low to end trigger pulse
		
		while (!done); //Wait until square wave is done.
    Wait_us(2000000); // Two seconds delay to reset system.
  }
}
 
void GPIOPortB_Handler(void){


    if (ECHO_PIN_PB6 == ECHO_VALUE_Bit6) {  // Sensor 1 echo pin rising edge
        Timer0A_Start();  
        HCSR04_1 = true;  
    } 
    else if (ECHO_PIN_PB7 == ECHO_VALUE_Bit7) {  // Sensor 2 echo pin rising edge
        Timer1A_Start();  
        HCSR04_2 = true; 
    } 
		else if(ECHO_PIN_PB0 == ECHO_VALUE_Bit0){
				Timer2A_Start();
				HCSR04_3 = true;
		}
    else { // Falling edge detected, calculate distance for the active sensor
        if (HCSR04_1) {
            Timer0A_Stop();
            distance1 = (uint32_t)(Timer0A_Get_Timer_Lapse() * MC_LEN * SOUND_SPEED) / 2;  // Calculate distance for sensor 1
            if (distance1 <= 10) { 
                LED_Sensor1 = RED1; 
								if(reserved_sensor4){
									reserved_sensor4 = false;
								}
								sensor4_status = 0x29;
            } else {
								if(reserved_sensor4){
									LED_Sensor1 = BLUE1;
									sensor4_status=0x00;
								}
								else{
									LED_Sensor1 = GREEN1;
									sensor4_status = 0x28;
								}
            }
						if(recieved_data == Sensor4){
							UART2_OutChar(sensor4_status);  
							recieved_data = 0x00;
							sensor4_status=0x00;
						}
            HCSR04_1 = false;  
						done = 1;
        }
        
        if (HCSR04_2) {
            Timer1A_Stop();
            distance2 = (uint32_t)(Timer1A_Get_Timer_Lapse() * MC_LEN * SOUND_SPEED) / 2;  // Calculate distance for sensor 2
            if (distance2 <= 10) { 
                LED_Sensor2 = RED2;  
								if(reserved_sensor5)
									reserved_sensor5 = false;
							  sensor5_status = 0x33;
            } else {
								if(reserved_sensor5){
									LED_Sensor2 = BLUE2;
									sensor5_status=0x00;
								}
								else{
									LED_Sensor2 = GREEN2;
									sensor5_status = 0x32;
								}
            }
						if(recieved_data==Sensor5){
							UART2_OutChar(sensor5_status); 
							recieved_data = 0x00;
							sensor5_status=0x00;
						}
            HCSR04_2 = false;  
						done = 1;
        }
        if (HCSR04_3) {
            Timer2A_Stop();
            distance3 = (uint32_t)(Timer2A_Get_Timer_Lapse() * MC_LEN * SOUND_SPEED) / 2;  // Calculate distance for sensor 2
            if (distance3 <= 10) { 
                //LED_Sensor3 = RED3; 
							  RGB_ON_BOARD = RED4;
								if(reserved_sensor6)
									reserved_sensor6=false;
								sensor6_status = 0x3D;
            } else {
                //LED_Sensor3 = GREEN3;  
								if(reserved_sensor6){
									RGB_ON_BOARD = BLUE4;
									sensor6_status=0x00;
								}
								else{
									RGB_ON_BOARD = GREEN4;
									sensor6_status = 0x3C;
								}
            }
						if(recieved_data==Sensor6){
							UART2_OutChar(sensor6_status); 
							recieved_data = 0x00;
							sensor6_status=0x00;
						}
            HCSR04_3 = false;  
						done = 1;
        }

    }

    // Clear interrupt for both echo pins
    GPIO_PORTB_ICR_R = ECHO_VALUE_Bit6 | ECHO_VALUE_Bit7 | ECHO_VALUE_Bit0;
}


void UART0_Handler(void){
    if(UART0_RIS_R&UART_RIS_RXRIS){ // received one item
        if ((UART0_FR_R&UART_FR_RXFE) == 0) {
					
        }
        UART0_ICR_R = UART_ICR_RXIC; // acknowledge RX FIFO
    }
}

void UART2_Handler(void){
  if(UART2_RIS_R&UART_RIS_RXRIS){       // received one item
		if ((UART2_FR_R&UART_FR_RXFE) == 0)
		  recieved_data = UART2_DR_R;
			if(recieved_data==Reserve_Sensor4){
				reserved_sensor4=true;
			}
			else if(recieved_data==Reserve_Sensor5){
				reserved_sensor5 = true;
			}
			else if(recieved_data==Reserve_Sensor6){
				reserved_sensor6=true;
			}
			
			if(recieved_data==Unreserve_Sensor4){
				reserved_sensor4=false;
			}
			else if(recieved_data==Unreserve_Sensor5){
				reserved_sensor5 = false;
			}
			else if(recieved_data==Unreserve_Sensor6){
				reserved_sensor6=false;
			}
    UART2_ICR_R = UART_ICR_RXIC;        // acknowledge RX FIFO
  }
}

