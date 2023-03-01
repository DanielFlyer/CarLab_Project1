/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 * 
 * 20.5cm circumferance of wheels
 * ========================================
*/
#include "project.h"
#include <stdio.h>
int counter; 
float dutyCycle;
int currTime;
float totalErr;
float currErr;
float prevErr;
float diffErr;
float kp = .2;
float ki = 0.005;
float kd = 0; 
float u0 = 22;

CY_ISR(inter)
{
    char strbuf[32];
    //counter++; 
    //sprintf(strbuf, "Count: %i", counter);                                                                                                                                                                                                                                                
    //LCD_ClearDisplay();
    //LCD_Position(0,0);
    //LCD_PrintString(strbuf);
    
    char strbuf2[32];
  
    LCD_Position(0,0);
    currTime = 65535 - Timer_ReadCounter();
    Timer_WriteCounter(65535);
    sprintf(strbuf, "Time: %i, %i", currTime, (int)dutyCycle);
    LCD_ClearDisplay();
    LCD_PrintString(strbuf);
    LCD_Position(0,0);
    
    sprintf(strbuf2, "%s\r\n",strbuf);
    UART_PutString(strbuf2);
    
    //PID Loop
    currErr = currTime - 336;
    if (currErr < 500){
        totalErr += currErr;
    }
    diffErr = currErr - prevErr; 
    dutyCycle = u0 + currErr*kp + ki*totalErr + diffErr*kd;
    
    prevErr = currErr;
}
    
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    counter = 0;
    dutyCycle = u0;
    currTime = 0;
    totalErr = 0;
    currErr = 0;
    prevErr = 0;
    diffErr = 0;
    
    LCD_Start();
    UART_Start();
    PWM_Start();
    Timer_Init();
    Timer_Enable();
    Timer_Start();
    
    HallEffect_Interrupt_Start();
    HallEffect_Interrupt_SetVector(inter);
 
    LCD_Position(0,0);
    LCD_PrintString("Hello, World"); // DELETE
    PWM_WriteCompare(u0);
    UART_PutString("Trial Start");

    for(;;)
    {
        PWM_WriteCompare(dutyCycle);
       // UART_PutString("jbjbjhb");
    }
}

/* [] END OF FILE */
