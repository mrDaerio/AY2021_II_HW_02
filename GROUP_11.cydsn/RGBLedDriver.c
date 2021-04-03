/* ========================================
 *
 * 
 *
 * ========================================
*/

#include "RGBLedDriver.h"

static void RGBLED_WriteRed(uint8_t red);
static void RGBLED_WriteGreen(uint8_t green);
static void RGBLED_WriteBlu(uint8_t blu);

volatile color rgb_color={0,0,0};

void RGBLED_Start()
{
    PWM_RG_Start();
    PWM_B_Start();
}

void RGBLED_Stop()
{
    PWM_RG_Stop();
    PWM_B_Stop();
}

void RGBLED_WriteColor(color c)
{
    RGBLED_WriteRed(c.red);
    RGBLED_WriteGreen(c.green);
    RGBLED_WriteBlu(c.blu);
}

static void RGBLED_WriteRed( uint8_t red)
{
    PWM_RG_WriteCompare1(red);
}

static void RGBLED_WriteGreen(uint8_t green)
{
    PWM_RG_WriteCompare2(green);
}
static void RGBLED_WriteBlu(uint8_t blu)
{
    PWM_B_WriteCompare(blu);
}

/* [] END OF FILE */
