
#define MOTOR_PIN D10

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(MOTOR_PIN, OUTPUT);
  analogWrite(MOTOR_PIN, 0);
  delay(2000);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("ramping up motor vibration");
  for(int k = 0; k <= 200; k += 10) {
    analogWrite(MOTOR_PIN, k);
    delay(100);
  }

  delay(1000);
  
  Serial.println("ramping down motor vibration");
  for(int k = 200; k >= 0; k -= 10) {
    analogWrite(MOTOR_PIN, k);
    delay(100);
  }

  analogWrite(MOTOR_PIN, 0);

  delay(5000);
}
