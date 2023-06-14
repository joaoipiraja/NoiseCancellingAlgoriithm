#include <string.h>
#include <math.h>

// Constants
#define LENGHT 10 //Size of a Batch
#define FILTER_LENGHT 100 // Filter Lenght 

// Arrays and variables
float filter_coefficients[FILTER_LENGHT];
float buffer[FILTER_LENGHT];
float output[LENGHT];     // Output
float reference_signal[LENGHT];     // Input with noise
float ruido[LENGHT]; // Noise

// Parameters
float step_size;
double energy;
float error;
float result;

void initArray(float values[], int length) {
  memset(values, 0, length * sizeof(float));
}

void nlms_filter() {
  error = 0.0;
  float reciprocal_energy = 1.0 / energy;

  for (int n = 0; n < LENGHT; n++) {
    memmove(buffer + 1, buffer, (FILTER_LENGHT - 1) * sizeof(float));
    buffer[0] = reference_signal[n];

    float result = 0.0;
    for (int i = 0; i < FILTER_LENGHT; i++) {
      result += filter_coefficients[i] * buffer[i];
    }
    output[n] = result;

    error = reference_signal[n] - output[n];

    energy += buffer[0] * buffer[0] - buffer[FILTER_LENGHT - 1] * buffer[FILTER_LENGHT - 1];

    for (int i = 0; i < FILTER_LENGHT; i++) {
      filter_coefficients[i] += (step_size * reciprocal_energy) * error * buffer[i];
    }
  }
}


void setup() {
    // Open a serial connection
    Serial.begin(31250);

    step_size = 0.0001;
    energy = 0.1;
    // Initialize arrays with zeros
    initArray(filter_coefficients, FILTER_LENGHT);
    initArray(buffer, FILTER_LENGHT);

    pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {

    initArray(reference_signal, LENGHT);
    initArray(ruido, LENGHT);
    initArray(output, LENGHT);
    
    
    // Wait until data is available on the serial connection
    
    while (!Serial.available()) {
      digitalWrite(LED_BUILTIN, HIGH);
    }

    // Read input values from serial
    for (int i = 0; i < LENGHT; i++) {
        reference_signal[i] = Serial.parseFloat();
    }

    // Read noise values from serial
    for (int i = 0; i < LENGHT; i++) {
        ruido[i] = Serial.parseFloat();
    }

    // Apply the NLMS Adaptative filter
    nlms_filter();

    // Print the output values
    for (int i = 0; i < LENGHT; i++) {
        Serial.println(output[i], 6);
    }
}