/* ========================================
 *
 * 
 *
 * ========================================
*/

#ifndef __RGBLED_PWM_H
    #define __RGBLED_PWM_H
    
    #include "project.h"
    #include "cytypes.h"
    
    typedef struct{
        uint8_t red;
        uint8_t green;
        uint8_t blu;
    }color;
    
    void RGBLED_Start();
    void RGBLED_Stop();
    void RGBLED_WriteColor(color c);
#endif

/* [] END OF FILE */