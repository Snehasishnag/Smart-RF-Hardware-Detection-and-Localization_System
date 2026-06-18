#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>

#define CE_PIN 4
#define CSN_PIN 5
#define BUZZER 27
void beep() {
  ledcAttach(BUZZER, 2000, 8);  // 2kHz tone
  ledcWrite(BUZZER, 128);       // 50% duty cycle
  delay(300);
  ledcWrite(BUZZER, 0);         // Stop
}

RF24 radio(CE_PIN, CSN_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte address[6] = "00001";

unsigned long lastReceived = 0;

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  // I2C
  Wire.begin(21, 22);

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting...");

  // NRF SPI
  SPI.begin(18, 19, 23, 5);

  if (!radio.begin()) {
    Serial.println("NRF NOT FOUND");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("NRF ERROR");

    while (1);
  }

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Receiver Ready");

  Serial.println("Receiver Ready");
}

void loop() {

  if (radio.available()) {

    char text[32] = "";

    radio.read(&text, sizeof(text));

    lastReceived = millis();

    Serial.print("Detected: ");
    Serial.println(text);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HW DETECTED");

    lcd.setCursor(0, 1);
    lcd.print(text);

    beep();
  }

  // Lost after 2 seconds
  if (millis() - lastReceived > 2000) {

    //digitalWrite(BUZZER, LOW);

    lcd.setCursor(0, 0);
    lcd.print("HW LOST      ");

    lcd.setCursor(0, 1);
    lcd.print("Searching... ");
  }
}