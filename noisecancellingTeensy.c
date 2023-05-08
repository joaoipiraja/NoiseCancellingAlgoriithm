#include <Audio.h>
#include <math.h>

#define BLOCK_SIZE 16
#define L 32

AudioInputI2S            i2s_in;       // define i2s input object
AudioOutputI2S           i2s_out;      // define i2s output object
AudioFilterBiquad        lowpass;      // define low-pass filter object
AudioEffectMultiply      pre_emph;     // define pre-emphasis factor object
AudioAnalyzePeak         peak;         // define peak level object
AudioAnalyzeFFT1024      fft1024;      // define FFT object
AudioEffectMultiply      nlms_gain;    // define NLMS filter gain object
AudioEffectDelay         nlms_buffer;  // define NLMS filter buffer object
AudioMixer4              mixer;        // define mixer object

//origem - pin saida / entrada - pin destino
//AudioConnection conexão(origem, saida, entrada, destino);
 
AudioConnection          patchCord1(i2s_in, 0, lowpass, 0);
AudioConnection          patchCord2(lowpass, 0, pre_emph, 0);
AudioConnection          patchCord3(pre_emph, 0, mixer, 0);
AudioConnection          patchCord4(i2s_in, 1, mixer, 1);
AudioConnection          patchCord5(mixer, nlms_buffer);
AudioConnection          patchCord6(nlms_buffer, 0, nlms_gain, 0);
AudioConnection          patchCord7(nlms_buffer, 0, nlms_gain, 1);
AudioConnection          patchCord8(nlms_gain, 0, i2s_out, 0);
AudioConnection          patchCord9(nlms_gain, 0, i2s_out, 1);
AudioConnection          patchCord10(i2s_in, 0, peak);
AudioConnection          patchCord11(i2s_in, 0, fft1024);

float x_buffer[BLOCK_SIZE];     // buffer to store the input signal
float d_buffer[BLOCK_SIZE];     // buffer to store the reference signal
float y_buffer[BLOCK_SIZE];     // buffer to store the output signal
float w[L];                     // array to store the filter coefficients
float energy = 0.1;             // energy estimate
float mu = 0.0001;              // step size parameter
float beta = 1;                 // regularization parameter
float pre_emphasis = 0.75;      // pre-emphasis factor
float cutoff_freq = 0.35;       // cutoff frequency for the lowpass filter

void setup() {
  AudioMemory(40);              // allocate memory for audio objects
  i2s_in.begin();               // start the i2s input
  i2s_out.begin();              // start the i2s output

  // configure the low-pass filter
  lowpass.setLowpass(0, cutoff_freq);
  lowpass.setHighpass(0, 0);
  lowpass.setCoefficients(0, lowpass.biquad);

  // configure the pre-emphasis factor
  pre_emph.gain(0, pre_emphasis);
  pre_emph.gain(1, -pre_emphasis);

  // configure the NLMS filter gain
  nlms_gain.gain(0, 1);
  nlms_gain.gain(1, 1);

  // configure the NLMS filter buffer
  nlms_buffer.delay(0, L - 1);
}

void loop() {
  // read the input signal
  while (i2s_in.available()) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        x_buffer[i] = i2s_in.readFloat();
    } 

    // apply pre-emphasis
    pre_emph.multiply(x_buffer, BLOCK_SIZE);

    // compute the output signal
    for (int i = 0; i < BLOCK_SIZE; i++) {
      // compute the error signal
      float e = d_buffer[i] - nlms_gain.read(0) * x_buffer[i] - nlms_gain.read(1) * x_buffer[i];

      // compute the filter update
      float u = mu / (beta + energy) * e;
      for (int j = 0; j < L; j++) {
        w[j] += u * x_buffer[i-j];
      }

      // compute the output signal
      y_buffer[i] = 0;
      for (int j = 0; j < L; j++) {
        y_buffer[i] += w[j] * x_buffer[i-j];
      }

      // update the energy estimate
      energy = (1 - mu / (beta + energy) * pow(e, 2)) * energy + mu * pow(y_buffer[i], 2);

      // update the buffer
      nlms_buffer.write(y_buffer[i]);
    }

    // write the output signal
    i2s_out.write(y_buffer, BLOCK_SIZE);

    // update the reference signal
    for (int i = 0; i < BLOCK_SIZE; i++) {
      d_buffer[i] = x_buffer[i];
    }

    // compute the peak level
    peak.update();

    // compute the FFT
    fft1024.windowFunction(AudioWindowHanning1024);
    fft1024.reorderInputData();
    fft1024.run();
}}

