#define TRIGGER_PIN_PB5 (*((volatile unsigned long *)0x40005080)) // PB5
#define TRIGGER_PIN_PB4 (*((volatile unsigned long *)0x40005040)) // PB4
#define TRIGGER_PIN_PB3 (*((volatile unsigned long *)0x40005020)) // PB3
#define TRIGGER_VALUE_PB5 0x20 // trigger at bit 5
#define TRIGGER_VALUE_PB4 0x10 // trigger at bit 4
#define TRIGGER_VALUE_PB3 0x08 // trigger at bit 3
#define ECHO_PIN_PB6 (*((volatile unsigned long *)0x40005100)) // PB6 is the echo pin
#define ECHO_PIN_PB7 (*((volatile unsigned long *)0x40005200)) // PB7 is the echo pin
#define ECHO_PIN_PB0 (*((volatile unsigned long *)0x40005004)) // PB0 is the echo pin
#define ECHO_VALUE_Bit6 0x40 // trigger at bit 6
#define ECHO_VALUE_Bit7 0x80 // trigger at bit 7
#define ECHO_VALUE_Bit0 0x01// trigger at bit 0
#define MC_LEN 0.0625 // length of one machine cycle in microseconds for a 16MHz clock
#define SOUND_SPEED 0.0343 // centimeters per microsecond

void HCSR04_Init_1(void);
void HCSR04_Init_2(void);
void HCSR04_Init_3(void);