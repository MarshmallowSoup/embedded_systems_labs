#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PB5 // buildin led

int main(void)
{
    DDRB |= 1 << LED_PIN; 

    while (1) 
    {
        PORTB |= 1 << LED_PIN;    // включити світлодіод
        _delay_ms(100);           // затримка
        PORTB &= ~(1 << LED_PIN); // виключити світлодіод
        _delay_ms(400);     
    }

  return 0;
}