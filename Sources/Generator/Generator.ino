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
#include <DmxSimple.h>
#include <Wire.h>
#include <stdint.h>

#define ADDRESS 0b0000100

void i2c_init(void);
void send_value(uint16_t channel, uint8_t value);

void setup() {
  DmxSimple.usePin(3);
  DmxSimple.maxChannel(10);
}

void loop() {
  i2c_init();

  uint8_t channel_high;
  uint8_t channel_low;
  uint16_t channel;
  uint8_t value;

  channel_high = Wire.read();
  channel_low = Wire.read();
  value = Wire.read();

  Serial.print("channel_high: ");
  Serial.println(channel_high);
  Serial.print("channel_low: ");
  Serial.println(channel_low);
  Serial.print("value: ");
  Serial.println(value);

  channel = (channel_high << 8) | channel_low;

  Serial.print("channel: ");
  Serial.println(channel);

  send_value(channel, value);
  delay(1000);
}

void i2c_init(void) {
  Wire.begin(ADDRESS);
  Serial.println("I2C communication Ready");
}

void send_value(uint16_t channel, uint8_t value) {
  DmxSimple.write(channel, value);
  Serial.println("Send values through I2C");
}
