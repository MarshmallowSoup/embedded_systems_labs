#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define LED_PIN PB1 // вбудований в Arduino nano світлодіод

uint8_t direction = 0, data = 1, is_run = 1;
void key_handler(uint8_t ks);
void key_handler2(uint8_t ks);

void running_lights(void)
{
    static uint8_t direction = 0, data = 1;
    static uint8_t dl = 0;
    if (is_run) {
        if (++dl > 5) {
            dl = 0;
            PORTD = ~data;
            if (direction == 0) {
                data <<= 1;
            } else {
                data >>= 1;
            }
            if (data == 0) {
                if (direction == 0) {
                    data = 1;
                } else {
                    data = 128;
                }
            }
        }
    }
}
void opposite_edges_animation(void)
{ 
    uint8_t direction = 0, data = 1;
    // New animation: LEDs turn on and off from two edges in the opposite order
    uint8_t dl = 0;
    if (is_run) {
        if (++dl > 5) {
            dl = 0;
            if (direction == 0) {
                PORTD = data;
                data <<= 1;
            } else {
                PORTD = data;
                data >>= 1;
            }
            if (data == 0) {
                direction ^= 1;
                if (direction == 0) {
                    data = 1;
                } else {
                    data = 128;
                }
            }
        }
    }
}

void scan_key1(void)
{
    static uint8_t shreg = 0xFF;
    static uint8_t long_press_cnt = 0;

    shreg <<= 1;
    if((PINB & (1 << PB1)) != 0) {
        shreg |= 1;
    }
    // push key
    if((shreg & 0x07) == 0x04) {
        long_press_cnt = 0;
        key_handler(0);
    }
    // up key
    if((shreg & 0x0F) == 0x03) {
        if(long_press_cnt < 100){
        key_handler(1);
        }
    }
    if(shreg == 0){
        if(long_press_cnt < 254){
            long_press_cnt++;
        }
        if (long_press_cnt == 100) {
            key_handler(2); //long press
        } 
    }
}

void scan_key2(void)
{
}
//-----------------------------------------------------------
void key_handler(uint8_t ks)
{
    if (ks == 1) { // short key up
        is_run = (is_run == 0)? 1: 0;
        
    } else if (ks == 2) {  //long key press
        opposite_edges_animation();
        is_run = 1;
    }
}

void key_handler2(uint8_t ks)
{
    if (ks == 1) { // short key up
        is_run = (is_run == 0)? 1: 0;
        
    } else if (ks == 2) {  //long key press
        direction ^=1;
        is_run = 1;
    }
}

void initInterrupts()
{
    // Set PB1 (pin 15) as interrupt source for button 1
    EICRA |= (1 << ISC00);
    EIMSK |= (1 << INT0);

    // Set PB2 (pin 16) as interrupt source for button 2
    EICRA |= (1 << ISC10);
    EIMSK |= (1 << INT1);

    sei(); // Enable global interrupts
}

// Interrupt Service Routine for button 1
ISR(INT0_vect)
{
    key_handler(0); // Call key_handler with appropriate parameter for button 1
}

// Interrupt Service Routine for button 2
ISR(INT1_vect)
{
    key_handler2(0); // Call key_handler2 with appropriate parameter for button 2
}
int main(void)
{
    uint8_t dl = 0;

    DDRD = 0xFF;
    DDRB = 0;
    UCSR0B = 0;

    initInterrupts();

    for (;;)
    {
        // Handle button presses in the main loop
        scan_key1();
        scan_key2();

        // 10 ms thread
        _delay_ms(10);
    }

    return 0;
}