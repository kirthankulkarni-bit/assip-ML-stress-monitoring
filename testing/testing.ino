#include "StressModel (1).h"

using namespace Eloquent::ML::Port;
stressModel mlModel;

void setup() {
    Serial.begin(115200);
    delay(4000); // Crucial: Gives the C3's native USB time to connect before printing
    
    Serial.println("\n--- NEW CACHE BREAK COMPILE ---");
    Serial.println("If you do not see this exact text, the IDE used old cached code.");
    
    // Dummy data matching your 5 features (mean, std, max, min, bvp_hr_bpm)
    float dummy_features[] = {0.1f, 0.2f, 0.5f, 0.01f, 75.0f};
    
    Serial.println("Running inference...");
    int prediction = mlModel.predict(dummy_features);
    
    Serial.print("Prediction ID: "); 
    Serial.println(prediction);
    Serial.println("Model ran successfully!");
}

void loop() {
    // Do nothing
    delay(1000);
}