#include "StressModel.h"

using namespace Eloquent::ML::Port;

stressModel randomForest;

#define MOTOR_PIN D10
#define EDA_PIN A0
#define windowSize 30
#define SENSOR_MIN_THRESHOLD 10   
#define SENSOR_MAX_THRESHOLD 4085

float edaBuffer[windowSize];
int bufferIndex = 0;

bool isSensorConnected(int rawEDA) {
    if (rawEDA < SENSOR_MIN_THRESHOLD || rawEDA > SENSOR_MAX_THRESHOLD) {
        return false;
    }
    return true;
}

// calculate mean function
 float calculateMean(float arr[], int size) {
    float sum = 0;
    for(int k = 0; k < size; k++) {
        sum += arr[k];
    }
    return sum / size;
 }

     // calculate standard deviation function
    float calculateStdDev(float arr[], int size, float mean) {
        float sum = 0;
        for(int k = 0; k < size; k++) {
            sum += pow(arr[k] - mean, 2);
        }
        return sqrt(sum / size);
    }
    
void setup() {
    Serial.begin(115200);
    delay(5000);
    pinMode(EDA_PIN, INPUT);
    pinMode(MOTOR_PIN, OUTPUT);
    analogWrite(MOTOR_PIN, 0);

    // buffer initialization
    for(int k = 0; k < windowSize; k++) {
        edaBuffer[k] = (float)analogRead(EDA_PIN);
    }
}

void loop() {
    // read the raw EDA value
    int rawValue = analogRead(EDA_PIN);

  if (!isSensorConnected(rawValue)) {
        Serial.println("sensor disconnected, out of range");
        delay(1000);
        return; 
    }

   // collect new sample
  edaBuffer[bufferIndex] = (float)analogRead(EDA_PIN);
  bufferIndex = (bufferIndex + 1) % windowSize;

    // compute EDA features: mean, std, max, min
    float sum = 0;
    float eda_max = edaBuffer[0];
    float eda_min = edaBuffer[0];

    for (int i = 0; i < windowSize; i++) {
        float val = edaBuffer[i];
        sum += val;
        if (val > eda_max) eda_max = val;
        if (val < eda_min) eda_min = val;
    }
    
    float eda_mean = sum / windowSize;

    float sumSq = 0;
    for (int i = 0; i < windowSize; i++) {
        sumSq += pow(edaBuffer[i] - eda_mean, 2);
    }
    float eda_std = sqrt(sumSq / windowSize);

    // retrieve heart rate from BVP sensor (for demonstration, using a fixed value)
    float bvp_hr_bpm = 75.0f; 

    // create feature array for prediction
    float live_features[] = { eda_mean, eda_std, eda_max, eda_min, bvp_hr_bpm };

    float mean_eda_mean = 512.0f; 
    float std_eda_mean  = 50.0f;
    float mean_eda_std = 512.0f;
    float std_eda_std  = 50.0f;
    float mean_eda_max = 512.0f;
    float std_eda_max  = 50.0f;
    float mean_eda_min = 512.0f;
    float std_eda_min  = 50.0f;
    float mean_bvp_hr = 75.0f;
    float std_bvp_hr  = 10.0f;

    live_features[0] = (eda_mean - mean_eda_mean) / std_eda_mean;
    live_features[1] = (eda_std - mean_eda_std) / std_eda_std;
    live_features[2] = (eda_max - mean_eda_max) / std_eda_max;
    live_features[3] = (eda_min - mean_eda_min) / std_eda_min;
    live_features[4] = (bvp_hr_bpm - mean_bvp_hr) / std_bvp_hr;

    // run prediction using the trained model
    int prediction = randomForest.predict(live_features);

    // display outputs
    Serial.print("Mean: "); Serial.print(eda_mean);
    Serial.print(" | Max: "); Serial.print(eda_max);
    Serial.print(" | Min: "); Serial.print(eda_min);
    Serial.print(" | HR: "); Serial.print(bvp_hr_bpm);
    Serial.print(" | Prediction: "); Serial.println(prediction);

    if(prediction == 1) {
       Serial.println("stress detected");
       Serial.println("triggering biofeedback, inhale");
      for(int k = 0; k <= 200; k += 10) {
       analogWrite(MOTOR_PIN, k);
       delay(100);
     }

      delay(1000);
  
    Serial.println("triggering biofeedback, exhale");
     for(int k = 200; k >= 0; k -= 10) {
       analogWrite(MOTOR_PIN, k);
       delay(100);
     }

     analogWrite(MOTOR_PIN, 0);

     delay(5000);

    } else {
       Serial.println("baseline");
       analogWrite(MOTOR_PIN, 0);
    }

    delay(5000); 
}