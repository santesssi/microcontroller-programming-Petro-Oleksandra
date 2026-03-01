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
    CyGlobalIntEnable;
    SW_Tx_UART_Start();
    SW_Tx_UART_PutCRLF();
    SW_Tx_UART_PutString("Software Transmit UART");
    SW_Tx_UART_PutCRLF();
    
    uint8_t last_state = 0;
    for(;;)
    {
        if(Button_Read () ==1 && last_state == 0)
        {
            last_state = 1;
            SW_Tx_UART_PutString("OMG!!! The button released!");
            SW_Tx_UART_PutCRLF();
            GREEN_Write(0);
            RED_Write(0);
            BLUE_Write(0);
    } else if(Button_Read() == 0 && last_state == 1)
    {
            last_state = 0;
            SW_Tx_UART_PutString("Petro Oleksandra pressed the button <3");
            SW_Tx_UART_PutCRLF();
            GREEN_Write(1);
            RED_Write(1);
            BLUE_Write(1);
        }
    }
}
