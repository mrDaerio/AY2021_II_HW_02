/* ========================================
 *
 * Group 11 - Comini, Di Liddo, Marelli
 * LTEBS II 2021
 * Assignment 2
 * File global_variables.h
 * Macros for the state variable and message commands
 *
 * ========================================
*/
#ifndef __GLOBAL_VARIABLES_H__
    #define __GLOBAL_VARIABLES_H__
    
    #define IDLE 0
    #define HEADER 1
    #define RED 2
    #define GREEN 3
    #define BLU 4
    #define TAIL 5
    #define TIMEOUT_HEADER 6
    #define TIMEOUT_CONFIG 7
    #define HEADER_CMD 0xA0
    #define TAIL_CMD 0xC0
    #define TIMEOUT_HEADER_CMD 0xA1
    #define CONNECTION_CMD 'v'
    
#endif

/* [] END OF FILE */
