#include <avr/io.h>
#include <util/delay.h>

uint8_t direction = 0, data = 1, is_run = 1, double_click_count = 0;
uint8_t effect_index = 0;
void key_handler(uint8_t ks);
uint8_t second_button_state = 0;
uint8_t speed = 10;

void scan_key(void)
{
    static uint8_t shreg = 0xFF;
    static uint8_t long_press_cnt = 0;

    shreg <<= 1;
    if ((PINC & (1 << PC5)) != 0) {
        shreg |= 1;
    }
    // push key
    if ((shreg & 0x07) == 0x04) {
        long_press_cnt = 0;
        key_handler(0);
    }
    // up key
    if ((shreg & 0x0F) == 0x03) {
        if(long_press_cnt < 100){
            key_handler(1);
        }
    }
    if (shreg == 0){
        if(long_press_cnt < 254){
            long_press_cnt++;
        }
        if (long_press_cnt == 100) {
            key_handler(2); //long press
        } 
    }
}

void opposite_edges_animation()
{
    // New animation: LEDs turn on and off from two edges in the opposite order
    static uint8_t dl = 0;
    if (is_run) {
        if (++dl > speed) {
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


void light_up_one_by_one()
{
    static uint8_t dl = 0;
    static uint8_t local_direction = 0;
    static uint8_t local_data = 1;

    if (is_run) {
        if (++dl > speed) { // Delay for 1 second (50 * 20ms)
            dl = 0;

            PORTD = ~local_data;

            if (local_direction == 0) {
                local_data <<= 1;
            } else {
                local_data >>= 1;
            }

            if (local_data == 0) {
                if (local_direction == 0) {
                    local_data = 1;
                } else {
                    local_data = 128;
                }
            }
        }
    }
}

void running_lights()
{
    static uint8_t dl = 0;
    static uint8_t local_direction = 0;
    static uint8_t local_data = 1;

    if (is_run) {
        if (++dl > speed) {
            dl = 0;
            PORTD = ~local_data;

            if (local_direction == 0) {
                local_data <<= 1;
            } else {
                local_data >>= 1;
            }

            if (local_data == 0) {
                if (local_direction == 0) {
                    local_data = 1;
                } else {
                    local_data = 128;
                }
            }
        }
    }
}


void even_odd_light_sequence()
{
    static uint8_t dl = 0;
    static uint8_t local_direction = 0;
    static uint8_t local_data = 0b01010100; 
    if (is_run) {
        if (++dl > 10) {
            dl = 0;

            PORTD = ~local_data;

            if (local_direction == 0) {
                local_data <<= 1;
                if (local_data == 0) {
                    local_data = 0b10101000; 
                    local_direction = 1; 
                }
            } else {
                local_data <<= 1;
                if (local_data == 0) {
                    local_data = 0b01010100;
                    local_direction = 0; 
                }
            }
        }
    }
}
//-----------------------------------------------------------
void key_handler(uint8_t ks)
{
    if (ks == 1) { // Short key up for the first button
        effect_index++;
        if(effect_index >3){
            effect_index = 0;
        }
    } else if (ks == 2) {  // Long key press for the first button
        speed += 10;
        if (speed > 50){
            speed = 10;
        }
    }   
}

//-----------------------------------------------------------
int main(void)
{ 
    DDRC = 0;
    DDRD = 255;
    UCSR0B = 0;

    for (;;) {
        scan_key();
        _delay_ms(10);
        switch (effect_index) {
            case 0:
                opposite_edges_animation();
                break;
            case 1:
                light_up_one_by_one();
                break;
            case 2:
                running_lights();
                break;
            case 3:
                even_odd_light_sequence();
                break;
            default:
                break;
        }
    }
    return 0;
}