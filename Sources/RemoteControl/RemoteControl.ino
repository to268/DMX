#include <SPI.h>
#include <WiFly.h>
#include "Credentials.h"

#define POT1 A0
#define POT2 A2
#define VA_CHANNEL 12

byte server[] = {192, 168, 1, 1};
WiFlyClient client(server, 2369);

void connection(void);
void send_wifi(String channel, String value);
void send_data(String channel, String value);
void read_values(int *pot1, int *pot2);

void setup() {
  pinMode (POT1, INPUT);
  pinMode (POT2, INPUT);
  pinMode (VA_CHANNEL, INPUT_PULLUP);

  Serial.begin(9600);

  WiFly.begin();

  if (!WiFly.join(ssid, passphrase, true)) {
    Serial.println("Association failed.");
    while (1) {
      // Hang on failure.
    }
  }

  Serial.println("connecting...");
  connection();
}

void loop() {
  int pot1 = 0;
  int pot2 = 0;

  read_values(&pot1, &pot2);
  String channel = String(VA_CHANNEL);
  String value = String(pot2);

  if (client.available())
    send_data(channel, value);

  delay(1000);
}

void connection(void) {
  if (client.connect()) {
    Serial.println("connected");
  } else {
    Serial.println("connection failed");
  }
}

void send_wifi(String channel, String value) {
  client.print(channel);
  client.print(" ");
  client.println(value);

  // Check ACK!
  char c = client.readChar();
  char c2 = client.readChar();
  client.stop();
  if (c == "O" && c2 == "K") {
      Serial.println("ACK Recieved !");
  } else {
      Serial.println("Error Sending the data !");
      delay(1000);
      send_wifi(channel, value);
  }
}

void send_data(String channel, String value) {
  connection();
  send_wifi(channel, value);
}

void read_values(int *pot1, int *pot2) {
  pot1 = (analogRead(POT1)) / 2;
  Serial.print("pot1: ");
  Serial.println (*pot1);
  delay (500);

  pot2 = (analogRead(POT2)) / 4;
  Serial.print("pot2: ");
  Serial.println (*pot2);
  Serial.println(!digitalRead(VA_CHANNEL));

  if (!digitalRead(VA_CHANNEL)) {
   Serial.println("sending values: ");
   Serial.println(*pot1);
   Serial.println(*pot2);
  }

  delay (100);
}
