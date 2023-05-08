#include <stdlib.h>
#include <math.h>


#define M_PI 3.14159265358979323846

/* Função que retorna o coeficiente b */
void butter_b_coeffs(int order, double cutoff_freq, double *b_coeffs) {
    int i, j;
    double fc = tan(cutoff_freq * M_PI) ;
    double sqrt2 = sqrt(2.0);
    double denom;

    for(i = 0; i <= order; i++) {
        b_coeffs[i] = 0.0;
        for(j = 0; j <= order; j++) {
            if (i == j) {
                denom = 1.0;
            } else if (i == j + 1 || i == j - 1) {
                denom = sqrt2 * fc;
            } else {
                denom = 0.0;
            }
            b_coeffs[i] += pow(-1.0, i+j) * fc / (1.0 + pow(fc, 2.0)) * denom;
        }
    }
}

/* Função que retorna o coeficiente a */
void butter_a_coeffs(int order, double *a_coeffs) {
    int i, j;
    double sqrt2 = sqrt(2.0);
    double denom;

    for(i = 0; i <= order; i++) {
        a_coeffs[i] = 0.0;
        for(j = 0; j <= order; j++) {
            if (i == j) {
                denom = 1.0;
            } else if (i == j + 1 || i == j - 1) {
                denom = sqrt2;
            } else {
                denom = 0.0;
            }
            a_coeffs[i] += pow(-1.0, i+j) * denom;
        }
    }
}

/* Função de filtro Butterworth */
void signal_butter(int order, double cutoff_freq, double *input, double *output, int length) {
    double b_coeffs[order+1];
    double a_coeffs[order+1];
    butter_b_coeffs(order, cutoff_freq, b_coeffs);
    butter_a_coeffs(order, a_coeffs);

    int i, j;
    double sum;

    for(i = 0; i < length; i++) {
        sum = 0.0;
        for(j = 0; j <= order; j++) {
            if(i - j < 0) {
                sum += 0.0;
            } else {
                sum += b_coeffs[j] * input[i-j];
            }
        }
        for(j = 1; j <= order; j++) {
            if(i - j < 0) {
                sum += 0.0;
            } else {
                sum -= a_coeffs[j] * output[i-j];
            }
        }
        output[i] = sum / a_coeffs[0];
    }
}

void signal_filtfilt(float *x, float *y, int N, float *b, int nb, float *a, int na) {
    // N: comprimento do sinal de entrada
    // x: sinal de entrada
    // y: sinal de saída
    // b: coeficientes do numerador do filtro
    // nb: ordem do numerador do filtro
    // a: coeficientes do denominador do filtro
    // na: ordem do denominador do filtro

    int i, j;
    float sum;

    // Inicializar saídas
    for (i = 0; i < N; i++) {
        y[i] = 0;
    }

    // Aplicar filtro
    for (i = 0; i < N; i++) {
        sum = 0;
        for (j = 0; j < nb; j++) {
            if (i - j >= 0) {
                sum += b[j] * x[i - j];
            }
        }
        for (j = 1; j < na; j++) {
            if (i - j >= 0) {
                sum -= a[j] * y[i - j];
            }
        }
        y[i] = sum / a[0];
    }
}



void nlms_filter_with_lowpass(float *x, float *d, int L, float mu, float beta, float pre_emphasis, float cutoff_freq, int len_x, float *y)
{
    // Apply a lowpass filter to the input signal
    float *x_filtered = (float*) malloc(len_x * sizeof(float));
    int i;
    float a[5], b[5];
    int order = 4;
    signal_butter(order, cutoff_freq, b, a);
    signal_filtfilt(b, a, x, len_x, x_filtered);

    // Apply pre-emphasis to the filtered input signal
    float *x_pre_emph = (float*) malloc(len_x * sizeof(float));
    x_pre_emph[0] = x_filtered[0];
    for (i = 1; i < len_x; i++) {
        x_pre_emph[i] = x_filtered[i] - pre_emphasis * x_filtered[i-1];
    }

    // Initialize the filter coefficients and energy estimate
    float *w = (float*) calloc(L, sizeof(float));
    float energy = 0.1f;

    // Create a buffer to store the previous input samples
    float *buffer = (float*) calloc(L, sizeof(float));

    // Apply the NLMS filter to the input signal
    for (i = 0; i < len_x; i++) {
        // Update the buffer with the latest input sample
        memmove(buffer + 1, buffer, (L - 1) * sizeof(float));
        buffer[0] = x_pre_emph[i];

        // Calculate the output signal and error
        float output = 0.0f;
        int j;
        for (j = 0; j < L; j++) {
            output += w[j] * buffer[j];
        }
        y[i] = output;

        float error = d[i] - output;

        // Update the filter coefficients and energy estimate
        energy = beta * energy + (1.0f - beta) * dot_product(buffer, buffer, L);

        for (j = 0; j < L; j++) {
            w[j] += (mu / energy) * error * buffer[j];
        }
    }

    free(x_filtered);
    free(x_pre_emph);
    free(w);
    free(buffer);
}
