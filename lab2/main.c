#include <avr/io.h>
#include <util/delay.h>

#define SECOND_BUTTON PB2

uint8_t is_run = 1, double_click_count = 0;
void key_handler(uint8_t ks);
uint8_t second_button_state = 0;
void running_lights(void);

void scan_key(void)
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

    shreg <<= 1;
    if ((PINB & (1 << SECOND_BUTTON)) != 0) {
        shreg |= 1;
    }

    // Long press for the second button
    if (shreg == 0x80) {
        key_handler(3); // Long press for the second button
    }
}


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

void key_management(void)
{
    // This function is called in the main loop to manage keys
    scan_key(); // Call the existing scan_key function
}
//-----------------------------------------------------------


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
void key_handler(uint8_t ks)
{
    if (ks == 1) { // short key up
        is_run = (is_run == 0)? 1: 0;
        running_lights();
    } else if (ks == 2) {  //long key press
        is_run = 1;
        opposite_edges_animation();
    }
}
//-----------------------------------------------------------
int main(void)
{
   // Налаштування портів вводу-виводу
    DDRD = 0xFF; // Всі лінії порта D виходи
    DDRB = 0; // Всі лінії порта B входи
    UCSR0B = 0;

   
    while (1) {
        
        // Key management thread
        scan_key();

        // Delay for 10 ms
        _delay_ms(10);
    }
    return 0;
}