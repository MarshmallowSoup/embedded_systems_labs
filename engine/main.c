#include <avr/io.h>
#include <util/delay.h>

#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12

#define START_BUTTON_PIN PD2
#define STOP_BUTTON_PIN PD3

int step_number = 0;

void setup() {
    // Set the stepper motor pins as outputs
    DDRB |= (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4);

    // Set the button pins as inputs
    DDRD &= ~(1 << START_BUTTON_PIN);
    DDRD &= ~(1 << STOP_BUTTON_PIN);

    // Enable internal pull-up resistors for the button pins
    PORTD |= (1 << START_BUTTON_PIN) | (1 << STOP_BUTTON_PIN);
}

void loop() {
    if (bit_is_clear(PIND, START_BUTTON_PIN)) {
        // Start the motor when the start button is pressed
        OneStep(0);
        _delay_ms(2);
    }

    if (bit_is_clear(PIND, STOP_BUTTON_PIN)) {
        // Stop the motor when the stop button is pressed
        PORTB &= ~((1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4));
    }
}

void OneStep(int dir) {
    // Motor stepping logic remains the same
    // ...

    // The following switch-case block is unchanged from the previous version
}

int main(void) {
    setup();

    while (1) {
        loop();
    }

    return 0;
}
