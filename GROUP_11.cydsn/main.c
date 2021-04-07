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
    
    //initialize state variable to IDLE
    state = IDLE;
    
    //initialize color variable to black (0,0,0);
    rgb_color.red = 0;
    rgb_color.green = 0; 
    rgb_color.blu = 0;
    
    //Message confirming program has started
    UART_PutString("\fSend 0xA0 to change colors\nSend 0xA1 to change Timeout\n");
    
    for(;;)
    {
        switch (state)
        {
            case TAIL:
                RGBLED_WriteColor(rgb_color);
                UART_PutString("\fColors updated\n\n"
                               "Send 0xA0 to change colors\n"
                               "Send 0xA1 to change Timeout\n");
                state = IDLE;
                break;
        }
    }
}

/* [] END OF FILE */
