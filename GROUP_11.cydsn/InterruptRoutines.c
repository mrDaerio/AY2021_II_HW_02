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
#include "global_variables.h"

extern int state;

uint8_t time_counter = 0;

// ISR for the Timer
CY_ISR(Custom_TIMER_OF_ISR)
{
    /* ISR code goes here */
    Timer_ReadStatusRegister(); // Clears interrupt
    //UART_PutString("Timer Overflow\n");
    
    if (state > IDLE && state < TAIL) //aggiungere condizioni 
    {
        //gestione timeout
        time_counter++;
    }
    if (time_counter >= 5) //5 in realtà è variabile
    {
        UART_PutString("Time is up! Returning to IDLE\n");
        state = IDLE;
        time_counter = 0;
    }
    
}

// ISR for the UART
CY_ISR(Custom_UART_RX_ISR)
{
    /* ISR code goes here */
    uint8_t received = UART_ReadRxData();
    //UART_PutString("Message Received\n");
    switch (state){
        case IDLE:
            if (received == 0xA0)
            {
                state = HEADER;
                UART_PutString("Insert RED data\n");
            }
            else
            {
                UART_PutString("Non recognized data, to insert a color send 0xA0\n");
            }
            break;
        case HEADER:
            //salva dato in variabile red
            UART_PutString("Received RED\n");
            state = RED;
            UART_PutString("Insert GREEN data\n");
            time_counter = 0;
            break;
        case RED:
            //salva dato in variabile green
            UART_PutString("Received GREEN\n");
            state = GREEN;
            UART_PutString("Insert BLU data\n");
            time_counter = 0;
            break;
        case GREEN:
            //salva dato in variabile blu(e)
            UART_PutString("Received BLU\n");
            state = BLU;
            UART_PutString("Wating for TAIL\n");
            time_counter = 0;
            break;
        case BLU:
            if (received == 0xC0)
            {
                UART_PutString("TAIL received\n");
                state = TAIL;
            }
            else
            {
                UART_PutString("Data not recognized. Waiting for TAIL\n");
            }
            
    }
    
}
/* [] END OF FILE */
