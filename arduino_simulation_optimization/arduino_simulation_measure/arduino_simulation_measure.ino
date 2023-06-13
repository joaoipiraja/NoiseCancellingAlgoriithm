#include <string.h>
#include <math.h>

// Constants
#define LENGHT 10 //Size of a Batch
#define FILTER_LENGHT 100 // Filter Lenght 


static const long reference_signal[] = {-0.06077251, -0.03236705, -0.07158277,  0.03226485,  0.15505562,
         0.02220206,  0.1239529 ,  0.06949102,  0.3687647 , -0.12777717};

static const long ruido[] = {-0.06077251, -0.03865648, -0.08416139,  0.01339755,  0.12990038,
        -0.00924013,  0.086225  ,  0.02547892,  0.31847013, -0.18435222};
// Arrays and variables
long filter_coefficients[FILTER_LENGHT];
long buffer[FILTER_LENGHT];
long output[LENGHT];     // Output

unsigned long startTime = 0.0;
unsigned long endTime = 0.0;

// Parameters
float step_size;
double energy;
float error;
float result;


//soma em árvore (tree-based summation):
// Calculates the dot product of two arrays

long dotProduct(long a[], long b[], int size) {
    long result = 0.0;

    // Loop otimizado para processar elementos em pares
    for (int i = 0; i < size - 1; i += 2) {
        result += a[i] * b[i] + a[i + 1] * b[i + 1];
    }

    // Processa elemento restante, se houver
    if (size % 2 != 0) {
        result += a[size - 1] * b[size - 1];
    }

    return result;
}

// long dotProduct(long a[], long b[], int size) {
//     long result = 0.0;
//     for (int i = 0; i < size; i++) {
//         result += static_cast<int>(a[i]) * static_cast<int>(b[i]);
//     }
//     return result;
// }



// Initializes an array with zeros
void initArray(long values[], int length) {
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
    initArray(filter_coefficients, FILTER_LENGHT);
    initArray(buffer, FILTER_LENGHT);


}

void loop() {

    startTime = 0.0;
    endTime = 0.0;
    initArray(output, LENGHT);
    
    //while (!Serial.available()) {}

    startTime = millis();
    nlms_filter(); // 58 ms
    //Com long ao invés de float - 48ms
    //Com artificio no dotproduct - 34ms

    endTime = millis();

    Serial.println(endTime - startTime);

}
