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

    step_size = 0.001;
    energy = 0.1;
    // Initialize arrays with zeros
    initArray(filter_coefficients, FILTER_LENGHT);
    initArray(buffer, FILTER_LENGHT);


}

void loop() {

    startTime = 0.0;
    endTime = 0.0;
    initArray(output, LENGHT);
    
    //while (!Serial.available()) {}

    startTime = millis();
    nlms_filter(); // 59 ms
    //Com recurso de memmove - 40 ms
    //

    endTime = millis();

    Serial.println(endTime - startTime);

}
