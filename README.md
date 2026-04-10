# Arduino pH Sensor Reader

## Overview
This project reads an analog pH sensor using an Arduino Uno, averages multiple sensor readings, converts the result to voltage, and estimates pH using a two-point linear calibration.

The code is written to be clear, modular, and easy to maintain. Functions are separated by purpose, variable names are descriptive, and comments are included throughout to explain the logic and calibration process.

## Hardware
- Arduino Uno
- Analog pH sensor board
- pH probe
- Connection from pH sensor output to analog pin A0

## File
- `ph_sensor_reader.ino` — main Arduino program

## How it works
The program:
1. Reads the pH sensor multiple times
2. Averages the readings to reduce noise
3. Converts the ADC reading to voltage
4. Applies a two-point calibration using measured values at pH 7 and pH 4
5. Prints voltage and estimated pH to the Serial Monitor

## Calibration
Before use, replace the calibration constants in the code with the voltages measured from your own sensor:

- `PH7_VOLTAGE`
- `PH4_VOLTAGE`

These should be obtained by placing the probe in standard buffer solutions and recording the sensor voltage.

## Software / Libraries
No external libraries are required.

This project uses only standard Arduino built-in functions available in the Arduino IDE.

## Suggested Serial Monitor Settings
- Baud rate: `9600`

## Notes
- The code assumes a 5.0 V analog reference, which matches the default Arduino Uno setup.
- If a different board or analog reference is used, update the reference voltage in the code.
- For best results, recalibrate the sensor regularly.

## Author
The Aquaponers

## Date
2026-04-09
