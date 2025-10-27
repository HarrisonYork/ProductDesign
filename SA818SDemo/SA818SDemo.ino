#include <SoftwareSerial.h>

// Use software serial to avoid conflict with USB serial
SoftwareSerial sa818Serial(10, 11); // Arduino RX is 10, Arduino TX is 11 (voltage divider)

#define PD_PIN 3
#define PTT_PIN 2

void setup() {
  pinMode(PD_PIN, OUTPUT);
  pinMode(PTT_PIN, OUTPUT);

  // Enable module (PD=HIGH)
  digitalWrite(PD_PIN, HIGH);
  digitalWrite(PTT_PIN, HIGH); // RX mode

  Serial.begin(9600);        // Serial Monitor
  sa818Serial.begin(9600);   // SA818S UART

  delay(500);
  Serial.println("SA818S Command Bridge Ready");
  Serial.println("Type AT commands (end with Enter).");
}

void loop() {
  // Forward commands from Serial Monitor → SA818
  if (Serial.available()) {
    char c = Serial.read();
    sa818Serial.write(c);
  }

  // Forward responses from SA818 → Serial Monitor
  if (sa818Serial.available()) {
    char c = sa818Serial.read();
    Serial.write(c);
  }

  // Transmit for 2 seconds
  // digitalWrite(PTT_PIN, LOW);
  // delay(2000);
  // // Back to receive for 3 seconds
  // digitalWrite(PTT_PIN, HIGH);
  // delay(3000);
}
