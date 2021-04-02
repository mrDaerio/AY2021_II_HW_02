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

#define MIN_TIMEOUT 1 //Minimum value acceptable for timeout settings
#define MAX_TIMEOUT 20 //Maximum value acceptable for timeout settings

extern int state; //Global variable

uint8_t timeout = 5;
uint8_t timeout_temp = 5;
uint8_t time_counter = 0;

// ISR for the Timer
CY_ISR(Custom_TIMER_OF_ISR)
{
    /* ISR code goes here */
    Timer_ReadStatusRegister(); // Clears interrupt
    
    if (state > IDLE && state < TAIL) // Control timeout
    {
        time_counter++;
        char str[20];
        sprintf(str, "Current time: %d\r\n", time_counter);
        UART_PutString(str);
    }
    if (time_counter >= timeout) //Timeout overflow --> reset to IDLE
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
    
    // Keys receiving
    if(received == 'v')
    {
        UART_PutString("RGB LED Program $$$\n"); //che minchia fa?
    }
    
    // Management of different states
    switch (state){
        case IDLE:
            // Check received key
            if (received == 0xA0) //Start changing color
            {
                state = HEADER;
                UART_PutString("Insert RED data\n");
            }
            else if(received == 0xA1) // Setting timeout
            {
                state = TIMEOUT_HEADER;
                UART_PutString("Insert new timeout value:\n");
            }
            else
            {
                UART_PutString("Data not accepted.\n");
                UART_PutString("To insert a color send 0xA0\n");
                UART_PutString("To set timeout send 0xA1\n");
            }
            break;
        case HEADER:
            /*------salva dato in variabile red--------*/
            UART_PutString("Received RED\n");
            state = RED;
            UART_PutString("Insert GREEN data\n");
            time_counter = 0;
            break;
        case RED:
            /*------salva dato in variabile green--------*/
            UART_PutString("Received GREEN\n");
            state = GREEN;
            UART_PutString("Insert BLU data\n");
            time_counter = 0;
            break;
        case GREEN:
            /*------salva dato in variabile blue--------*/
            UART_PutString("Received BLU\n");
            state = BLU;
            UART_PutString("Confirm your choice by inserting 0xC0\n");
            time_counter = 0;
            break;
        case BLU:
            //Check recived key
            if (received == 0xC0) // Correct key --> see main.c
            {
                UART_PutString("Colors updated succesfully\n\n");
                UART_PutString("Send 0xA0 to change colors\nSend 0xA1 to change Timeout\n");
                state = TAIL;
            }
            else
            {
                UART_PutString("Data not accepted.\nConfirm by inserting 0xC0\n");
            }
            break;
            
        case TIMEOUT_HEADER:
            // Timeout settings update
            if(received > MIN_TIMEOUT-1 && received < MAX_TIMEOUT+1)
            {
                timeout_temp = received;
                char inserted_number[20];
                sprintf(inserted_number, "Inserted number: %d\r\n", timeout_temp);
                UART_PutString(inserted_number);
                UART_PutString("Confirm your choice by inserting 0xC0\n");
                state = TIMEOUT_CONFIG;
                time_counter = 0;
            }
            else
            {
                UART_PutString("Data not accepted.\nInsert a value between 1 and 20\n");
            }
            break;
            
        case TIMEOUT_CONFIG:
            if(received == 0xC0)
            {
                state = IDLE;
                timeout = timeout_temp;
                UART_PutString("Timeout updated succesfully\n\n");
                UART_PutString("Send 0xA0 to change colors\nSend 0xA1 to change Timeout\n");
            }
            else
            {
                UART_PutString("Data not accepted\nInsert 0xC0\n");
            }
            break;
    }
    
}
/* [] END OF FILE */
