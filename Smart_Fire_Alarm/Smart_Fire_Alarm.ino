#include <SoftwareSerial.h>

#define FLAME_SENSOR_PIN A0   // Define the pin for the flame sensor
#define SMOKE_SENSOR_PIN A1   // Define the pin for the smoke sensor (gas sensor in this case)
#define redLed 13
#define greenLed 12
#define buzzer 11
unsigned long previousMillis = 0;  // Variable to store the start time of sleep
unsigned long sleepDuration = 500;  // Sleep duration in milliseconds (5 seconds)
int ledState = LOW;

const long interval = 5000;  // Interval at which to blink (milliseconds)
 // Current state of the LED

// Define the external TX and RX pins for the Bluetooth module
const int bluetoothTx = 2; // Define TX pin
const int bluetoothRx = 3; // Define RX pin

// Create a SoftwareSerial object
SoftwareSerial bluetoothSerial(bluetoothTx, bluetoothRx);

void setup() {
  Serial.begin(9600);        // Initialize serial communication
  pinMode(FLAME_SENSOR_PIN, INPUT);  // Set flame sensor pin as input
  pinMode(SMOKE_SENSOR_PIN, INPUT);  // Set smoke sensor pin as input
  pinMode(redLed , OUTPUT);
  pinMode(greenLed , OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Initialize Bluetooth communication
  bluetoothSerial.begin(9600); // Change baud rate to match your Bluetooth module
}

void loop() {
  int flameValue = analogRead(FLAME_SENSOR_PIN); // Read flame sensor value
  int smokeValue = analogRead(SMOKE_SENSOR_PIN); // Read smoke sensor value (interpreted as gas sensor)

  // Print flame and smoke sensor values to Serial Monitor
  displaySensorValues(flameValue, smokeValue);

  // Check if flame or smoke is detected and trigger corresponding alerts
  if (smokeValue > 100 && flameValue < 100) {
    digitalWrite(greenLed, LOW);
    Serial.println("Alert: Smoke or Gas and Flame Detected");
    sendBluetoothAlert("Alert: Smoke or Gas and Flame Detected");
    RedLed();
  } 
  else if (smokeValue > 100 && flameValue > 100) {
    digitalWrite(greenLed, LOW);
    Serial.println("Alert: Smoke or Gas Detected");
    sendBluetoothAlert("Alert: Smoke or Gas Detected");
    RedLed();
  } 
  else if (smokeValue < 100 && flameValue < 100) {
    digitalWrite(greenLed, LOW);
    Serial.println("Alert: Flame Detected");
    sendBluetoothAlert("Alert: Flame Detected");
    RedLed();
  }
  else {
    digitalWrite(greenLed, HIGH);
    
  }
}

void displaySensorValues(int flameValue, int smokeValue) {
  unsigned long currentMillis = millis();
  static int prevFlameValue = -1;

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (flameValue != prevFlameValue) {
      Serial.print("Flame Sensor Value: ");
      Serial.println(flameValue);
      Serial.print("Smoke Sensor Value: ");
      Serial.println(smokeValue);
      prevFlameValue = flameValue;
    }
  }
}

void sendBluetoothAlert(const char* message) {
  bluetoothSerial.println(message); // Send the message via Bluetooth
}

void RedLed() {
  digitalWrite(redLed, HIGH);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  digitalWrite(redLed, LOW);
  delay(100);
  digitalWrite(buzzer, LOW);
}
