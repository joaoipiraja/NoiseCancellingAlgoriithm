# Documentação para testes


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
