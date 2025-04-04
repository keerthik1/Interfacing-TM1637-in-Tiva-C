#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

// Define GPIO pins (Port B: PB0 for CLK, PB1 for DIO)
#define CLK_PIN  GPIO_PIN_0 // PB0
#define DIO_PIN  GPIO_PIN_1 // PB1
#define GPIO_PORT GPIO_PORTB_BASE

// Segment codes for digits 0-9 (common anode)
const uint8_t digitToSegment[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

// Delay function (approximate microseconds)
void delay_us(uint32_t us) {
    SysCtlDelay(us * (SysCtlClockGet() / 3000000)); // SysCtlDelay is cycles/3
}

// Initialize GPIO pins
void init_GPIO(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // Enable Port B
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)); // Wait for ready
    GPIOPinTypeGPIOOutput(GPIO_PORT, CLK_PIN | DIO_PIN); // Set PB0, PB1 as outputs
    GPIOPinWrite(GPIO_PORT, CLK_PIN | DIO_PIN, 0); // Initial state low
}

// Set DIO as input (for ACK check)
void DIO_input(void) {
    GPIOPinTypeGPIOInput(GPIO_PORT, DIO_PIN);
}

// Set DIO as output
void DIO_output(void) {
    GPIOPinTypeGPIOOutput(GPIO_PORT, DIO_PIN);
}

// TM1637 Start signal
void TM1637_start(void) {
    GPIOPinWrite(GPIO_PORT, CLK_PIN, CLK_PIN);  // CLK high
    GPIOPinWrite(GPIO_PORT, DIO_PIN, DIO_PIN);  // DIO high
    delay_us(2);
    GPIOPinWrite(GPIO_PORT, DIO_PIN, 0);        // DIO low
}

// TM1637 Stop signal
void TM1637_stop(void) {
    GPIOPinWrite(GPIO_PORT, CLK_PIN, 0);        // CLK low
    GPIOPinWrite(GPIO_PORT, DIO_PIN, 0);        // DIO low
    delay_us(2);
    GPIOPinWrite(GPIO_PORT, CLK_PIN, CLK_PIN);  // CLK high
    GPIOPinWrite(GPIO_PORT, DIO_PIN, DIO_PIN);  // DIO high
}

// Write a byte to TM1637 (LSB first)
void TM1637_writeByte(uint8_t data) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        GPIOPinWrite(GPIO_PORT, CLK_PIN, 0); // CLK low
        delay_us(2);
        if (data & 0x01) {
            GPIOPinWrite(GPIO_PORT, DIO_PIN, DIO_PIN); // DIO high
        } else {
            GPIOPinWrite(GPIO_PORT, DIO_PIN, 0);       // DIO low
        }
        delay_us(2);
        GPIOPinWrite(GPIO_PORT, CLK_PIN, CLK_PIN); // CLK high
        delay_us(2);
        data >>= 1;
    }
    // Check ACK
    GPIOPinWrite(GPIO_PORT, CLK_PIN, 0); // CLK low
    DIO_input(); // DIO as input
    delay_us(2);
    GPIOPinWrite(GPIO_PORT, CLK_PIN, CLK_PIN); // CLK high
    delay_us(2);
    GPIOPinWrite(GPIO_PORT, CLK_PIN, 0); // CLK low
    DIO_output(); // DIO back to output
}

// Display a 4-digit number
void TM1637_display(uint16_t number) {
    uint8_t digits[5];
    uint8_t i;
    digits[0] = digitToSegment[number / 1000 % 10]; // Thousands
    digits[1] = digitToSegment[number / 100 % 10] ;  // Hundreds | 0x80 for colon
    digits[2] = digitToSegment[number / 10 % 10];   // Tens
    digits[3] = digitToSegment[number % 10];        // Units

    TM1637_start();
    TM1637_writeByte(0x40); // Data command: write data
    TM1637_stop();

    TM1637_start();
    TM1637_writeByte(0xC0); // Address command: start at digit 0
    for (i = 0; i < 4; i++) {
        TM1637_writeByte(digits[i]); // Send segment data
    }
    TM1637_stop();

    TM1637_start();
    TM1637_writeByte(0x8F); // Display control: turn on, max brightness
    TM1637_stop();
}

int main(void) {
    // Set system clock to 16 MHz (default for TM4C123G)
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    init_GPIO(); // Initialize GPIO pins
    int i;
    while (1) {
        for(i=0;i<9999;i++)
        {
            TM1637_display(i); // Display "1234"
            SysCtlDelay(1000000); // Delay ~250ms at 16 MHz
        }

    }
}
