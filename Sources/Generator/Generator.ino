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
