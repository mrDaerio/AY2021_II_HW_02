/* ========================================
 *
 * Group 11 - Comini, Di Liddo, Marelli
 * LTEBS II 2021
 * Assignment 2
 * File RGBLedDriver.h
 * header file for RGB PWM control functions declarations
 * and color type definition
 *
 * ========================================
*/
#ifndef __RGBLED_PWM_H
    #define __RGBLED_PWM_H
    
    #include "global_variables.h"
    
    void RGBLED_Start();
    void RGBLED_Stop();
    void RGBLED_WriteColor(color c);
#endif

/* [] END OF FILE */