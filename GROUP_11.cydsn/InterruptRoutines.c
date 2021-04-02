/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "InterruptRoutines.h"
#include "UART.h"
#include "stdio.h"

// ISR for the Timer
CY_ISR(Custom_TIMER_OF_ISR)
{
    /* ISR code goes here */
    Timer_ReadStatusRegister(); // Clears interrupt
    UART_PutString("Timer Overflow\n");
}

// ISR for the UART
CY_ISR(Custom_UART_RX_ISR)
{
    /* ISR code goes here */
    UART_PutString("Message Received\n");
}
/* [] END OF FILE */
