# NLMS Filter 
This project was made for the Embedded Systems Course taught by  [Prof.Elias Teodoro da Silva Júnior](http://lattes.cnpq.br/9405844293925084) from the [Federal Institute of Science & Technology of Ceará - Fortaleza Campus](https://ifce.edu.br/fortaleza) - 2023.1

This code implements a Normalized Least Mean Squares (NLMS) adaptive filter. The filter is designed to reduce the noise in an input signal by adapting its filter coefficients based on the input and desired output.

## Code Description
The code consists of the following files:

- `nlms_filter.c`: This file contains the main implementation of the NLMS filter with low-pass. It defines the necessary functions and performs the filtering operation.

### Usage
To use , follow these steps:

- Clone the repository or download the nlms_filter.c file.
- Compile the code using a C compiler. For example, you can use the following command with GCC:
 ```bash
 gcc nlms_filter.c -o nlms_filter
 ```
- Run the compiled executable:

```bash
./nlms_filter
```
- The filtered output will be printed to the console.

## Input and Noise Signals
The code uses two input signals:

- `x` : This array represents the input signal to be filtered. It should be defined in the main function and can be modified to include your own input data.

- `ruido`: This array represents the noise signal to be filtered out. It should also be defined in the main function and can be modified to include your own noise data.

Both `x` and `ruido` should have a length of 100, as defined by the LENGHT constant in the code.

## Customization
You can customize the behavior of the NLMS filter by modifying the following variables in the `NLMSFilter` function:

- `mu`: This variable represents the step size or adaptation rate of the filter. Adjusting this value can impact the convergence speed and stability of the filter.

- `energy` : This variable represents the initial energy of the filter. It is used in the adaptation formula and can affect the filter's behavior.

## Output
The filtered output is stored in the y array, which represents the output of the NLMS filter. The printArray function is used to print the values of the y array to the console.

## License
This code is provided under the MIT License. Feel free to use and modify the code according to your needs.

## Resources

- [Active Noise Cancellation Project](http://www-personal.umich.edu/~gowtham/bellala_EECS452report.pdf)
- [Active Noise Cancellation – From Modeling to Real-Time Prototyping ](https://www.youtube.com/watch?v=mlkAIlL5Bqs&t=1s)
- [Adaptative FIR LMS Filter](https://youtu.be/OPWHS9LYunA)
