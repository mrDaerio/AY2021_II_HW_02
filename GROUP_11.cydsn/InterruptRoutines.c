/* ========================================
 *
 * Group 11 - Comini, Di Liddo, Marelli
 * LTEBS II 2021
 * Assignment 2
 * File InterruptRoutines.c
 * source file for TIMER and UART interrupts definitions
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
#define TIMER_PERIOD 999
#define DEFAULT_TIMEOUT 5

#define MESSAGES_LEN 30

extern int state; //Global variable

uint8_t timeout = DEFAULT_TIMEOUT;
uint8_t timeout_temp = DEFAULT_TIMEOUT;
uint8_t time_counter = 0;

extern color rgb_color;

/*******************************************************************************
* Function Name: Custom_TIMER_OF_ISR
********************************************************************************
* Summary:
*  A counter is updated at timer overflow, if the state variable is one of the
*  intermediate states. If the counter reaches a timeout variable (between 1-20s)
*  the state is reset to IDLE
*******************************************************************************/
CY_ISR(Custom_TIMER_OF_ISR)
{
    Timer_ReadStatusRegister(); // Clears interrupt
    
    if (state > IDLE && state < TAIL) // Control timeout
    {
        time_counter++;
        //char str[20];
        //sprintf(str, "Current time: %d\r\n", time_counter);
        //UART_PutString(str);
    }
    if (time_counter >= timeout) //Timeout overflow --> reset to IDLE
    {
        UART_PutString("Time is up! Returning to IDLE\n");
        state = IDLE;
        time_counter = 0;
    }
    
}

/*******************************************************************************
* Function Name: Custom_UART_RX_ISR
********************************************************************************
* Summary:
*  When a serial byte is received, the interrupt checks the value of 'state'
*  flag. 
*
*  To update the color of the RGB led, it must receive 5 consecutive bytes:
*  [0xA0, R, G, B, 0xC0]
*   -0xA0: header byte to initialize color change
*   -R: value of red brightness (0-255)
*   -G: value of green brightness (0-255)
*   -B: value of blue brightness (0-255)
*   -0xC0: tail byte, to confirm the color that will be set in the 'main' function
*
*  To update the counter timeout it must receive 3 bytes:
*  [0xA1, T, 0xC0]
*   -0xA1: header byte to initialize timeout change
*   -T: value of counter timeout (1-20)
*   -0xC0: tail byte to confirm timeout change
*
*  To connect to the application, while in IDLE state, if the UART receives 
*  a character 'v' it must send back a response string:
*  "RGB LED Program $$$"
*
*  The function sends error messages on the serial port for unexpected values
*  and cycles until the UART buffer is empty
*******************************************************************************/
CY_ISR(Custom_UART_RX_ISR)
{
    uint8_t received;
    char message[MESSAGES_LEN];
    
    //Do if Buffer is non empty 
    while(UART_GetRxBufferSize()!=0) 
    {
        received = UART_ReadRxData(); //Read the data (Buffer shifts of 1 step)
        
        // Management of different states
        switch (state){
            case IDLE:
                // Check received key
                if (received == HEADER_CMD) //Start changing color
                {
                    state = HEADER;
                    UART_PutString("Insert RED data\n");
                    time_counter = 0;
                    Timer_WriteCounter(TIMER_PERIOD); //Reset timer
                }
                else if(received == TIMEOUT_HEADER_CMD) // Setting timeout
                {
                    state = TIMEOUT_HEADER;
                    UART_PutString("Insert new timeout value:\n");
                }
                else if (received == CONNECTION_CMD) // Connection command
                {
                    UART_PutString("RGB LED Program $$$"); //Connection echo
                }
                else
                {
                    UART_PutString("Data not accepted.\n"
                                   "To insert a color send 0xA0\n"
                                   "To set timeout send 0xA1\n");
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
                if (received == TAIL_CMD) // Correct key --> see main.c
                {
                    UART_PutString("Tail byte received\n");
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
                    sprintf(message, "Inserted number: %d\r\n", timeout_temp);
                    UART_PutString(message);
                    UART_PutString("Confirm your choice by inserting 0xC0\n");
                    state = TIMEOUT_CONFIG;
                }
                else
                {
                    UART_PutString("Data not accepted.\nInsert a value between 1 and 20\n");
                }
                break;
                
            case TIMEOUT_CONFIG:
                if(received == TAIL_CMD)
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
