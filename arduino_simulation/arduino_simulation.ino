#include <string.h>
#include <math.h>

// Constants
#define LENGHT 5 //Size of a Batch
#define FILTER_LENGHT 200 // Filter Lenght 

// Arrays and variables
float filter_coefficients[FILTER_LENGHT];
float buffer[FILTER_LENGHT];
float output[LENGHT];     // Output
float reference_signal[LENGHT];     // Input with noise
float ruido[LENGHT]; // Noise

// Parameters
float step_size = 0.0001;
float energy;
float error;
float result;

// Calculates the dot product of two arrays
float dotProduct(float a[], float b[], int size) {
    result = 0.0;
    for (int i = 0; i < size; i++) {
        result += a[i] * b[i];
    }
    return result;
}

// Initializes an array with zeros
void initArray(float values[], int length) {
    for (int i = 0; i < length; i++) {
        values[i] = 0.0;
    }
}

// Applies the NLMS Adaptative filter
void nlms_filter() {
  
    
    // Apply the NLMS filter to the input signal
    for (int n = 0; n < LENGHT; n++) {
        // Shift the filter coefficients
        for (int i = FILTER_LENGHT- 1; i > 0; i--) {
            buffer[i] = buffer[i - 1];
        }
        buffer[0] = reference_signal[n];

        // Calculate the output
        output[n] = dotProduct(filter_coefficients, buffer, FILTER_LENGHT);

        // Calculate the error
      
        error = reference_signal[n] - output[n];

        // Update the filter coefficients and energy estimate
        energy = energy + dotProduct(buffer, buffer, FILTER_LENGHT);
        for (int i = 0; i < FILTER_LENGHT; i++) {
            filter_coefficients[i] += (step_size/ energy) * error * buffer[i];
        }
    }
}

void setup() {
    // Open a serial connection
    Serial.begin(31250);

    energy = 0.1;
    error = 0.0;  
    
    // Initialize arrays with zeros
    initArray(filter_coefficients, FILTER_LENGHT);
    initArray(buffer, FILTER_LENGHT);
}

void loop() {

    initArray(reference_signal, LENGHT);
    initArray(ruido, LENGHT);
    initArray(output, LENGHT);
    // Wait until data is available on the serial connection
    while (Serial.available() < 0) {}

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
        Serial.println(output[i], 7);
    }
}
