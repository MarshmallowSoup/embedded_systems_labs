#include <avr/io.h>
#include <util/delay.h>

#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12

int step_number = 0;

void setup() {
    // Set the stepper motor pins as outputs
    DDRB |= (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4);
}

void loop() {
    OneStep(0);
    _delay_ms(2);
}

void OneStep(int dir) {
    if (dir) {
        switch (step_number) {
            case 0:
                PORTB = (1 << PB1);
                break;
            case 1:
                PORTB = (1 << PB2);
                break;
            case 2:
                PORTB = (1 << PB3);
                break;
            case 3:
                PORTB = (1 << PB4);
                break;
        }
    } else {
        switch (step_number) {
            case 0:
                PORTB = (1 << PB4);
                break;
            case 1:
                PORTB = (1 << PB3);
                break;
            case 2:
                PORTB = (1 << PB2);
                break;
            case 3:
                PORTB = (1 << PB1);
                break;
        }
    }

    step_number++;
    if (step_number > 3) {
        step_number = 0;
    }
}

int main(void) {
    setup();

    while (1) {
        loop();
    }

    return 0;
}
