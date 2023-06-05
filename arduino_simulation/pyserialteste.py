import serial
import time
import numpy as np
from tqdm import tqdm


# Interface for serial communication
class SerialInterface:

    def __init__(self, input_size):
        self.port = '/dev/cu.usbmodem1101'  # Change to the correct serial port
        self.baud_rate = 31250
        self.serial = serial.Serial(self.port, self.baud_rate)
        self.input_size = input_size
        print("Opened connection to:", self.port)
        time.sleep(2)

    def read(self):
        elements = []
        for _ in range(self.input_size):
            while self.serial.in_waiting == 0:
                pass  # Wait until data is available
            c = self.serial.readline()
            element = float(c.decode())
            elements.append(element)
        return elements

    def write(self, num):
        response = "{:.7f}".format(num)
        self.serial.write(response.encode())

    def close(self):
        self.serial.close()


# Divide input arrays into batches
class ArrayDivider:

    def __init__(self, array1, array2, batch_size):
        self.output = []
        self.batch_array1 = self.__divide_chunks(array1, batch_size)
        self.batch_array2 = self.__divide_chunks(array2, batch_size)
        self.batch_size = batch_size
        self.size = len(array1)

    def __divide_chunks(self, array, size):
        # Loop through the array in chunks
        for i in range(0, len(array), size):
            yield array[i:i + size]

    def divide(self, serial_interface):
        count_of_batches = self.size // self.batch_size
        output = []

        print("Sending", self.size * 2, "data points")

        for batch1, batch2, _ in zip(self.batch_array1, self.batch_array2, tqdm(range(count_of_batches))):
            if(len(batch1) != len(batch2)):
                print(len(batch1), len(batch2))
            for elem in batch1:
                serial_interface.write(elem)

            for elem in batch2:
                serial_interface.write(elem)

            output += serial_interface.read()

        return output


# Generate a noisy sine wave signal
def generate_noisy_sine_wave():
    time_array = np.linspace(0, 2 * np.pi, 1000)  # Create an array of time with 1000 points from 0 to 2*pi
    sine_wave = np.sin(time_array)  # Generate a sine wave without noise
    noise = np.random.normal(0, 0.1, 1000)  # Generate an array of white noise with mean 0 and standard deviation 0.1
    noisy_sine_wave = sine_wave + noise  # Add the noise to the sine wave

    return noisy_sine_wave, noise


# NLMS filter implementation
def nlms_filter(input_signal, reference_signal, filter_length=100, step_size=0.001):
    # input_signal: input signal (1D array)
    # reference_signal: reference signal (1D array)
    # filter_length: filter length
    # step_size: step size parameter

    # Initialize the filter coefficients and energy estimate
    filter_coefficients = np.zeros(filter_length)
    energy = 0.1

    # Create a buffer to store the previous input samples
    buffer = np.zeros(filter_length)

    # Create an array to store the output signal
    output_signal = np.zeros(len(input_signal))

    # Apply the NLMS filter to the input signal
    for n in range(len(input_signal)):
        # Update the buffer with the latest input sample
        buffer[1:] = buffer[:-1]
        buffer[0] = input_signal[n]

        # Calculate the output signal and error
        output_signal[n] = np.dot(filter_coefficients, buffer)
        # try:
        error = reference_signal[n] - output_signal[n]
        # except IndexError:
        #     print("error", n, len(reference_signal), len(output_signal) )
        #     error = 0

        # Update the filter coefficients and energy estimate
        energy = energy + np.dot(buffer, buffer)
        filter_coefficients = filter_coefficients + (step_size / energy) * error * buffer

    return output_signal


# Generate a noisy sine wave and noise
audio_with_noise, noise = generate_noisy_sine_wave()

audio_with_noise = audio_with_noise*20
noise = noise*20

# Divide the arrays into batches
divider = ArrayDivider(audio_with_noise, noise, 70)

# Create a serial interface and divide the input arrays using the interface
serial_interface = SerialInterface(70)
output = divider.divide(serial_interface)

# Print the input arrays and the output
print(audio_with_noise.tolist())
print("\n")
print(noise.tolist())
print("\n")
print(output)
print("\n")

# Compute the desired output using the NLMS filter directly
desired_output = nlms_filter(audio_with_noise, noise)
print(desired_output.tolist())

print("\n\n")
print("Serial Output | Desired Output | Error")
for i, (serial_out, desired_out) in enumerate(zip(output, desired_output)):
    print(i, serial_out, " | ", desired_out, " | ", abs(desired_out - serial_out))

# Close the serial connection
serial_interface.close()
