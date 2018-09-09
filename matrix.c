/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"


#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);


void matrix_init(void)
{
    // initialize row and col
    unselect_rows();
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        _delay_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

/* Column pin configuration
 * col: 0   1   2   3   4   5   6   7   8   9   10  11  12
 * pin: B1  B2  B3  B7  B6  B5  B4  D7  D3  C6  C7  D5  D4  (Rev.TP)
 */
static void  init_cols(void)
{
    // Input with pull-up(DDR:0, PORT:1)
    DDRB  &= ~(1<<7 | 1<<6 | 1<< 5 | 1<<4 | 1<<3 | 1<<2 | 1<<1);
    PORTB |=  (1<<7 | 1<<6 | 1<< 5 | 1<<4 | 1<<3 | 1<<2 | 1<<1);
    DDRC  &= ~(1<<7 | 1<<6);
    PORTC |=  (1<<7 | 1<<6);
    DDRD  &= ~(1<<7 | 1<<5 | 1<<4 | 1<<3);
    PORTD |=  (1<<7 | 1<<5 | 1<<4 | 1<<3);
}

static matrix_row_t read_cols(void)
{
    return (PINB&(1<<1) ? 0 : (1<<0)) |
           (PINB&(1<<2) ? 0 : (1<<1)) |
           (PINB&(1<<3) ? 0 : (1<<2)) |
           (PINB&(1<<7) ? 0 : (1<<3)) |
           (PINB&(1<<6) ? 0 : (1<<4)) |
           (PINB&(1<<5) ? 0 : (1<<5)) |
           (PINB&(1<<4) ? 0 : (1<<6)) |
           (PIND&(1<<7) ? 0 : (1<<7)) |
           (PIND&(1<<3) ? 0 : (1<<8)) |
           (PINC&(1<<6) ? 0 : (1<<9)) |
           (PINC&(1<<7) ? 0 : (1<<10)) |
           (PIND&(1<<5) ? 0 : (1<<11)) |
           (PIND&(1<<4) ? 0 : (1<<12));
}

/* Row pin configuration
 * row: 0   1   2   3   4
 * pin: F1  F4  F5  F6  F7 (Rev.TP)
 */
static void unselect_rows(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRF  &= ~0b11110010;
    PORTF &= ~0b11110010;
}

static void select_row(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
    switch (row) {
        case 0:
            DDRF  |= (1<<1);
            PORTF &= ~(1<<1);
            break;
        case 1:
            DDRF  |= (1<<4);
            PORTF &= ~(1<<4);
            break;
        case 2:
            DDRF  |= (1<<5);
            PORTF &= ~(1<<5);
            break;
        case 3:
            DDRF  |= (1<<6);
            PORTF &= ~(1<<6);
            break;
        case 4:
            DDRF  |= (1<<7);
            PORTF &= ~(1<<7);
            break;
    }
}
