#include <string.h>
#include <math.h>

// Constants
#define LENGHT 10 //Size of a Batch
#define FILTER_LENGHT 100 // Filter Lenght 


static const float reference_signal[] = {-0.06077251, -0.03236705, -0.07158277,  0.03226485,  0.15505562,
         0.02220206,  0.1239529 ,  0.06949102,  0.3687647 , -0.12777717};

static const float ruido[] = {-0.06077251, -0.03865648, -0.08416139,  0.01339755,  0.12990038,
        -0.00924013,  0.086225  ,  0.02547892,  0.31847013, -0.18435222};
// Arrays and variables
// Arrays and variables
float filter_coefficients[FILTER_LENGHT];
float buffer[FILTER_LENGHT];
float output[LENGHT];     // Output

// Parameters
float step_size;
double energy;
float error;
float result;
long startTime;
long endTime;


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
    error = 0.0;  

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

    step_size = 0.001;
    energy = 0.1;
    // Initialize arrays with zeros
    initArray(filter_coefficients, FILTER_LENGHT);
    initArray(buffer, FILTER_LENGHT);

    //Setamos o pino 8 para analisar o tempo no osciloscópio
    pinMode(8, OUTPUT);
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);


}

void loop() {

    startTime = 0.0;
    endTime = 0.0;


    initArray(output, LENGHT);
    
    //while (!Serial.available()) {}

   // startTime = millis();
    digitalWrite(8, HIGH);
    nlms_filter(); // 59 ms
    //Com recurso de memmove/ memoryset/  - 40 ms
    digitalWrite(8, LOW);
    delay(50);

   // endTime = millis();

  // Serial.println(endTime - startTime);
   

}
