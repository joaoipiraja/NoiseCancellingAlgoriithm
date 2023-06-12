# Arduino Simulation

# Process 
The start algorithm was not running on the arduino. We made some changes

- Input of 2 `Float` arrays 1000 positions

### in python

- Generate the signal from a sinusoid with noise + reference noise
With `pyserial`, send this data in small batches of 10 and receive the output
- Run the same filter algorithm and compare error between responses

### On arduino:
- Receive 2 * 10 samples, process the response and send via serial
- Reset inputs, outputs, energy and error
- Keep filter coefficients and buffer

## Problems

- Memory limitation:
	- Filter size adjustments and adapt algorithm to its adaptive character
	- Overflow of variables in operation
- Limitation on precision of numbers

# Installation Guide

## C

- Install the Arduino IDE: Download and install the Arduino IDE on your computer.
- Connect your Arduino board: Connect your Arduino board to your computer using a USB cable.
- Open the Arduino IDE: Launch the Arduino IDE that you installed in the previous step.
- Open the `arduino_simulation.ino` sketch: In the Arduino IDE, go to FILE > OPEN and navigate to the location where you saved the file. Select the file and click Open.
- Select the board and port: In the Arduino IDE, go to Tools > Board and select the appropriate Arduino board model. Then, go to TOOLS > PORT and select the port to which your Arduino board is connected.
- Upload the sketch: Click on the Upload button in the Arduino IDE to compile and upload the sketch to your Arduino board.

## Python 

- Install Python: Make sure you have Python installed on your system

- Clone or download the code: Clone the code repository or download the pyserialteste.py file to your local machine.

- Install the required dependencies: Open a terminal or command prompt and navigate to the directory where you downloaded `pyserialteste.py`. Run the following command to install the required dependencies:

```bash
pip install numpy tqdm pyserial
```
This command will install the `numpy`, `tqdm`, and `pyserial` libraries.

- Connect your serial device: Connect the serial device (e.g., Arduino, microcontroller) to your computer using a USB cable. Note the serial port name to be used in the code.

- Modify the code (optional): If necessary, open the pyserialteste.py file in a text editor and modify the following variables:

	- port: Change the value to the correct serial port name. For example, on Windows, it might be something like COM1, and on macOS or Linux, it might be /dev/cu.usbmodem1101.
	- baud_rate: Change the value to match the baud rate of your serial device if it's different from 31250.
- Run the code: In the terminal or command prompt, navigate to the directory where you saved `pyserialteste.py`. Run the following command to execute the code:

```bash
python pyserialteste.py
```
The code will start running, and you should see the input arrays, output, and desired output printed in the terminal.

- Review the results: The code will compare the serial output with the desired output and print the values along with the absolute error for each data point.

- Close the serial connection: Once the code execution is complete, it will automatically close the serial connection. You can disconnect the serial device from your computer.



- A length that is too small hinders the performance of the filter.

