/*
 * Copyright © 2021 to268 and contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the organization nor the
 * names of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY yourname ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL yourname BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
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
