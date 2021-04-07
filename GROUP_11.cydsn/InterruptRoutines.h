/* ========================================
 *
 * LTEBS II 2021
 * Assignment 2
 * File InterruptRoutines.h
 * header file for TIMER and UART interrupts declarations
 *
 * ========================================
*/
#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    
    #include "global_variables.h"
    
    void TimerReset(void);
    
    CY_ISR_PROTO(Custom_UART_RX_ISR);
    CY_ISR_PROTO(Custom_TIMER_OF_ISR);

#endif

/* [] END OF FILE */
