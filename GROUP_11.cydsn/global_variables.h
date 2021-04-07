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
    
    #include "project.h"
    #include "stdio.h"
    #include "cytypes.h"
    
    //maximum length of sprintf strings
    #define MESSAGES_LEN 100

    // Color change states
    #define IDLE 0 
    #define HEADER 1
    #define RED 2
    #define GREEN 3
    #define BLU 4
    #define TAIL 5
    
    // Timeout configuration states
    #define TIMEOUT_HEADER 6
    #define TIMEOUT_CONFIG 7
    
    //Header and tail commands
    #define HEADER_CMD 0xA0
    #define TAIL_CMD 0xC0
    #define TIMEOUT_HEADER_CMD 0xA1
    
    // Connection command for 'rgbcontrol.exe' application
    #define CONNECTION_CMD 'v'
    
    //color type declaration
    typedef struct{
        uint8_t red;
        uint8_t green;
        uint8_t blu;
    }color;
    volatile color rgb_color; //global color variable
    
    volatile uint8_t state; //global state variable
    
#endif

/* [] END OF FILE */
