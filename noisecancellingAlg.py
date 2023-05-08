import numpy as np
import scipy.io.wavfile as wav
from scipy import signal
from matplotlib import pyplot as plt

#with low pass filter

def nlms_filter_with_lowpass(x, d, L, mu, beta=1, pre_emphasis=0.75, cutoff_freq=0.35):
    # x: input signal (1D array)
    # d: reference signal (1D array)
    # L: filter length
    # mu: step size parameter
    # beta: regularization parameter
    # pre_emphasis: pre-emphasis factor
    # cutoff_freq: cutoff frequency for the lowpass filter


    b, a = signal.butter(4, cutoff_freq, 'lowpass')
    x_filtered = signal.filtfilt(b, a, x)


    # Apply pre-emphasis to the filtered input signal
    x_pre_emph = np.append(x_filtered[0], x_filtered[1:] - pre_emphasis * x_filtered[:-1])

    # Initialize the filter coefficients and energy estimate
    w = np.zeros(L)
    energy = 0.1

    # Create a buffer to store the previous input samples
    buffer = np.zeros(L)


    # Create an array to store the output signal
    y = np.zeros(len(x_filtered))

    # Apply the NLMS filter to the input signal
    for n in range(len(x_pre_emph)):
        # Update the buffer with the latest input sample
        buffer[1:] = buffer[:-1]
        buffer[0] = x_pre_emph[n]

        # Calculate the output signal and error
        y[n] = np.dot(w, buffer)

        try:
            e = d[n] - y[n]
        except:
            e = 0

        # Update the filter coefficients and energy estimate
        energy = beta * energy + (1 - beta) * np.dot(buffer, buffer)

      
        w = w + (mu / energy) * e * buffer


    return y



def applyNoiseReduction(fileName, func):

  L = 256
  mu = 0.0001

  # leia o arquivo de áudio de entrada
  rate, data = wav.read('audio_com_ruido.wav')

  # normalize o sinal de entrada para o intervalo [-1, 1]
  data = data / np.max(np.abs(data))  

  rate, ref = wav.read('audio_ruido.wav')
  ref = ref / np.max(np.abs(ref))  

  filtered = func(data, ref, L, mu)

  # normalize o sinal filtrado para o intervalo [-1, 1]
  filtered = filtered / np.max(np.abs(filtered))
  
  # escreva o sinal filtrado em um novo arquivo de áudio
  wav.write('%s.wav'%(fileName), rate, np.array(filtered * 32768, dtype=np.int16)) 

  plt.subplot(3, 1, 1)
  plt.plot(data)
  plt.title('Sinal de entrada')
  plt.xlabel('Amostras')
  plt.ylabel('Amplitude')

  plt.subplot(3, 1, 2)
  plt.plot(ref)
  plt.title('Sinal de referência')
  plt.xlabel('Amostras')
  plt.ylabel('Amplitude')

  plt.subplot(3, 1, 3)
  plt.plot(filtered)
  plt.title('Sinal filtrado')
  plt.xlabel('Amostras')
  plt.ylabel('Amplitude')

  plt.tight_layout()
  plt.show()


applyNoiseReduction("output-nlmslowpass",nlms_filter_with_lowpass)

