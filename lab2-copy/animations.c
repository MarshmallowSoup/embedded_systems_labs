#include <avr/io.h>
#include <util/delay.h>

uint8_t is_run = 1;

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

void fill_to_middle_animation(void)
{
    // LEDs light up from two sides and fill to the middle
    static uint8_t dl = 0;
    static uint8_t data = 3; // Initial pattern with two LEDs at each end

    if (is_run) {
        if (++dl > 5) {
            dl = 0;
            PORTD = data;
            data = (data << 1) | (data >> 1); // Shift and wrap-around to create the filling effect
            if (data == 255) {
                data = 3; // Start over when data is reset
            }
        }
    }
}