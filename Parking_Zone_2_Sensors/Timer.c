//Timer.c
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
This program initializes Timer0A for capture mode and Timer 1A for periodic mode.
The purpose of this program is to send 10us pulses to the trigger pin on the Ultrasonic sensor.
In addition, it captures the high-level event of the echo pulse. This can be used to calculate the object's distance.
Programmed by Edward Ramos

*/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Timer0A.h"
#include "HC-SR04.h"
#include "leds.h"
#define TIMER_TAMR_TACDIR_Down 0x00000000  // GPTM Timer A Count Down Direction

unsigned long long int period = 0xFFFFFFFFFFFFFFF;
static volatile uint32_t high_edge = 0;
static volatile uint32_t low_edge = 0;
static volatile uint32_t timer_lapse = 0;
static volatile uint32_t distance = 0;

int count = 0;


void Timer0A_Init(){
	//Initializes Timer 0A for capture mode
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;
	while((SYSCTL_PRTIMER_R & SYSCTL_PRTIMER_R0) == 0) {
       // Busy-wait loop until Timer0 clock is stable
  }
	TIMER0_CTL_R &= ~(TIMER_CTL_TAEN); // This disables Timer0A
	TIMER0_CFG_R = TIMER_CFG_32_BIT_TIMER; //This selects the 16-bit timer configuration
	TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD | TIMER_TAMR_TACDIR_Down; // Configures for capture mode, count-up and edge-time mode
	TIMER0_TAILR_R = period-1; // GPTM Timer A Interval Load Register
	//TIMER0_CTL_R &= ~(1<<3)|~(1<<2); //Captures both edges

//	TIMER0_ICR_R = TIMER_ICR_CAECINT;           // Clear Timer0A capture event interrupt
//  TIMER0_IMR_R |= TIMER_IMR_CAEIM;            // Enable Timer0A capture mode interrupt
//	NVIC_PRI4_R = (NVIC_PRI4_R&0x1FFFFFFF)|0x100000000; // Configures for priority 1 for Timer 0A.
//  NVIC_EN0_R = 0x00080000; //Enables interrupt for Timer0a
	TIMER0_CTL_R = TIMER_CTL_TAEN; //This enables Timer 0A
	

}

void Timer1A_Init(){
	//Initializes Timer1A for periodic mode
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;
	while((SYSCTL_PRTIMER_R & SYSCTL_PRTIMER_R1) == 0) {
        // Busy-wait loop until Timer1 clock is stable
  }
	TIMER1_CTL_R &= ~(TIMER_CTL_TAEN); // This disables Timer1A
	TIMER1_CFG_R = TIMER_CFG_32_BIT_TIMER; //This selects the 32-bit timer configuration
	TIMER1_TAMR_R = TIMER_TAMR_TAMR_PERIOD | TIMER_TAMR_TACDIR_Down; // This configures for periodic count-down mode and caputre mode.
	TIMER1_TAILR_R = period-1; // GPTM Timer A Interval Load Register
	TIMER1_CTL_R = TIMER_CTL_TAEN; //This enables TIMER0A
}

void Timer2A_Init(){
	//Initializes Timer1A for periodic mode
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R2;
	while((SYSCTL_PRTIMER_R & SYSCTL_RCGCTIMER_R2) == 0) {
        // Busy-wait loop until Timer2 clock is stable
  }
	TIMER2_CTL_R &= ~(TIMER_CTL_TAEN); // This disables Timer1A
	TIMER2_CFG_R = TIMER_CFG_32_BIT_TIMER; //This selects the 32-bit timer configuration
	TIMER2_TAMR_R = TIMER_TAMR_TAMR_PERIOD | TIMER_TAMR_TACDIR_Down; // This configures for periodic count-down mode and caputre mode.
	TIMER2_TAILR_R = period-1; // GPTM Timer A Interval Load Register
	TIMER2_CTL_R = TIMER_CTL_TAEN; //This enables TIMER0A
}

void Wait_us(int delay){
	TIMER1_CTL_R &= ~(TIMER_CTL_TAEN); // This disables Timer0A
	TIMER1_TAILR_R = delay*16 - 1; //Load value for 10us delay (microsecond * clock frequency)-1
	TIMER1_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
	TIMER1_ICR_R = TIMER_ICR_TATOCINT; // Clears the TATORIS bit in the GPTMRIS register and the TATOMIS bit in the GPTMMIS register.
  TIMER1_CTL_R = TIMER_CTL_TAEN;  // Enables Timer0A
  while((TIMER1_RIS_R)==0); // wait for count flag
	TIMER1_IMR_R = 0x00000000;
}

