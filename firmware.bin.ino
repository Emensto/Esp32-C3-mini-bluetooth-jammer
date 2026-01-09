#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define SCK_PIN   4
#define MISO_PIN  5
#define MOSI_PIN  6
#define CE_PIN    20
#define CSN_PIN   21

// ===== Gpio 7  =====
#define BUTTON_PIN 7

bool active = false;
bool lastButtonState = HIGH;
// ======================

SPIClass spi(FSPI);
RF24 radio(CE_PIN, CSN_PIN);

void setup() {
  Serial.begin(115200);
  delay(500);

  // ===== Button lang  =====
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // ======================

  spi.begin(SCK_PIN, MISO_PIN, MOSI_PIN);

  Serial.println("Initializing nRF24L01+...");

  if (!radio.begin(&spi)) {
    Serial.println("nRF24L01+ initialization failed!");
    while (1);
  }

  Serial.println("nRF24L01+ initialized!");

  // ---- ORIGINAL CODE (UNCHANGED) ----
  radio.setAutoAck(false);
  radio.setRetries(0, 0);
  radio.setPALevel(RF24_PA_MAX, true);
  radio.setDataRate(RF24_2MBPS);
  radio.setCRCLength(RF24_CRC_DISABLED);
  radio.setChannel(0);
  radio.stopListening();
  // radio.startConstCarrier(...)  // ← DI NA AGAD START
  // ----------------------------------


  randomSeed(analogRead(0));
}

void loop() {

  // ===== DAGDAG LANG (BUTTON TOGGLE) =====
  bool buttonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && buttonState == LOW) {
    delay(40);
    if (digitalRead(BUTTON_PIN) == LOW) {

      active = !active;

      if (active) {
        Serial.println("ACTIVATED");
        radio.startConstCarrier(RF24_PA_MAX, 45); // ORIGINAL CALL
      } else {
        Serial.println("STOPPED");
        radio.stopConstCarrier(); // DAGDAG LANG
      }
    }
  }
  lastButtonState = buttonState;
  // =====================================

  // ---- ORIGINAL LOOP LOGIC (GUARDED) ----
  if (active) {
    byte channel = random(0, 126);
    radio.setChannel(channel);
    delayMicroseconds(random(30, 100));
  }
}
