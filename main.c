//Raspberry Pi Pico CC 4-digit 7 Segment Display Example
//Device: RP2040, HS420561K-32 (CC 7-Segment Display)
//File: main.c
//Author: Mike Kushnerik
//Date: 7/15/2022

//Note: This is written for Common Cathode 7-segment displays!
//      For Common Anode, simply switch the output logic :)

//      Additionally, be sure to use current limiting resistors on each segment pin
//      Good value (assuming roughly 2V forward voltage): 1K
//      If using a value lower than this, be sure to use gpio_set_drive_strength()

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

//7 segment wiring (adjust as needed):
#define a_pin       6
#define b_pin       7
#define c_pin       8
#define d_pin       9
#define e_pin       10
#define f_pin       11
#define g_pin       12
#define digit_1     13
#define digit_2     14
#define digit_3     15
#define digit_4     16

//create mask for segment pins
const uint32_t segment_mask =   (1 << a_pin) |
                                (1 << b_pin) |
                                (1 << c_pin) |
                                (1 << d_pin) |
                                (1 << e_pin) |
                                (1 << f_pin) |
                                (1 << g_pin);

//create mask for digit pins
const uint32_t digit_mask =     (1 << digit_1) |
                                (1 << digit_2) |
                                (1 << digit_3) |
                                (1 << digit_4);

//LUT for segment pins
//i.e. segment_pins[5] will display the value 5 on the display digit
const uint32_t segment_pins[10] =
{
    //0
    ((1<<a_pin) | (1<<b_pin) | (1<<c_pin) | (1<<d_pin) | (1<<e_pin) | (1<<f_pin)),
    //1
    ((1<<b_pin) | (1<<c_pin)),
    //2
    ((1<<a_pin) | (1<<b_pin) | (1<<d_pin) | (1<<e_pin) | (1<<g_pin)),
    //3
    ((1<<a_pin) | (1<<b_pin) | (1<<c_pin) | (1<<d_pin) | (1<<g_pin)),
    //4
    ((1<<b_pin) | (1<<c_pin) | (1<<f_pin) | (1<<g_pin)),
    //5
    ((1<<a_pin) | (1<<c_pin) | (1<<d_pin) | (1<<f_pin) | (1<<g_pin)),
    //6
    ((1<<a_pin) | (1<<c_pin) | (1<<d_pin) | (1<<e_pin) | (1<<f_pin) | (1<<g_pin)),
    //7
    ((1<<a_pin) | (1<<b_pin) | (1<<c_pin)),
    //8
    ((1<<a_pin) | (1<<b_pin) | (1<<c_pin) | (1<<d_pin) | (1<<e_pin) | (1<<f_pin) | (1<<g_pin)),
    //9
    ((1<<a_pin) | (1<<b_pin) | (1<<c_pin) | (1<<d_pin) | (1<<f_pin) | (1<<g_pin))
};

//LUT for digit pins
//i.e. digit_pins[0] is the most significant digit of the display
uint32_t digit_pins[4] =
{
    digit_1,
    digit_2,
    digit_3,
    digit_4
};

uint32_t counter = 0;

//function prototypes
void init_display(void);
void update_display(uint32_t value);
bool counter_cb(struct repeating_timer *t);

int main() 
{
    stdio_init_all();
    init_display();

    //timer to increment counter every second
    struct repeating_timer timer;
    add_repeating_timer_ms(1000, counter_cb, NULL, &timer);    

    while(1)
    {
        //must run continuously
        update_display(counter);
    }

    return 0;
}

//function to init the 7 segment display
void init_display(void)
{
    //init 7 segment output pins
    gpio_init_mask(segment_mask | digit_mask);
    //set pins as output
    gpio_set_dir_out_masked(segment_mask | digit_mask);

}

//function to update the value of the 7 segment display
void update_display(uint32_t value)
{
    //turn all digits off
    gpio_put_masked(digit_mask, digit_mask);
    //turn all segments off
    gpio_clr_mask(segment_mask);

    //loop to sequentially drive each digit
    for(int i = 0; i < 4; i++)
    {
        //turn all digits off
        gpio_put_masked(digit_mask, digit_mask);
        //select digit
        gpio_put(digit_pins[3-i], 0);
        //write digit value
        gpio_put_masked(segment_mask, segment_pins[value % 10]);
        //move to next digit
        value /= 10;
        //2000us seems to work well :p
        sleep_us(2000);
    }
}

//timer interrupt callback to increment counter
bool counter_cb(struct repeating_timer *t)
{
    counter++;

    //check if we have run out of digits 
    if(counter > 9999)
    {
        counter = 0;
    }

    return true;
}