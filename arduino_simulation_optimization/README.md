
# Analysis

### Original Version:

```cpp
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
```
### Updated Version:


```cpp

void nlms_filter() {
  error = 0.0;
  float reciprocal_energy = 1.0 / energy;

  for (int n = 0; n < LENGHT; n++) {
    // Shift buffer values
    memmove(buffer + 1, buffer, (FILTER_LENGHT - 1) * sizeof(float));
    buffer[0] = reference_signal[n];
    
    output[n] = dotProduct(filter_coefficients, buffer, FILTER_LENGHT);

    error = reference_signal[n] - output[n];    // Calculate error for current sample

    // Update energy by subtracting the squared value of the buffer's last element
    // and adding the squared value of the current buffer's first element
    energy += buffer[0] * buffer[0] - buffer[FILTER_LENGHT - 1] * buffer[FILTER_LENGHT - 1];

    // Update filter coefficients using the NLMS update equation
    for (int i = 0; i < FILTER_LENGHT; i++) {
      filter_coefficients[i] += (step_size * reciprocal_energy) * error * buffer[i];
    }
  }
}
```
## main differences:

- Shifting buffer values: In the original version, the buffer values are shifted manually in a for loop, moving each value to the next position. In the updated version, the memmove function is used to shift the buffer values efficiently in a single operation.

- Energy update: In the original version, the energy estimate is updated by summing the squared values of the entire buffer using the dotProduct function. In the updated version, the energy update is optimized by subtracting the squared value of the last element of the buffer and adding the squared value of the current element.

- Filter coefficients update: In the original version, the filter coefficients are updated using the NLMS update equation, dividing the step size by the energy estimate. In the updated version, a reciprocal energy value (reciprocal_energy) is calculated once to avoid repeated division in each iteration of the loop, improving efficiency.

Overall, the updated version introduces some optimizations and improvements to the NLMS filter implementation.

## Result:

### Original Version
- 52,4 ma 
- 57.60 ms (measure in osciloscope) and 59ms (measure using `millis()`)
- 3690 (11%) of Flash Memory
- 1088 bytes (53%) of SRAM Memory

### New Version
- 54 ma 
- 41.2 ms (measure in osciloscope) and 40ms (measure using `millis()`)
- 3800 (11%) of Flash Memory
- 1084 bytes (52%) of SRAM Memory
