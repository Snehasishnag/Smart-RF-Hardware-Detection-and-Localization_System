#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println("NRF NOT FOUND!");
    while (1);
  }

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  Serial.println("Transmitter Ready");
}

void loop() {
  char text[] = "TOOLKIT01";

  bool ok = radio.write(&text, sizeof(text));

  if (ok)
    Serial.println("Sent");
  else
    Serial.println("Failed");

  delay(500);
}