#include <avr/io.h>
#include <util/delay.h>

#define SECOND_BUTTON PB2

uint8_t direction = 0, data = 1, is_run = 1, double_click_count = 0;
void key_handler(uint8_t ks);
uint8_t second_button_state = 0;

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


void running_lights(uint8_t* direction, uint8_t* data)
{
    static uint8_t dl = 0;
    static uint8_t local_direction = 0;
    static uint8_t local_data = 1;

    if (is_run) {
        if (++dl > 5) {
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

        *direction = local_direction; // Update the direction variable
        *data = local_data;           // Update the data variable
    }
}


void opposite_edges_animation(void)
{
    // New animation: LEDs turn on and off from two edges in the opposite order
    static uint8_t dl = 0;
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
void key_handler(uint8_t ks)
{
    if (ks == 1) { // Short key up for the first button
        is_run = (is_run == 0) ? 1 : 0;
        running_lights(0, 1);
    } else if (ks == 2) {  // Long key press for the first button
        direction ^= 1;
        is_run = 1;
        running_lights(0, 1);
    } else if (ks == 3) { // Long key press for the second button
        opposite_edges_animation(); // Call the new animation function
    }
}

//-----------------------------------------------------------
int main(void)
{
    uint8_t dl = 0;
    // Налаштування портів вводу-виводу
    DDRD = 0xFF; // Всі лінії порта D виходи
    DDRB = 0; // Всі лінії порта B входи
    UCSR0B = 0;
    // Initialization code

   
    for (;;) {
        // LED animation thread
        running_lights(0, 1);

        // Key management thread
        key_management();

        // Delay for 10 ms
        _delay_ms(10);
    }
    return 0;
}