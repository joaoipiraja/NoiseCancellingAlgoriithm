
#include <string.h>
#include <math.h>


#define LENGHT 100
#define L 64

float w[L];
float b[L];
float y[LENGHT];
float x[] = { 0.05783501,  0.14401321,  0.3029481 ,  0.12347155,  0.26501613,
        0.21797423,  0.59412402,  0.59967532,  0.4947141 ,  0.5849392 ,
        0.75278209,  0.61580319,  0.65267839,  0.92548981,  0.71347061,
        0.81594012,  0.7271617 ,  0.96596322,  0.94864944,  0.93394517,
        0.98624816,  0.92318725,  1.02619158,  1.17772075,  0.9035374 ,
        1.04702673,  0.98149966,  0.91770941,  0.95217648,  0.91388521,
        1.00163526,  1.12904628,  0.79831386,  0.74019122,  0.89985859,
        0.76734886,  0.90792408,  0.68319408,  0.80195323,  0.62150539,
        0.52754548,  0.50219547,  0.29810401,  0.31214137,  0.55113679,
        0.34359251,  0.07643776,  0.37559284,  0.062235  ,  0.00121459,
       -0.15918961, -0.07990132, -0.10119248, -0.22469265, -0.01185407,
       -0.38107543, -0.37893211, -0.3118943 , -0.43514095, -0.44163762,
       -0.70187065, -0.80490644, -0.81691181, -0.8054469 , -0.86081044,
       -1.03856819, -0.86281979, -0.90672223, -1.10781648, -0.89018683,
       -0.93755383, -0.99877347, -1.12011027, -1.17562972, -1.20396737,
       -0.90036054, -1.00796217, -1.17358392, -0.98230974, -1.00014837,
       -1.04702353, -0.74300316, -0.86848824, -0.96270176, -0.54660593,
       -0.69478863, -0.72878821, -0.63392928, -0.94454714, -0.62015691,
       -0.46436797, -0.52705819, -0.6670443 , -0.30604644, -0.21926622,
       -0.30773503, -0.19626517, -0.10824563,  0.06225036, -0.09074405};
float ruido[] = { 5.78350096e-02,  8.05892904e-02,  1.76355647e-01, -6.57796941e-02,
        1.38681462e-02, -9.40592157e-02,  2.22461560e-01,  1.69880404e-01,
        8.51736764e-03,  4.42983831e-02,  1.59874164e-01, -2.69844216e-02,
       -3.74006191e-02,  1.90898106e-01, -6.26758494e-02,  1.36416496e-03,
       -1.22563725e-01,  8.45098528e-02,  3.90174424e-02, -2.02686324e-04,
        3.13459152e-02, -4.86243160e-02,  4.13838310e-02,  1.83882286e-01,
       -9.53299429e-02,  4.71526032e-02, -1.53551179e-02, -7.21120347e-02,
       -2.66259662e-02, -4.99569441e-02,  5.66344458e-02,  2.06691988e-01,
       -9.76799140e-02, -1.25834181e-01,  6.72887395e-02, -2.84129787e-02,
        1.52174501e-01, -2.95000935e-02,  1.35184226e-01,  3.34640783e-03,
       -3.95143851e-02, -1.14819182e-02, -1.60122514e-01, -8.87891680e-02,
        2.09116649e-01,  6.18599519e-02, -1.43872775e-01,  2.17591443e-01,
       -3.28210397e-02, -3.05133470e-02, -1.27461680e-01,  1.51547216e-02,
        5.68089189e-02, -4.38211946e-03,  2.69878484e-01, -3.90552883e-02,
        2.19984297e-02,  1.46332219e-01,  7.85364415e-02,  1.25422248e-01,
       -8.37116678e-02, -1.38137439e-01, -1.04217636e-01, -4.96973267e-02,
       -6.50486015e-02, -2.05998331e-01,  3.20561494e-03, -1.07284585e-02,
       -1.85462190e-01,  5.48139905e-02,  2.62883277e-02, -1.99710205e-02,
       -1.30288831e-01, -1.78774943e-01, -2.04093238e-01,  9.85067986e-02,
       -1.41237100e-02, -1.88776163e-01, -1.04981684e-02, -4.52461330e-02,
       -1.12875667e-01,  1.66628835e-01,  1.29651250e-02, -1.12976327e-01,
        2.67970024e-01,  8.13578352e-02,  5.80349818e-03,  5.61497363e-02,
       -3.01759534e-01, -2.72489821e-02,  7.62728464e-02, -4.08614560e-02,
       -2.37249383e-01,  6.56160188e-02,  9.27672216e-02, -5.65870422e-02,
       -7.01392412e-03,  1.83468255e-02,  1.25674276e-01, -9.07440544e-02 };



float dotProduct(float a[], float b[], int l) {
    float result = 0.0;
    for (int i = 0; i < l; i++) {
        result += a[i] * b[i];
    }
    return result;
}

void initArray(float values[], int lenght){

  for(int i = 0; i< lenght; i++){
      values[i] = 0.0;
  }
}

void printFloat(float value){
    char buffer[20];
    dtostrf( value, 7, 7, buffer); 
    Serial.print(buffer);
}


void printArray(float values[], int lenght){
  Serial.print("\nsaida = [");
  if(Serial.availableForWrite()){
    for(int i = 0; i< lenght; i++){

        if(i < lenght - 1){
          
              printFloat(values[i]);
              Serial.print(",");
        }else{
              printFloat(values[i]);
        }
      }
      Serial.print("] \n");
  }
  
}


void nlmsFilterWithLowpass(float x[], float d[], float y[]) {

    //Resolucao - trocar buffer por b
    //mu: step size parameter **
    //w[L]: peso(weight) **
       
    float mu = 0.0001;
    float energy = 0.1;
    float error = 0.0;
   
  
    // Initialize the filter coefficients
    initArray(w, L);
    initArray(b, L);
  
    // Apply the NLMS filter to the input signal **
    for (int n = 0; n < LENGHT; n++) {
        for (int i = L - 1; i > 0; i--) {
            b[i] = b[i - 1];
        }
        b[0] = x[n];

        y[n] = dotProduct( w, b, L);

        error = d[n] - y[n];

        // Update the filter coefficients and energy estimate **
        energy = energy + dotProduct(b, b, L);
      
          for (int i = 0; i < L; i++) {
              w[i] += (mu / energy) * error * b[i];
          }
    }
}
 

void setup() {
   
  Serial.begin(9600);
  initArray(y, LENGHT);
  nlmsFilterWithLowpass(x,ruido, y);
  printArray(y, LENGHT);
}

void loop() {
  
}
