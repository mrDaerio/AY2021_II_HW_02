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
    
    #include "RGBLedDriver.h"
    
    // Switch cases for color change
    #define IDLE 0 
    #define HEADER 1
    #define RED 2
    #define GREEN 3
    #define BLU 4
    #define TAIL 5
    
    // Switch cases for timeout configuration
    #define TIMEOUT_HEADER 6
    #define TIMEOUT_CONFIG 7
    
    // Header and tail commands
    #define HEADER_CMD 0xA0
    #define TAIL_CMD 0xC0
    #define TIMEOUT_HEADER_CMD 0xA1
    
    // Connection for the 'rgbcontrol.exe'
    #define CONNECTION_CMD 'v'
    
    volatile color rgb_color; //global color variable
    
#endif

/* [] END OF FILE */
