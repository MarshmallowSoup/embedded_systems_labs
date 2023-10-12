#include <avr/io.h>
#include <util/delay.h>
#define LED_PIN PB5 // Built-in LED on Arduino Nano

void flashDot()
{
    PORTB |= 1 << LED_PIN; 
    _delay_ms(200); 
    PORTB &= ~(1 << LED_PIN); 
    _delay_ms(200);
}

void flashDash() 
{
    PORTB |= 1 << LED_PIN; 
    _delay_ms(600); 
    PORTB &= ~(1 << LED_PIN); 
    _delay_ms(200); 
}

void blink_msg(char *msg)
{
    while(*msg) {
        // ----
        if(*msg == "-"){
            flashDash();
        }
        else if(*msg == "."){
            flashDot();
        }
        else if(*msg == " "){
            _delay_ms(400);
        }
        else{
            break;
        }
        msg++;
    }

    _delay_ms(2000);
}

int main(void) {
    DDRB |= 1 << LED_PIN;


    blink_msg("... --- ...");

        // Turn off the LED
    PORTB &= ~(1 << LED_PIN);
    

    return 0;
}
