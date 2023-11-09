#include <avr/io.h>
#include <util/delay.h>

#define EXTERNAL_LED_PIN PD6 // Pin for the external LED
#define BUTTON_PIN PD2 //

int main(void) {
    // Set PB2 as input (button) and PB7 as output (LED)
    DDRB &= ~(1 << BUTTON_PIN); // Set PB2 as input
    DDRB |= (1 << EXTERNAL_LED_PIN);  // Set PB7 as output

    //PORTB |= (1 << PB2);  // Enable the internal pull-up resistor for PB2

    while (1) {
        // Check if the button is pressed (logic low)
        if (!(PINB & (1 << PB2))) {
            // Button is pressed, turn on the LED
            PORTB |= (1 << PB7);
        } else {
            // Button is not pressed, turn off the LED
            PORTB &= ~(1 << PB7);
        }
    }

    return 0;
}