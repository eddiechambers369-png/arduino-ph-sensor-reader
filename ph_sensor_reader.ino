/*
  File: ph_sensor_reader.ino
  Purpose:
    Read an analog pH sensor on an Arduino Uno, average multiple samples,
    convert the reading to voltage, and estimate pH using a two-point calibration.

  Hardware assumptions:
    - Arduino Uno
    - Analog pH sensor board connected to A0
    - Default analog reference voltage of 5.0 V

  Calibration notes:
    Replace PH7_VOLTAGE and PH4_VOLTAGE with the actual voltages measured
    from your sensor while placed in standard pH 7 and pH 4 buffer solutions.

  External libraries:
    None required.
    Uses only standard Arduino built-in functions.

  Author:
    [Your Name / Team Name]

  Date:
    [Insert Date]
*/

// -----------------------------
// Pin configuration
// -----------------------------
const int PH_SENSOR_PIN = A0;

// -----------------------------
// Electrical configuration
// -----------------------------
const float ADC_REFERENCE_VOLTAGE = 5.0f;   // Arduino Uno default analog reference
const int ADC_RESOLUTION = 1023;            // 10-bit ADC: values from 0 to 1023

// -----------------------------
// Sampling configuration
// -----------------------------
const int NUMBER_OF_SAMPLES = 10;           // Number of analog readings to average
const int SAMPLE_DELAY_MS = 20;             // Delay between samples
const int LOOP_DELAY_MS = 1000;             // Delay between output updates

// -----------------------------
// Calibration values
// Replace these with measured values from your own calibration process
// Example only:
//   If the sensor reads 2.50 V in pH 7 buffer
//   and 3.00 V in pH 4 buffer
// -----------------------------
const float PH7_VOLTAGE = 2.50f;
const float PH4_VOLTAGE = 3.00f;

// Function prototypes
float readAverageAnalogValue(int pin, int sampleCount, int delayBetweenSamplesMs);
float convertAdcToVoltage(float adcValue, float referenceVoltage, int adcMaxValue);
float calculatePhFromVoltage(float measuredVoltage, float voltageAtPh7, float voltageAtPh4);
void printSensorData(float averageAdcReading, float measuredVoltage, float calculatedPh);

void setup() {
  Serial.begin(9600);

  Serial.println("======================================");
  Serial.println("Arduino pH Sensor Reader Starting...");
  Serial.println("Using two-point linear calibration");
  Serial.println("======================================");
}

void loop() {
  // Step 1: Read and average raw ADC values from the pH sensor
  float averageAdcReading = readAverageAnalogValue(
    PH_SENSOR_PIN,
    NUMBER_OF_SAMPLES,
    SAMPLE_DELAY_MS
  );

  // Step 2: Convert the averaged ADC value to a voltage
  float measuredVoltage = convertAdcToVoltage(
    averageAdcReading,
    ADC_REFERENCE_VOLTAGE,
    ADC_RESOLUTION
  );

  // Step 3: Convert measured voltage to pH using calibration data
  float calculatedPh = calculatePhFromVoltage(
    measuredVoltage,
    PH7_VOLTAGE,
    PH4_VOLTAGE
  );

  // Step 4: Print the results to the Serial Monitor
  printSensorData(averageAdcReading, measuredVoltage, calculatedPh);

  delay(LOOP_DELAY_MS);
}

/*
  readAverageAnalogValue
  ----------------------
  Reads an analog pin multiple times and returns the average ADC value.

  Parameters:
    pin - the analog input pin
    sampleCount - number of readings to take
    delayBetweenSamplesMs - delay in milliseconds between readings

  Returns:
    The average analog reading as a float
*/
float readAverageAnalogValue(int pin, int sampleCount, int delayBetweenSamplesMs) {
  long totalReading = 0;

  for (int sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++) {
    totalReading += analogRead(pin);
    delay(delayBetweenSamplesMs);
  }

  return totalReading / (float)sampleCount;
}

/*
  convertAdcToVoltage
  -------------------
  Converts a raw ADC reading into a voltage.

  Formula:
    voltage = adcValue * referenceVoltage / adcMaxValue

  Parameters:
    adcValue - averaged ADC reading
    referenceVoltage - ADC reference voltage (typically 5.0 V on Uno)
    adcMaxValue - maximum ADC reading (1023 for 10-bit ADC)

  Returns:
    The measured voltage
*/
float convertAdcToVoltage(float adcValue, float referenceVoltage, int adcMaxValue) {
  return adcValue * referenceVoltage / adcMaxValue;
}

/*
  calculatePhFromVoltage
  ----------------------
  Uses two-point linear calibration to convert sensor voltage into pH.

  Calibration line:
    pH = slope * voltage + intercept

  The slope and intercept are calculated from:
    (voltageAtPh7, pH 7)
    (voltageAtPh4, pH 4)

  Parameters:
    measuredVoltage - current sensor voltage
    voltageAtPh7 - measured sensor voltage in pH 7 buffer
    voltageAtPh4 - measured sensor voltage in pH 4 buffer

  Returns:
    Estimated pH value
*/
float calculatePhFromVoltage(float measuredVoltage, float voltageAtPh7, float voltageAtPh4) {
  float slope = (7.0f - 4.0f) / (voltageAtPh7 - voltageAtPh4);
  float intercept = 7.0f - (slope * voltageAtPh7);

  return (slope * measuredVoltage) + intercept;
}

/*
  printSensorData
  ---------------
  Prints the averaged ADC reading, voltage, and pH to the Serial Monitor.

  Parameters:
    averageAdcReading - averaged raw ADC value
    measuredVoltage - converted sensor voltage
    calculatedPh - estimated pH value
*/
void printSensorData(float averageAdcReading, float measuredVoltage, float calculatedPh) {
  Serial.print("ADC Reading: ");
  Serial.print(averageAdcReading, 1);

  Serial.print(" | Voltage: ");
  Serial.print(measuredVoltage, 3);
  Serial.print(" V");

  Serial.print(" | Estimated pH: ");
  Serial.println(calculatedPh, 2);
}
