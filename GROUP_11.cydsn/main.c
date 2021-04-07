/* ========================================
 *
 * Group 11 - Comini, Di Liddo, Marelli
 * LTEBS II 2021
 * Assignment 2
 * File main.c
 *
 * ========================================
*/
#include "project.h"
#include "InterruptRoutines.h"
#include "global_variables.h"
#include "RGBLedDriver.h"

volatile int state = IDLE;

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
