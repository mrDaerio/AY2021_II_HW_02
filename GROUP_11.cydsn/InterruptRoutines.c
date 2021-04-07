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

#include "InterruptRoutines.h"

#define MIN_TIMEOUT 1 //Minimum value acceptable for timeout settings
#define MAX_TIMEOUT 20 //Maximum value acceptable for timeout settings

#define TIMER_PERIOD 999 //Reset value for Timer's internal counter

#define DEFAULT_TIMEOUT 5 //Default timeout for intermediate states (5s) before state reset to IDLE

uint8_t timeout = DEFAULT_TIMEOUT; //timeout initialization
uint8_t timeout_temp = DEFAULT_TIMEOUT; //initialization for timeout configuration temporary variable

uint8_t time_counter = 0; //counter for time (in seconds)


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
        TimerReset();
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
                    TimerReset();
                }
                else if(received == TIMEOUT_HEADER_CMD) // Setting timeout
                {
                    state = TIMEOUT_HEADER;
                    sprintf(message,"Insert new timeout value between 0x%02x and 0x%02x:\r\n",MIN_TIMEOUT,MAX_TIMEOUT);
                    UART_PutString(message);
                }
                else if (received == CONNECTION_CMD) // Connection command
                    UART_PutString("RGB LED Program $$$"); //Connection echo
                else
                {
                    sprintf(message,"Data not accepted: 0x%02x\r\n"
                                    "To insert a color send 0x%02x\r\n"
                                    "To set timeout send 0x%02x\r\n",received,HEADER_CMD,TIMEOUT_HEADER_CMD);
                    UART_PutString(message);
                }
                break;
            case HEADER:
                rgb_color.red = received;  // Update red value
                sprintf(message, "Received RED value: 0x%02x\r\n", received);
                UART_PutString(message);
                state = RED;
                UART_PutString("Insert GREEN data\r\n");
                TimerReset();
                break;
            case RED:
                rgb_color.green = received; // Update green value
                sprintf(message, "Received GREEN value: 0x%02x\r\n", received);
                UART_PutString(message);
                state = GREEN;
                UART_PutString("Insert BLU data\r\n");
                TimerReset();
                break;
            case GREEN:
                rgb_color.blu = received;  // Update blu value
                sprintf(message,"Received BLU value: 0x%02x\r\n"
                                "Confirm your choice by inserting 0x%02x\r\n", rgb_color.blu,TAIL_CMD);
                UART_PutString(message);
                state = BLU;
                TimerReset();
                break;
            case BLU:
                //Check recived key
                if (received == TAIL_CMD) // Correct key --> see main.c
                    state = TAIL;
                else
                {
                     sprintf(message,"Data not accepted.\r\nConfirm your choice by inserting 0x%02x\r\n",TAIL_CMD);
                     UART_PutString(message);
                }
                break;
                
            case TIMEOUT_HEADER:
                // Timeout settings update
                if(received > MIN_TIMEOUT-1 && received < MAX_TIMEOUT+1)
                {
                    timeout_temp = received;
                    state = TIMEOUT_CONFIG;
                    sprintf(message, "New timeout: %d seconds\r\nConfirm by inserting 0x%02x\r\n", timeout_temp,TAIL_CMD);
                    UART_PutString(message);

                }
                else
                {
                    sprintf(message,"Data not accepted: 0x%02x\r\nInsert new timeout value between 0x%02x and 0x%02x:\r\n",received,MIN_TIMEOUT,MAX_TIMEOUT);
                    UART_PutString(message);
                }
                break;
                
            case TIMEOUT_CONFIG:
                if(received == TAIL_CMD)
                {
                    timeout = timeout_temp;
                    state = IDLE;
                    sprintf(message,"\fTimeout updated succesfully to %d seconds\r\n\r\n",timeout);
                    UART_PutString(message);
                    sprintf(message,"To insert a color send 0x%02x\r\nTo set timeout send 0x%02x\r\n",HEADER_CMD,TIMEOUT_HEADER_CMD);
                    UART_PutString(message);
                }
                else
                {
                    sprintf(message,"Data not accepted: 0x%02x\r\nConfirm by inserting 0x%02x\r\n",received,TAIL_CMD);
                    UART_PutString(message);
                }
                break;
        }
        
    };

}

/*******************************************************************************
* Function Name: TimerReset
********************************************************************************
* Summary: reset timer and counter variable
*******************************************************************************/
void TimerReset()
{
    time_counter = 0;
    Timer_WriteCounter(TIMER_PERIOD);
}
/* [] END OF FILE */
