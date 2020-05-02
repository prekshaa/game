#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "tty.h"
#include "snake.h"

void TIM2_IRQHandler(void)
{
    TIM2->SR &= ~0x1;
    animate();
}

//===================================
// Disable timer 2.  Wait for a key press.
void freeze(void)
{
    TIM2->CR1 &= ~TIM_CR1_CEN;
    while(!available())
        ;
    getchar();
    TIM2->CR1 |= TIM_CR1_CEN;
}

int get_seed(void)
{
    return TIM2->CNT;
}

//============================================================================
// Enable clock to timer2.
// Setup prescalar and arr so that the interrupt is triggered every 125 ms.
// Enable the timer 2 interrupt.
// Start the timer.
void setup_timer2() {
    // Enable timer 2 here.
    // Make it generate an interrupt 8 times per second.
    // Set the prescaler to 48 so that the free-running
    // counter goes from 0 ... 124999.
    RCC->APB1ENR &= ~(1);
    RCC->APB1ENR |= (1);
    TIM2->PSC = 48-1; //(48000000)/10E-6 = 48
    TIM2->ARR = 125000-1;
    TIM2->DIER &= ~(1);
    TIM2->DIER |= (1);
    NVIC->ISER[0] = (1<<TIM2_IRQn);
    TIM2->CR1 &= ~(1);
    TIM2->CR1 |= (1);

    //... You finish this part

    // Also, make sure TIM2 has lower priority than USART1
    // by using the NVIC_SetPriority() function.  Use a higher
    // number (lower priority) than you did for USART1.
    NVIC_SetPriority(TIM2_IRQn,2);
}

int main(void)
{

    tty_init();
    raw_mode();
    cursor_off();
    init();
    splash();
    setup_timer2();
    for(;;)
        asm("wfi");
    return 0;
}
