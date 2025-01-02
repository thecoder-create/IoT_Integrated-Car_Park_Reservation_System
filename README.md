# IoT_Integrated-Car_Park_Reservation_System

This repository contains code for a parking system that uses multiple ultrasonic sensors to detect parking spot occupancy and manage parking spot reservations. The system communicates with a microcontroller (TM4C123) using UART, and each sensor controls a corresponding LED to indicate the parking spot's status. The system also supports reserving and unreserving parking spots via UART commands.

## Introduction

This code is designed for a TM4C123 microcontroller to manage parking spot reservation using ultrasonic sensors and LED indicators. The system uses three ultrasonic sensors to measure the distance to vehicles in each parking spot, and each parking spot is represented by an LED that changes color based on the spot's status (e.g., occupied, reserved, or available). The system can also manage reservation and unreservation commands sent via UART.

## System Overview

- **Ultrasonic Sensors (HC-SR04)**: The system uses three ultrasonic sensors to measure the distance to objects in the parking spots.
- **LED Indicators**: Each parking spot has an associated LED that indicates the parking spot's status (e.g., green for available, red for occupied, and blue for reserved).
- **UART Communication**: The system communicates over UART to handle reservation and unreservation commands from an external device (such as a server or remote control).

## Code Structure

- **Main Functions**: 
  - The main loop reads data from the ultrasonic sensors and updates the status of the LEDs based on the sensor readings.
  - The system uses interrupt handlers for each sensor to trigger actions when the echo pin is detected.
  - UART handlers are used to receive commands for reserving and unreserving parking spots.
  
- **Key Functions**:
  - `GPIOPortB_Handler()`: Handles the GPIO interrupts for ultrasonic sensor echo pins, calculates the distance for each sensor, and updates the corresponding parking spot status.
  - `UART2_Handler()`: Handles incoming UART commands for reserving and unreserving parking spots.
  - `Timer0A_Handler()`, `Timer1A_Handler()`, and `Timer2A_Handler()`: Handle timers for calculating the duration of the ultrasonic pulse.

## Hardware Setup

This code is intended to run on a **TM4C123 microcontroller**. The hardware setup involves the following:

- **Ultrasonic Sensors**: 
  - Sensor 1 is connected to **PB5** (trigger pin) and **PB6** (echo pin).
  - Sensor 2 is connected to **PB4** (trigger pin) and **PB7** (echo pin).
  - Sensor 3 is connected to **PB3** (trigger pin) and **PB0** (echo pin).
  
- **LED Indicators**: 
  - LEDs connected to GPIO pins for visual feedback on the status of each parking spot (e.g., green for available, red for occupied, blue for reserved).

- **UART Communication**: 
  - UART2 is used for communication with an external device to send commands for reserving or unreserving parking spots.

## External Dependencies

- **`Timer0A.h`, `Timer1A.h`, `Timer2A.h`**: These header files provide functions for handling timers for distance measurement using the ultrasonic sensors.
- **`HC-SR04.h`**: This header file provides functions to initialize and trigger the ultrasonic sensors.
- **`LEDs.h`**: This header file provides functions to control the LEDs that indicate the status of each parking spot.
- **`PLL.h`**: This header file contains functions to initialize the Phase-Locked Loop (PLL) for the microcontroller's clock.
- **`UART.h`**: This header file provides functions to initialize UART communication.

## UART Communication

The system uses UART2 for communication. It listens for the following commands:

- **Reserve Parking Spot**:
  - `Reserve_Sensor1`: Reserves parking spot 1.
  - `Reserve_Sensor2`: Reserves parking spot 2.
  - `Reserve_Sensor3`: Reserves parking spot 3.

- **Unreserve Parking Spot**:
  - `Unreserve_Sensor1`: Unreserves parking spot 1.
  - `Unreserve_Sensor2`: Unreserves parking spot 2.
  - `Unreserve_Sensor3`: Unreserves parking spot 3.

Commands are received by the `UART2_Handler` interrupt handler, which updates the reservation status and modifies the LED status accordingly.

## Interrupts and Handlers

- **GPIO Interrupts**: The system uses GPIO interrupts to trigger the calculation of distances for each ultrasonic sensor based on the rising edge of the echo pin.
- **Timer Interrupts**: Each sensor uses a timer to measure the time elapsed between the trigger and the echo pulse. This is used to calculate the distance to the object.
- **UART Interrupts**: UART interrupts handle the reception of commands for reserving and unreserving parking spots.


