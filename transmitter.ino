/*
Bell Transmitter with Potentiometer Selector (1–12)
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
char text[3]; // To hold up to "12" + null terminator

const int IRSensor = 2; // IR sensor on D2
const int potPin = A0;

unsigned long lastTrigger = 0;
const unsigned long triggerDelay = 400; // milliseconds

void setup() {
  Serial.begin(9600); // Debugging

  pinMode(IRSensor, INPUT);

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  // Map potentiometer to bell number (1–12)
  int potVal = analogRead(potPin); // 0–1023
  int bellNum = map(potVal, 0, 1023, 1, 12);

  // Convert number to ASCII string
  snprintf(text, sizeof(text), "%d", bellNum); // e.g. "1", "10", etc.

  int sensorStatus = digitalRead(IRSensor);
  if (sensorStatus == 0 && (millis() - lastTrigger > triggerDelay)) {
    Serial.print("IR sensor triggered. Bell: ");
    Serial.println(text);

    bool success = radio.write(&text, sizeof(text));
    if (!success) {
      Serial.println("Transmission failed!");
    }

    lastTrigger = millis();
  }
}
