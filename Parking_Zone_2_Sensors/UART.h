#include <stdbool.h> 

void UART2_Init(bool RxInt, bool TxInt);

unsigned char UART2_InChar(void);

void UART2_OutChar(unsigned char data);