/*
 * trackpoint.c
 *
 * Created: 01/05/2016
 * Author: zerone
 *
 * https://github.com/rampadc/arduino-trackpoint-extended/blob/master/TrackPoint.cpp
 * Convert class methods to functions for tmk env
 *
 */


#include <avr/io.h>
#include <util/delay.h>
#include "trackpoint.h"

uint8_t getClockState(void) {
    return PIND & (1<<0);
}

uint8_t getDataState(void) {
    return PIND & (1<<1);
}

void setClockState(uint8_t state) {
    // PIN_D0 CLOCK
    if (state == HIGH) {
        DDRD &= ~(1<<0);
        PORTD |= (1<<0);
    } else {
        DDRD |= (1<<0);
        PORTD &= ~(1<<0);
    }
}

void setDataState(uint8_t state) {
    // PIN_D1 DATA
    if (state) {
        DDRD &= ~(1<<1);
        PORTD |= (1<<1);
    } else {
        DDRD |= (1<<1);
        PORTD &= ~(1<<1);
    }
}

void trackpoint_reset(Trackpoint* tp) {
    // PIN_D2 RST
    DDRD |= (1<<2);
    PORTD |= (1<<2);
    _delay_ms(2000); // empirical value
    PORTD &= ~(1<<2);
}

void getDataBit(Trackpoint* tp) {
    tp->dataAvailable = 0;
    tp->val = (getDataState() ? 1 : 0);
    tp->n = tp->bitcount-1;
    if(tp->n <= 7) {
        tp->incoming |= (tp->val << tp->n);
    }
    tp->bitcount++;
    if(tp->bitcount == 11) {
        switch(tp->counter) {
            case 0:
                tp->data.state = tp->incoming;
                tp->counter++;
                break;

            case 1:
                tp->data.x = tp->incoming;
                tp->counter++;
                break;

            case 2:
                tp->data.y = tp->incoming;
                tp->counter = 0;
                tp->dataAvailable = 1;
                break;
        }
        tp->bitcount = 0;
        tp->incoming = 0;
    }
}

DataReport getStreamReport(Trackpoint* tp) {
    return tp->data;
}

uint8_t trackpoint_read(Trackpoint* tp) {
    uint8_t data = 0x00;
    uint8_t i;
    uint8_t bit = 0x01;
    // start clock
    setClockState(HIGH);
    setDataState(HIGH);
    //delayMicroseconds(50);
    _delay_us(50);
    while (getClockState() == HIGH)
        ;
    //delayMicroseconds(5);   // not sure why.
    _delay_us(5);
    while (getClockState() == LOW)
        ;   // eat start bit
    for (i=0; i < 8; i++)
    {
        while (getClockState() == HIGH)
            ;
        if (getDataState() == HIGH) {
            data = data | bit;
        }
        while (getClockState() == LOW)
            ;
        bit = bit << 1;
    }
    // eat parity bit, ignore it.
    while (getClockState() == HIGH)
        ;
    while (getClockState() == LOW)
        ;
    // eat stop bit
    while (getClockState() == HIGH)
        ;
    while (getClockState() == LOW)
        ;
    setClockState(LOW);  // hold incoming data

    return data;
}

uint8_t reportAvailable(Trackpoint* tp) {
    return tp->dataAvailable;
}

void trackpoint_setup(Trackpoint* tp) {
    tp->bitcount = 0;
    tp->n = 0;
    tp->val = 0;
    tp->incoming = 0;
    tp->counter = 0;
    tp->dataAvailable = 0;
    setClockState(HIGH);
    setDataState(HIGH);
}

void trackpoint_write(Trackpoint* tp, uint8_t data) {
    uint8_t i;
    uint8_t parity = 1;
    setDataState(HIGH);
    setClockState(HIGH);
    //delayMicroseconds(300);
    _delay_us(300);
    setClockState(LOW);
    //delayMicroseconds(300);
    _delay_us(300);
    setDataState(LOW);
    setClockState(HIGH);  // start bit
    //delayMicroseconds(10);
    _delay_us(10);
    /* wait for device to take control of clock */
    while (getClockState() == HIGH)
        ;   // this loop intentionally left blank
    // clear to send data
    for (i=0; i < 8; i++) {
        if (data & 0x01) {
            setDataState(HIGH);
        } else {
            setDataState(LOW);
        }
        // wait for clock
        while (getClockState() == LOW)
            ;
        while (getClockState() == HIGH)
            ;
        parity = parity ^ (data & 0x01);
        data = data >> 1;
    }
    // parity bit
    if (parity) {
        setDataState(HIGH);
    } else {
        setDataState(LOW);
    }
    // clock cycle - like ack.
    while (getClockState() == LOW)
        ;
    while (getClockState() == HIGH)
        ;
    // stop bit
    setDataState(HIGH);
    _delay_us(50); //delayMicroseconds(50);
    while (getClockState() == HIGH)
        ;
    // mode switch
    while ((getClockState() == LOW) || (getDataState() == LOW))
        ;
    // hold up incoming data
    setClockState(LOW);
}

void setStreamMode(Trackpoint* tp) {
    trackpoint_write(tp, 0xea);
    trackpoint_read(tp);
    trackpoint_write(tp, 0xf4); //enable report
    trackpoint_read(tp);
    //put mouse into idle mode, ready to send
    setClockState(HIGH);
    setDataState(HIGH);
}

void writeToRamLocation(Trackpoint* tp, uint8_t location, uint8_t value) {
    trackpoint_write(tp, 0xe2);
    trackpoint_read(tp); // ACK
    trackpoint_write(tp, 0x81);
    trackpoint_read(tp); // ACK
    trackpoint_write(tp, location);
    trackpoint_read(tp); // ACK
    trackpoint_write(tp, value);
    trackpoint_read(tp); // ACK
}

uint8_t readFromRamLocation(Trackpoint* tp, uint8_t location) {
    trackpoint_write(tp, 0xe2);
    trackpoint_read(tp); // ACK
    trackpoint_write(tp, 0x80);
    trackpoint_read(tp); // ACK
    trackpoint_write(tp, location);
    trackpoint_read(tp); // ACK
    return trackpoint_read(tp);
}

void setSensitivityFactor(Trackpoint* tp, uint8_t sensitivityFactor) {
    writeToRamLocation(tp, 0x4a, sensitivityFactor);
}

uint8_t sensitivityFactor(Trackpoint* tp) {
    return readFromRamLocation(tp, 0x4a);
}
