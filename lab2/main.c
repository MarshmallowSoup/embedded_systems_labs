#include <avr/io.h>
#include <util/delay.h>

#define NUM_LEDS 6  // Number of LEDs
#define DELAY_MS 100 // Delay between LED transitions
#define BUTTON_PIN PD2 // Pin for the button
#define LED_PORT PORTD

int ledPins[NUM_LEDS] = {PD7, PD6, PD5, PD4, PB0, PB1};  // Pins for the LEDs
int currentEffect = 0;  // Current selected effect

void initializeLEDs() {
    for (int i = 0; i < NUM_LEDS; i++) {
        // Set the LED pins as outputs
        DDRD |= (1 << ledPins[i]);
    }
}

void runningLight() {
    for (int i = 0; i < NUM_LEDS; i++) {
        // Turn on the i-th LED
        PORTD |= (1 << ledPins[i]);
        _delay_ms(DELAY_MS);
        // Turn off the i-th LED
        PORTD &= ~(1 << ledPins[i]);
    }

    for (int i = NUM_LEDS - 1; i >= 0; i--) {
        // Turn on the i-th LED
        PORTD |= (1 << ledPins[i]);
        _delay_ms(DELAY_MS);
        // Turn off the i-th LED
        PORTD &= ~(1 << ledPins[i]);
    }
}

void shadow() {
    for (int i = 0; i < NUM_LEDS; i++) {
        // Turn on the i-th LED
        PORTD |= (1 << ledPins[i]);
        _delay_ms(DELAY_MS);
        // Turn off the i-th LED
        PORTD &= ~(1 << ledPins[i]);
    }

    for (int i = NUM_LEDS - 1; i >= 0; i--) {
        // Turn on the i-th LED
        PORTD |= (1 << ledPins[i]);
        _delay_ms(DELAY_MS);
        // Turn off the i-th LED
        PORTD &= ~(1 << ledPins[i]);
    }
}

void changeEffect() {
    currentEffect = (currentEffect + 1) % 2;  // Toggle between effects (assuming 2 effects for simplicity)
}

int main(void) {
    // Set the LED pins and button pin as outputs and inputs
    DDRD &= ~(1 << BUTTON_PIN);  // Set the button pin as an input
    initializeLEDs();  // Initialize LED pins

    int buttonState = 0;  // 0 for button released, 1 for button pressed
    int prevButtonState = 0;
    unsigned long buttonPressStartTime = 0;

    while (1) {
        buttonState = PIND & (1 << BUTTON_PIN);

        if (buttonState == 0 && prevButtonState == 1) {
            // Button released after a long press (2 seconds)
            unsigned long buttonPressDuration = millis() - buttonPressStartTime;
            if (buttonPressDuration >= 2000) {
                changeEffect();  // Change the effect
            }
        } else if (buttonState == 1 && prevButtonState == 0) {
            // Button pressed
            buttonPressStartTime = millis();
        }

        // Run the selected effect
        if (currentEffect == 0) {
            runningLight();
        } else {
            shadow();
        }

        prevButtonState = buttonState;
    }

    return 0;
}