void Timer0A_Handler(void){
		TIMER0_ICR_R = TIMER_ICR_TATOCINT;//Clear the Timer A timeout flag
}

void Timer1A_Handler(void){
		TIMER1_ICR_R = TIMER_ICR_TATOCINT;//Clear the Timer A timeout flag
}

void Timer2A_Handler(void){
		TIMER2_ICR_R = TIMER_ICR_TATOCINT;//Clear the Timer A timeout flag
}

void Timer0A_Start(void){
		TIMER0_CTL_R &= ~0x00000001; // Disable TIMER1A during setup
	  TIMER0_TAILR_R = period - 1;  //or just period??----------------------------
    //TIMER0_TAV_R = 0x00000000;   // Clear TIMER1A's current value
    TIMER0_CTL_R = 0x00000001;   // Enable TIMER1A
	  TIMER0_IMR_R = 0x00000000;

}

void Timer0A_Stop(void){
	TIMER0_CTL_R &= ~0x00000001;
	TIMER0_IMR_R &= ~0x00000001;
}

void Timer1A_Start(void){
		TIMER1_CTL_R &= ~0x00000001; // Disable TIMER1A during setup
	  TIMER1_TAILR_R = period - 1;  //or just period??----------------------------
    //TIMER1_TAV_R = 0x00000000;   // Clear TIMER1A's current value
    TIMER1_CTL_R = 0x00000001;   // Enable TIMER1A
	  TIMER1_IMR_R = 0x00000000;

}

void Timer1A_Stop(void){
	TIMER1_CTL_R &= ~0x00000001;
	TIMER1_IMR_R &= ~0x00000001;
}

void Timer2A_Start(void){
		TIMER2_CTL_R &= ~0x00000001; // Disable TIMER1A during setup
	  TIMER2_TAILR_R = period - 1;  //or just period??----------------------------
    //TIMER1_TAV_R = 0x00000000;   // Clear TIMER1A's current value
    TIMER2_CTL_R = 0x00000001;   // Enable TIMER1A
	  TIMER2_IMR_R = 0x00000000;

}

void Timer2A_Stop(void){
	TIMER2_CTL_R &= ~0x00000001;
	TIMER2_IMR_R &= ~0x00000001;
}

//void Timer0A_Handler(void) {
//		TIMER0_ICR_R = TIMER_ICR_CAECINT; // Acknowledge Timer 0A capture event interrupt
//    
//    uint32_t capture = TIMER0_TAR_R;  // Capture the current timer value
//    
//    if (high_edge == 0) {
//        // Rising edge (start of pulse)
//        high_edge = capture;
//    } else {
//        // Falling edge (end of pulse)
//        if (capture >= high_edge) {
//            timer_lapse = capture - high_edge;
//        } else {
//            timer_lapse = (0xFFFF - high_edge + capture + 1);  // Handle timer overflow
//        }

//        // Calculate distance
//        distance = ((timer_lapse * MC_LEN * SOUND_SPEED) / 2);  // Result in centimeters

//        // Reset high_edge for the next pulse
//        high_edge = 0;
//    }
//}

unsigned long Timer1A_Get_Timer_Lapse(void) {
    // Read the current value of Timer1A
    uint32_t timerValue = TIMER1_TAV_R;
    
    // Calculate the number of machine cycles elapsed
    uint32_t time_difference = (TIMER1_TAILR_R) - timerValue;
    
    // Convert to unsigned long before returning
    return (unsigned long)time_difference;
}

unsigned long Timer0A_Get_Timer_Lapse(void) {
    // Read the current value of Timer1A
    uint32_t timerValue = TIMER0_TAV_R;
    
    // Calculate the number of machine cycles elapsed
    uint32_t time_difference = (TIMER0_TAILR_R) - timerValue;
    
    // Convert to unsigned long before returning
    return (unsigned long)time_difference;
}

unsigned long Timer2A_Get_Timer_Lapse(void) {
    // Read the current value of Timer1A
    uint32_t timerValue = TIMER2_TAV_R;
    
    // Calculate the number of machine cycles elapsed
    uint32_t time_difference = (TIMER2_TAILR_R) - timerValue;
    
    // Convert to unsigned long before returning
    return (unsigned long)time_difference;
}


//unsigned long Timer0A_Get_Timer_Lapse(void) {
//    
//    // Convert to unsigned long before returning
//    return (unsigned long)timer_lapse;
//}


