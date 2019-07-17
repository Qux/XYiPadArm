#include "BluetoothSerial.h"
#include <StringAsOSC.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Metro.h>

/* BT Settings */
BluetoothSerial bt;
constexpr char DEVICE_NAME[] = "ESP32";

/* PCA9685 settings */
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/* User variables */
Metro metro = Metro(1000);

inline void setRotation(StringAsOSC &mes) {
//  static int spd = mes.getArgAsInt(0);
  pwm.setPWM(0, 0, mes.getArgAsInt(0)); //Vertical rotation
  pwm.setPWM(1, 0, mes.getArgAsInt(1)); //Horizontal rotation
}

inline void readBT(void* param) {
//  String d = bt.readStringUntil('\n');
  StringAsOSC mes;
  mes.fill(bt.readStringUntil('\n'));
  mes.bind("/rotate", setRotation);
}

void setup() {
  //    Serial.begin(115200);
  //  Serial.println("Boot");
  pwm.begin();
  pwm.setPWMFreq(50); // This is the maximum PWM frequency

  bt.begin(DEVICE_NAME);
}



void loop() {
  if (bt.available()) {
    xTaskCreatePinnedToCore(readBT, "readBT", 4096, NULL, 1, NULL, 0);
  }

  if (metro.check()) {
    StringAsOSC mes;
    mes.setAddress("/bar");
    mes.addArg(1);
    mes.addArg(0.01);
    bt.println(mes.getString());
  }
  yield();
}
