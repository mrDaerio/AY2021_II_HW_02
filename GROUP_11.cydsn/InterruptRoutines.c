/* ========================================
 *
 * Group 11 - Comini, Di Liddo, Marelli
 * LTEBS II 2021
 * Assignment 2
 * File InterruptRoutines.c
 *
 * ========================================
*/
#include "project.h"
#include "InterruptRoutines.h"
#include "UART.h"
#include "stdio.h"
#include "global_variables.h"
#include "RGBLedDriver.h"

#define MIN_TIMEOUT 1 //Minimum value acceptable for timeout settings
#define MAX_TIMEOUT 20 //Maximum value acceptable for timeout settings
#define TIMER_PERIOD 1000

extern int state; //Global variable

uint8_t timeout = 5;
uint8_t timeout_temp = 5;
uint8_t time_counter = 0;

extern color rgb_color;

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
    uint8_t received;
    char message[30];
    
    //Do if Buffer is non empty 
    while(UART_GetRxBufferSize()!=0) 
    {
        received = UART_ReadRxData(); //Read the data (Buffer shifts of 1 step)
        
        // Management of different states
        switch (state){
            case IDLE:
                // Check received key
                if (received == 0xA0) //Start changing color
                {
                    state = HEADER;
                    UART_PutString("Insert RED data\n");
                    Timer_WriteCounter(TIMER_PERIOD); //Reset timer
                }
                else if(received == 0xA1) // Setting timeout
                {
                    state = TIMEOUT_HEADER;
                    UART_PutString("Insert new timeout value:\n");
                }
                else if (received == 'v') // Connection command
                {
                    UART_PutString("RGB LED Program $$$"); //Connection echo
                }
                else
                {
                    UART_PutString("Data not accepted.\n");
                    UART_PutString("To insert a color send 0xA0\n");
                    UART_PutString("To set timeout send 0xA1\n");
                }
                break;
            case HEADER:
                rgb_color.red = received;  // Update red value
                sprintf(message, "Received RED value: %d\r\n", received);
                UART_PutString(message);
                state = RED;
                UART_PutString("Insert GREEN data\n");
                time_counter = 0;
                Timer_WriteCounter(TIMER_PERIOD); //Reset timer
                break;
            case RED:
                rgb_color.green = received; // Update green value
                sprintf(message, "Received GREEN value: %d\r\n", received);
                UART_PutString(message);
                state = GREEN;
                UART_PutString("Insert BLU data\n");
                time_counter = 0;
                Timer_WriteCounter(TIMER_PERIOD); //Reset timer
                break;
            case GREEN:
                rgb_color.blu = received;  // Update blu value
                sprintf(message, "Received BLUE value: %d\r\n", received);
                UART_PutString(message);
                state = BLU;
                UART_PutString("Confirm your choice by inserting 0xC0\n");
                time_counter = 0;
                Timer_WriteCounter(TIMER_PERIOD); //Reset timer
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
        
    };

}
/* [] END OF FILE */
