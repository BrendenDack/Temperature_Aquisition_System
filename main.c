#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "LCD.h"
#include "TimerA0.h"
#include "PLL.h"
#include "SysTickInts.h"
//#include "mailbox.h"

extern uint8_t mailbox;

float Sample_to_Farenheit(uint32_t sample) {
    float celsius = ((sample*3.3)/4096) * 100;
    float farenheit = (celsius * (9/5)) + 32;
    return farenheit * 10;
}

void GPIO_PORTF_INIT(){
    SYSCTL_RCGC2_R |= (1<<5);                   /* Set bit 5 of RCGCGPIO to enable clock to PORTF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;             /* Unlock commit register to configure PF0 */
    GPIO_PORTF_CR_R = 0x01;                     /* Make PORTF0 configurable */
    GPIO_PORTF_LOCK_R = 0;                      /* Lock commit register after configuration */
    GPIO_PORTF_DIR_R |= (1<<3);                 /* Set PF3 as digital output to control green LED */
    GPIO_PORTF_DEN_R |= (1<<4)|(1<<3)|(1<<0);   /* Enable digital function on PF4, PF3, and PF0 */
}

void main()
{
    // Inits for Program
    PLL_Init(Bus80MHz);         // set system clock to 80 MHz
    LCD_Init();
    GPIO_PORTF_INIT();
    SysTick_Init(8000000);

    while(1) {                   // Main loop

        if (mailbox == 1){
            uint32_t sample = SysTick_Mailbox();
            mailbox = 0;
            float result = Sample_to_Farenheit(sample);
            LCD_OutCmd(0x80);
            LCD_OutUFix(result);
        }

    }
}
