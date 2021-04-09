/* ========================================
 *
 * Group 11 - Comini, Di Liddo, Marelli
 * LTEBS II 2021
 * Assignment 2
 * File main.c
 *
 * ========================================
*/

#include "global_variables.h"
#include "InterruptRoutines.h"
#include "RGBLedDriver.h"

volatile color rgb_color = {0,0,0};
volatile uint8_t state = IDLE;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    // Peripherals start
    Timer_Start();
    UART_Start();
    RGBLED_Start();
    
    // ISR init. of our custom functions
    isrTIMER_StartEx(Custom_TIMER_OF_ISR);
    isrUART_StartEx(Custom_UART_RX_ISR);
    
    //Message confirming program has started
    char message[MESSAGES_LEN];
    sprintf(message,"\fTo insert a color send 0x%02x\r\nTo set timeout send 0x%02x\r\n",HEADER_CMD,TIMEOUT_HEADER_CMD);
    UART_PutString(message);
    for(;;)
    {
        switch (state)
        {
            case TAIL:
                RGBLED_WriteColor(rgb_color);
                state = IDLE;
                sprintf(message,"\fColors updated to: (R:0x%02x G:0x%02x B:0x%02x)\r\n\r\n", rgb_color.red,rgb_color.green,rgb_color.blu);
                UART_PutString(message);
                sprintf(message,"To insert a color send 0x%02x\r\nTo set timeout send 0x%02x\r\n",HEADER_CMD,TIMEOUT_HEADER_CMD);
                UART_PutString(message);
                break;
        }

    }
}

/* [] END OF FILE */
