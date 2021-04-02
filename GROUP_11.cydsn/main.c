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
#include "global_variables.h"

volatile int state = IDLE;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Timer_Start();
    UART_Start();
    isrTIMER_StartEx(Custom_TIMER_OF_ISR);
    isrUART_StartEx(Custom_UART_RX_ISR);
    
    UART_PutString("Send 0xA0 to change colors\nSend 0xA1 to change Timeout\n");

    for(;;)
    {
        /* Place your application code here. */
        switch (state)
        {
            case TAIL:
                //creazione source file per gestione PWM
                //qua chiamo la funzione PWM
                UART_PutString("Colors updated\n");
                state = IDLE;
                break;
        }
    }
}

/* [] END OF FILE */
