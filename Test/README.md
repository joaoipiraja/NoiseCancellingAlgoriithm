# Documentação para testes

# Sinal com ruído

Conferir arquivo `audio_com_ruido.csv`


# Sinal de referência

Conferir arquivo `audio_ruido.csv`


# `signal.butter` 

```swift
b, a = signal.butter(4, 0.75, 'lowpass')
x_filtered = signal.filtfilt(b, a, data)
```
- Saída para b
```
array([0.34682181, 1.38728723, 2.08093085, 1.38728723, 0.34682181])

```
- Saída para a

```
array([1.        , 1.96842779, 1.73586071, 0.72447083, 0.1203896 ])

```


# `signal.filtfilt(b, a, data)` 

```swift
b, a = signal.butter(4, 0.75, 'lowpass')
x_filtered = signal.filtfilt(b, a, data)
```

Conferir arquivo `signalFiltFilt.csv`

# Ler arquivo .csv em C

```swift
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_MAX_RECORDS 100
#define MAX_FIELD_LENGTH 20

typedef struct {
    double *values;
    int size;
} CSVData;

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

int main() {
    const char *filename = "audio_com_ruido.csv";
    CSVData csvData = processCSVFile(filename);

    if (csvData.values != NULL) {
        // Imprime os valores do array
        for (int i = 0; i < csvData.size; i++) {
            printf("%lf\n", csvData.values[i]);
        }

        // Libera a memória alocada para os valores
        free(csvData.values);
    }

    return 0;
}


```

