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

#define MAX_LINE_LENGTH 100
#define MAX_FIELD_LENGTH 20
#define MAX_RECORDS 100

int main() {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char *field;
    double values[MAX_RECORDS];
    int recordCount = 0;

    // Abre o arquivo CSV para leitura
    file = fopen("arquivo.csv", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.");
        return 1;
    }

    // Lê linha por linha do arquivo
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        field = strtok(line, ",");
        if (field != NULL) {
            double value = atof(field);
            values[recordCount] = value;
            recordCount++;
        }
    }

    // Imprime os valores do array
    for (int i = 0; i < recordCount; i++) {
        printf("%lf\n", values[i]);
    }

    // Fecha o arquivo
    fclose(file);

    return 0;
}

```

