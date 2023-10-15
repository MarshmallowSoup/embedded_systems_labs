#include <avr/io.h>
#include <util/delay.h>

uint8_t direction = 0, data = 1, is_run = 1;
void key_handler(uint8_t ks);

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
}
//-----------------------------------------------------------
void key_handler(uint8_t ks)
{
    if (ks == 1) { // short key up
        is_run = (is_run == 0)? 1: 0;
        
    } else if (ks == 2) {  //long key press
        direction ^=1;
        is_run = 1;
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

    for (;;) {
        // ~200 ms thread -----
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
        // 10 ms thread ---------
        scan_key();
        _delay_ms(10);
    }
    return 0;
}