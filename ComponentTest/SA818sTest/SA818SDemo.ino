#include <SoftwareSerial.h>

// Wiring notes:
// sa818Serial(rxPin, txPin) -> parameters are (ArduinoRxPin, ArduinoTxPin)
// Connect: SA818 TX -> Arduino pin 8
//          SA818 RX <- Arduino pin 9 (use voltage divider to drop 5V -> 3.3V)
// Common GND required.
SoftwareSerial sa818Serial(10, 11); // RX=10, TX=11

#define PD_PIN 7
// #define PTT_PIN 6 // if you later use PTT, define and wire appropriately

// If your board has a hardware Serial1, prefer Serial1 (Mega, Leonardo variants, etc.)

void setup() {
  pinMode(PD_PIN, OUTPUT);

  // Enable module (confirm PD polarity in datasheet)
  digitalWrite(PD_PIN, HIGH);

  // Give module time to fully power up
  delay(2000);

  Serial.begin(9600);        // Serial Monitor
  Serial.println("Serial started.");
  
  sa818Serial.begin(9600);   // SA818S UART (SoftwareSerial can be flaky at higher baud)

  Serial.println();
  Serial.println("SA818S Command Bridge Ready");
  Serial.println("Type AT commands (make sure Serial Monitor line ending is set correctly)");
}

void loop() {
  // Forward everything that's currently available from Serial Monitor → SA818
  while (Serial.available()) {
    char c = Serial.read();
    // Optionally translate line endings from '\n' to '\r' or '\r\n' depending on module needs:
    // if (c == '\n') sa818Serial.write('\r'); else sa818Serial.write(c);
    sa818Serial.write(c);
  }

  // Forward responses from SA818 → Serial Monitor (consume all available bytes)
  while (sa818Serial.available()) {
    char c = sa818Serial.read();
    Serial.write(c);
  }

  // String command = "AT+DMOSETGROUP=0,462.5625,462.5625,0000,8,0000\r\n";
  // send_command(command);

  // Optional: add a very small yield to let background tasks run
  // (not required, but helps on some boards)
  delay(1000);
}

void send_command(String command) {
  // Send the string to the SA818 once
  sa818Serial.print(command);
  Serial.println("Sent to SA818:");
  Serial.println(command);

  // Wait for and print the response
  unsigned long startTime = millis();
  while (millis() - startTime < 2000) {  // wait up to 2 seconds
    if (sa818Serial.available()) {
      char c = sa818Serial.read();
      Serial.write(c);
    }
  }
}
