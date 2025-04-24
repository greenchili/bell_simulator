/*
Bell Receiver
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(2400); // Match this to your software's expected rate (Abel/Virtual Belfry)
  radio.begin();
  radio.setDataRate(RF24_250KBPS); // Must match transmitter
  radio.setPALevel(RF24_PA_MIN);
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = {0}; // Zero out buffer
    radio.read(&text, sizeof(text));

    // Only send the first character, no newline or carriage return
    Serial.write(text[0]);
  }
}
