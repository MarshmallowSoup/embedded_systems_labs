#include <avr/io.h>
#include <util/delay.h>
#define LED_PIN PB5 // Built-in LED on Arduino Nano

void flashDot() {
    PORTB |= 1 << LED_PIN; // Turn on the LED (dot)
    _delay_ms(200); // Dot duration
    PORTB &= ~(1 << LED_PIN); // Turn off the LED
    _delay_ms(200); // Gap between dots and dashes
}

void flashDash() {
    PORTB |= 1 << LED_PIN; // Turn on the LED (dash)
    _delay_ms(600); // Dash duration
    PORTB &= ~(1 << LED_PIN); // Turn off the LED
    _delay_ms(200); // Gap between dots and dashes
}

int main(void) {
    DDRB |= 1 << LED_PIN;

    while (1) {
        // "S" in Morse code: dot-dot-dot
        for (int i = 0; i < 3; i++) {
            flashDot();
        }
        _delay_ms(400); // Gap between letters

        // "O" in Morse code: dash-dash-dash
        for (int i = 0; i < 3; i++) {
            flashDash();
        }
        _delay_ms(400); // Gap between letters

        // "S" in Morse code: dot-dot-dot
        for (int i = 0; i < 3; i++) {
            flashDot();
        }
        
        // Delay before shutting down the LED
        _delay_ms(2000);

        // Turn off the LED
        PORTB &= ~(1 << LED_PIN);
    }

    return 0;
}
