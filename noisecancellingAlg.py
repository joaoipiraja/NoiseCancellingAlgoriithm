import numpy as np

#with low pass filter

def nlms_filter(x, d, L, mu):
    # x: input signal (1D array)
    # d: reference signal (1D array)
    # L: filter length
    # mu: step size parameter


    # Initialize the filter coefficients and energy estimate
    w = np.zeros(L)
    energy = 0.1

    # Create a buffer to store the previous input samples
    buffer = np.zeros(L)


    # Create an array to store the output signal
    y = np.zeros(len(x_filtered))

    # Apply the NLMS filter to the input signal
    for n in range(len(x)):
        # Update the buffer with the latest input sample
        buffer[1:] = buffer[:-1]
        buffer[0] = x[n]

        # Calculate the output signal and error
        y[n] = np.dot(w, buffer)

        try:
            e = d[n] - y[n]
        except:
            e = 0

        # Update the filter coefficients and energy estimate
        energy =  energy + np.dot(buffer, buffer)

      
        w = w + (mu / energy) * e * buffer


    return y
