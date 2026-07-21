# include "StressModel.h"

using namespace Eloquent::ML::Port;

StressModel randomForest;

#define GSR_PIN A0
#define SENSOR_MIN_THRESHOLD 10   
#define SENSOR_MAX_THRESHOLD 4085

void setup() {
    Serial.begin(115200);
    randomForest.begin();
}

void loop() {
    float live_features[] = {2.3, 1.7, 2.7, 0.5, 1.2}; 

    int prediction = randomForest.predict(live_features);

    Serial.print("prediction class: ");
    Serial.println(prediction);

    if(prediction == 2) {
        Serial.println("stress detected");
    } else {
        Serial.println("baseline");
    }

    delay(5000); 
}