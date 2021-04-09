/* ========================================
 *
 * Group 11 - Comini, Di Liddo, Marelli
 * LTEBS II 2021
 * Assignment 2
 * File RGBLedDriver.c
 * source file for RGB PWM control functions definitions
 *
 * ========================================
*/

#include "RGBLedDriver.h"

static void RGBLED_WriteRed(uint8_t red);
static void RGBLED_WriteGreen(uint8_t green);
static void RGBLED_WriteBlu(uint8_t blu);


/*******************************************************************************
* Function Name: RGBLED_Start()
********************************************************************************
* Summary:
*  Starts the two PWM peripherals for RGB control
*******************************************************************************/
void RGBLED_Start()
{
    PWM_RG_Start();
    PWM_B_Start();
}

/*******************************************************************************
* Function Name: RGBLED_WriteColor(color c)
********************************************************************************
* Summary:
*  Given a color struct, sets the values of PWM to change the brightness of each
*  color.
*  
* Parameters:
*  color c: color struct containing the three values for red, green and blue.
*           (defined in RGBLedDriver.h)
*******************************************************************************/
void RGBLED_WriteColor(color c)
{
    RGBLED_WriteRed(c.red);
    RGBLED_WriteGreen(c.green);
    RGBLED_WriteBlu(c.blu);
}

/*******************************************************************************
* Function Name: RGBLED_WriteRed
********************************************************************************
* Summary:
*  Internal function to set the red color of PWM
*
* Parameters:
*  red: value for color red (0-255)
*******************************************************************************/
static void RGBLED_WriteRed(uint8_t red)
{
    PWM_RG_WriteCompare1(red);
}

/*******************************************************************************
* Function Name: RGBLED_WriteGreen
********************************************************************************
* Summary:
*  Internal function to set the green color of PWM
*
* Parameters:
*  green: value for color green (0-255)
*******************************************************************************/
static void RGBLED_WriteGreen(uint8_t green)
{
    PWM_RG_WriteCompare2(green);
}

/*******************************************************************************
* Function Name: RGBLED_WriteBlu
********************************************************************************
* Summary:
*  Internal function to set the blue color of PWM
*
* Parameters:
*  blu: value for color blue (0-255)
*******************************************************************************/
static void RGBLED_WriteBlu(uint8_t blu)
{
    PWM_B_WriteCompare(blu);
}

/* [] END OF FILE */
