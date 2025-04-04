# TM1637 Interfacing with TIVA C LaunchPad

## Overview
This project demonstrates interfacing the TM1637 4-digit 7-segment display driver with the TIVA C LaunchPad (EK-TM4C123GXL, TM4C123GH6PM microcontroller). The TM1637 is used to display numbers, characters, and custom patterns, controlled via a 2-wire communication protocol (CLK and DIO pins). The project showcases embedded systems skills, including GPIO configuration, bit-banging for communication, and precise timing control.

### Features
- Displays 4-digit numbers (e.g., 1234) and custom characters on a 7-segment display.
- Implements a software-based 2-wire protocol to communicate with the TM1637.
- Configures the TIVA C LaunchPad’s system clock to 16 MHz for accurate timing.
- Includes brightness control and display refresh functionality.

## Hardware Requirements
- **TIVA C LaunchPad**: EK-TM4C123GXL with TM4C123GH6PM microcontroller.
- **TM1637 4-Digit 7-Segment Display Module**: Commonly available as a pre-assembled module.
- **Jumper Wires**: For connecting the TM1637 to the LaunchPad.
- **Connections**:
  - TM1637 CLK → TIVA GPIO (e.g., PB0)
  - TM1637 DIO → TIVA GPIO (e.g., PB1)
  - TM1637 VCC → 3.3V (or 5V, depending on module)
  - TM1637 GND → GND

## Software Requirements
- **Code Composer Studio (CCS)**: Version 12.4.0 or later.
- **TivaWare**: Version 2.2.0.295 or later.
- **Compiler**: TI ARM Compiler (included with CCS).

## Setup Instructions
1. **Hardware Setup**:
   - Connect the TM1637 module to the TIVA C LaunchPad as per the pin assignments above.
   - Ensure the LaunchPad is powered via USB.
2. **Software Setup**:
   - Clone or download this repository.
