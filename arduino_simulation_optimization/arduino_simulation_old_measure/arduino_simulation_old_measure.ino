#include <string.h>
#include <math.h>

// Constants
#define LENGTH 10           // Size of a batch
#define FILTER_LENGTH 100   // Filter length

static const float reference_signal[] = {-0.06077251, -0.03236705, -0.07158277,  0.03226485,  0.15505562,
                                          0.02220206,  0.1239529 ,  0.06949102,  0.3687647 , -0.12777717};

static const float noise[] = {-0.06077251, -0.03865648, -0.08416139,  0.01339755,  0.12990038,
                              -0.00924013,  0.086225  ,  0.02547892,  0.31847013, -0.18435222};

// Arrays and variables
float filter_coefficients[FILTER_LENGTH];   // Filter coefficients
float buffer[FILTER_LENGTH];                // Buffer
float output[LENGTH];                       // Output

// Parameters
float step_size;                            // Step size
double energy;                              // Energy
float error;                                // Error
float result;                               // Result
long startTime;                             // Start time
long endTime;                               // End time

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

// Applies the NLMS adaptive filter
void nlms_filter() {
    error = 0.0;

    // Apply the NLMS filter to the input signal
    for (int n = 0; n < LENGTH; n++) {
        // Shift the filter coefficients
        for (int i = FILTER_LENGTH - 1; i > 0; i--) {
            buffer[i] = buffer[i - 1];
        }
        buffer[0] = reference_signal[n];

        // Calculate the output
        output[n] = dotProduct(filter_coefficients, buffer, FILTER_LENGTH);

        // Calculate the error
        error = reference_signal[n] - output[n];

        // Update the filter coefficients and energy estimate
        energy = energy + dotProduct(buffer, buffer, FILTER_LENGTH);
        for (int i = 0; i < FILTER_LENGTH; i++) {
            filter_coefficients[i] += (step_size / energy) * error * buffer[i];
        }
    }
}

void setup() {
    // Open a serial connection
    Serial.begin(31250);

    step_size = 0.001;
    energy = 0.1;

    // Initialize arrays with zeros
    initArray(filter_coefficients, FILTER_LENGTH);
    initArray(buffer, FILTER_LENGTH);

    // Set pin 8 to analyze the time on the oscilloscope
    pinMode(8, OUTPUT);
    
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
}

void loop() {
    startTime = 0.0;
    endTime = 0.0;

    initArray(output, LENGTH);

    startTime = millis();
    digitalWrite(8, HIGH);
    nlms_filter();
    digitalWrite(8, LOW);
    delay(50);

    endTime = millis();

    Serial.println(endTime - startTime);
}
