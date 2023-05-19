#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define M_PI 3.14159265358979323846

#define INITIAL_MAX_RECORDS 100
#define MAX_FIELD_LENGTH 20

typedef struct {
    double *values;
    int size;
} CSVData;


typedef struct {
    double b0, b1, b2;
    double a1, a2;
} FilterCoefficients;


CSVData processCSVFile(const char *filename) {
    CSVData csvData;
    FILE *file;
    char *line = NULL;
    char *field;
    double *values = NULL;
    int recordCount = 0;
    int maxRecords = 0;
    size_t lineLength = 0;
    size_t maxLineLength = 0;

    // Abre o arquivo CSV para leitura
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.");
        csvData.values = NULL;
        csvData.size = 0;
        return csvData;
    }

    // Lê linha por linha do arquivo
    while (getline(&line, &lineLength, file) != -1) {
        field = strtok(line, ",");
        while (field != NULL) {
            double value = atof(field);

            // Realoca o array de valores se necessário
            if (recordCount >= maxRecords) {
                maxRecords = (maxRecords == 0) ? INITIAL_MAX_RECORDS : maxRecords * 2;
                values = realloc(values, maxRecords * sizeof(double));
                if (values == NULL) {
                    printf("Erro ao alocar memória.");
                    fclose(file);
                    free(line);
                    csvData.values = NULL;
                    csvData.size = 0;
                    return csvData;
                }
            }

            values[recordCount] = value;
            recordCount++;

            field = strtok(NULL, ",");
        }

        // Atualiza o tamanho máximo da linha, se necessário
        size_t currentLineLength = strlen(line);
        if (currentLineLength > maxLineLength) {
            maxLineLength = currentLineLength;
            line = realloc(line, (maxLineLength + 1) * sizeof(char));
            if (line == NULL) {
                printf("Erro ao alocar memória.");
                fclose(file);
                free(values);
                csvData.values = NULL;
                csvData.size = 0;
                return csvData;
            }
        }
    }

    csvData.values = values;
    csvData.size = recordCount;

    // Libera a memória alocada para a linha
    free(line);

    // Fecha o arquivo
    fclose(file);

    return csvData;
}

double dotProduct(const double *a, const double *b, int length) {
    double result = 0.0;
    for (int i = 0; i < length; i++) {
        result += a[i] * b[i];
    }
    return result;
}

void forwardFilter(const double *input, const double *b, const double *a,double *output, int size, int ncoef) {
    for (int i = 0; i < size; i++) {
        double sum = 0.0;
        for (int j = 0; j < ncoef; j++) {
            if (i - j >= 0) {
                sum += b[j] * input[i - j];
            }
        }
        for (int j = 1; j < ncoef; j++) {
            if (i - j >= 0) {
                sum -= a[j] * output[i - j];
            }
        }
        output[i] = sum / a[0];
    }
}

void backwardFilter(const double *input, const double *b, const double *a, double *output, int size, int ncoef) {
    for (int i = size - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = 0; j < ncoef; j++) {
            if (i + j < size) {
                sum += b[j] * input[i + j];
            }
        }
        for (int j = 1; j < ncoef; j++) {
            if (i + j < size) {
                sum -= a[j] * output[i + j];
            }
        }
        output[i] = sum / a[0];
    }
}

void signal_filtfilt(const double *b, const double *a, const double *x, double *y, int size, int ncoef) {
    double *temp = (double *)malloc(size * sizeof(double));
    double *reverse = (double *)malloc(size * sizeof(double));

    forwardFilter(x, b, a, temp, size, ncoef);
    backwardFilter(temp, b, a, reverse, size, ncoef);
    forwardFilter(reverse, b, a, y, size, ncoef);

    free(temp);
    free(reverse);
}


void butterworthLowpass(int order, double cutoff, double *b, double *a) {
    double omegaC = tan(M_PI * cutoff);
    double omegaCSquared = omegaC * omegaC;
    double theta = M_PI / (2.0 * order);

    // Calculate the denominator coefficients (a)
    for (int i = 0; i < order; i++) {
        double alpha = sin((2 * i + 1) * theta);
        double beta = 1.0 + (omegaC * alpha);
        a[i] = omegaCSquared / (beta + omegaCSquared);
    }

    // Calculate the numerator coefficients (b)
    for (int i = 0; i < order; i++) {
        b[i] = 1.0;
    }
}

void nlmsFilterWithLowpass(const double *x, const double *d, int length,   double *y) {

    int L = 256;
    double mu = 0.0001;
    double beta = 1.0;
  
    int order = 4;
    double cutoff = 0.75;
    double pre_emphasis = 0.35;
    double energy = 0.1;

  
    double *x_filtered = (double *)malloc(length * sizeof(double));
    double *x_pre_emph = (double *)malloc(length * sizeof(double));
    double *w = (double *)malloc(L * sizeof(double));
    double *buffer = (double *)malloc(L * sizeof(double));
    double *b = (double *)malloc(order * sizeof(double));
    double *a = (double *)malloc(order * sizeof(double));

    butterworthLowpass(order,cutoff,b, a);
    signal_filtfilt(b,a, x, x_filtered, L, order);



    // Apply pre-emphasis to the filtered input signal
    x_pre_emph[0] = x_filtered[0];
    for (int i = 1; i < length; i++) {
        x_pre_emph[i] = x_filtered[i] - pre_emphasis * x_filtered[i - 1];
    }

    // Initialize the filter coefficients
    for (int i = 0; i < L; i++) {
        w[i] = 0.0;
    }

    // Apply the NLMS filter to the input signal
    for (int n = 0; n < length; n++) {
        // Update the buffer with the latest input sample
        for (int i = L - 1; i > 0; i--) {
            buffer[i] = buffer[i - 1];
        }
        buffer[0] = x_pre_emph[n];

        // Calculate the output signal
        y[n] = 0.0;
        for (int i = 0; i < L; i++) {
            y[n] += w[i] * buffer[i];
        }

        // Calculate the error
        double e;
        if (n < length) {
            e = d[n] - y[n];
        } else {
            e = 0.0;
        }

        // Update the filter coefficients and energy estimate
        energy = beta * energy + (1 - beta) * dotProduct(buffer, buffer, L);
        for (int i = 0; i < L; i++) {
            w[i] += (mu / energy) * e * buffer[i];
        }
    }
}




int main() {

   int order = 4;
   double cutoff = 0.75;
  //  const char *filename = "audio_com_ruido.csv";
    CSVData audioRuido = processCSVFile("audio_ruido.csv");
    CSVData audioComRuido = processCSVFile("audio_com_ruido.csv");

    double *y = (double*) malloc(audioRuido.size *             sizeof(double));
 
    nlmsFilterWithLowpass(audioRuido.values, audioComRuido.values,   audioRuido.size, y);


    return 0;
}
