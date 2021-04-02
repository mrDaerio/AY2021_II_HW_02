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

#define MIN_TIMEOUT 1
#define MAX_TIMEOUT 20

extern int state;

int timeout = 5;
int timeout_temp = 5;

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
    if (time_counter >= timeout)
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
            else if(received == 0xA1) // setting timeout
            {
                state = TIMEOUT_HEADER;
                UART_PutString("Insert new timeout value:\n");
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
            break;
            
        case TIMEOUT_HEADER:
            if(received > MIN_TIMEOUT-1 && received < MAX_TIMEOUT+1)
            {
                timeout_temp = received;
                UART_PutString("Confirm your last choice by inserting C0\n");
                state = TIMEOUT_CONFIG;
                time_counter = 0;
            }
            else
            {
                UART_PutString("Insert a value between 1 and 20\n");
            }
            break;
            
        case TIMEOUT_CONFIG:
            if(received == 0xC0)
            {
                state = IDLE;
                timeout = timeout_temp;
                UART_PutString("Timeout updated succesfully\n");
            }
            else
            {
                UART_PutString("Insert C0\n");
            }
            break;
    }
    
}
/* [] END OF FILE */
