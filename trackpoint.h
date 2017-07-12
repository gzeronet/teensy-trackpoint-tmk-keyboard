/*
 * trackpoint.h
 *
 * Created: 13/02/2016
 * Author: zerone
 *
 * https://github.com/rampadc/arduino-trackpoint-extended/blob/master/TrackPoint.h
 * Convert class to struct for tmk env
 *
 */


#define PIN_CLOCK_INT   0  // PIN_B0 SS
#define HIGH 0x1
#define LOW  0x0

typedef struct {
    uint8_t state;
    int8_t x;
    int8_t y;
} DataReport;

typedef struct {
    DataReport data;
    // getDataBit() variables
    volatile uint8_t bitcount;
    volatile uint8_t n;
    volatile uint8_t val;
    volatile uint8_t incoming;
    volatile uint8_t counter;
    volatile uint8_t dataAvailable;
    uint8_t usingSeparateResetPin;
} Trackpoint;

uint8_t getClockState(void);
uint8_t getDataState(void);
void setClockState(uint8_t state);
void setDataState(uint8_t state);
void trackpoint_setup(Trackpoint* tp);
void trackpoint_reset(Trackpoint* tp);
void getDataBit(Trackpoint* tp);
uint8_t trackpoint_read(Trackpoint* tp);
uint8_t reportAvailable(Trackpoint* tp);
void trackpoint_write(Trackpoint* tp, uint8_t data);
void setStreamMode(Trackpoint* tp);
void writeToRamLocation(Trackpoint* tp, uint8_t location, uint8_t value);
uint8_t readFromRamLocation(Trackpoint* tp, uint8_t location);
void setSensitivityFactor(Trackpoint* tp, uint8_t sensitivityFactor);
uint8_t sensitivityFactor(Trackpoint* tp);
DataReport getStreamReport(Trackpoint* tp);
