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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        if(Button_Read () ==1)
        {
            GREEN_Write(0);
            RED_Write(1);
            BLUE_Write(0);
    } else{
            GREEN_Write(1);
            RED_Write(0);
            BLUE_Write(0);
            
        }
    }
}
