#define LED_Sensor1 (*((volatile unsigned long *)0x40004380)) //PA
#define LED_Sensor2 (*((volatile unsigned long *)0x40004070)) //PA
#define LED_Sensor3 (*((volatile unsigned long *)0x4002401C)) //PE
#define RGB_ON_BOARD (*((volatile unsigned long *)0x40025038)) //PF
	
#define GREEN1 0x40
#define RED1 0x20
#define BLUE1 0x80 
#define YELLOW1 (RED1 | GREEN1)
#define CYAN1 (BLUE1 | GREEN1)


#define GREEN2 0x10
#define RED2 0x08
#define BLUE2 0x04
#define YELLOW2 (RED2 | GREEN2)
#define CYAN2 (BLUE2 | GREEN2)

#define GREEN3 0x02
#define RED3 0x01
#define BLUE3 0x04
#define YELLOW3 (RED3 | GREEN3)
#define CYAN3 (BLUE3 | GREEN3)

#define GREEN4 0x08
#define RED4 0x02
#define BLUE4 0x04
#define YELLOW4 (RED4 | GREEN4)
#define CYAN4 (BLUE4 | GREEN4)

#define OFF 0x00

void LEDS_Sensor1_And_2_Init(void);
void LEDS_Sensor3_Init(void);
void On_Board_LEDS_Init(void);
