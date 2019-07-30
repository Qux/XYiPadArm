//OSC
#include <WiFi.h>
#include <WiFiUDP.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

//PCA9685
//#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Metro.h>

//Metro
#include <Metro.h>

//Setup WiFi
constexpr char ssid[] = "**";
constexpr char pass[] = "**";
WiFiUDP udp;
const IPAddress REMOTE_IP(224, 0, 0, 1);     // remote IP (not needed for receive)
const unsigned int REMOTE_PORT = 50000;          // remote port (not needed for receive)
const unsigned int LISTEN_PORT = 50000;       // local port to listen for UDP packets (here's where we send the packets)

/* If you want to use static ip, uncomment below. */
//#define USE_STATIC_IP;
//const IPAddress IP(192, 168, 1, 32);
//const IPAddress GATEWAY(192,168, 1, 1);
//const IPAddress SUBNET(255, 255, 255, 0);
//const IPAddress DNS(192, 168, 1, 1);

OSCErrorCode error;
constexpr int TIME_ZONE = -14;

/* PCA9685 settings */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/* Metro */
Metro pingMetro = Metro(1000);

/* Servo */
constexpr unsigned char SERVO_PIN = 0;
constexpr unsigned int SERVO_ANGLE_MIN = 150;
constexpr unsigned int SERVO_ANGLE_WIDTH = 330;
float angle, current_angle;
int angle_pwm;


constexpr unsigned char ROTATE_PIN = 1;
constexpr unsigned int ROTATION_STOP = 300;
constexpr unsigned int ROTATION_RANGE = 100;
float current_spd, spd;
int rotation_pwm;




void setupDriver() {
    pwm.begin();
    pwm.setPWMFreq(50); // This is the maximum PWM frequency
    Wire.setClock(400000);
    angle = 0.5;
    current_angle = 0.5;
    spd = 0.0;
    current_spd = 0.0;
}

void setupWiFi() {
#ifdef USE_STATIC_IP
    WiFi.config(IP, GATEWAY, SUBNET, DNS);
#endif
    // Connect to WiFi network
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }
    udp.begin(LISTEN_PORT);
    configTime(TIME_ZONE * 3600L, 0,  "time.google.com", "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
}

void sendMessage(OSCMessage &msg) {
    udp.beginPacket(REMOTE_IP, REMOTE_PORT);
    msg.send(udp);
    udp.endPacket();
    msg.empty();
}

void ping() {
    OSCMessage mes("/ping");
    mes.add(1);
#ifndef USE_STATIC_IP
    mes.add(WiFi.localIP()[0]);
    mes.add(WiFi.localIP()[1]);
    mes.add(WiFi.localIP()[2]);
    mes.add(WiFi.localIP()[3]);
#endif
    sendMessage(mes);
}

void updateAngle() {
    current_angle += (angle - current_angle) * 0.1;
    angle_pwm = SERVO_ANGLE_WIDTH * current_angle + SERVO_ANGLE_MIN;
    pwm.setPWM(SERVO_PIN, 0, angle_pwm);
}

void setAngle(OSCMessage &msg) {
    angle = msg.getFloat(0);
}

void updateSpd() {
    current_spd += (spd - current_spd) * 0.1;
    rotation_pwm = ROTATION_RANGE * current_spd + ROTATION_STOP;
    pwm.setPWM(ROTATE_PIN, 0, rotation_pwm);
}

void setSpd(OSCMessage &msg) {
    spd = msg.getFloat(0);
}

void updateOsc() {
    //Check receive
    OSCMessage msgIn;
    int size = udp.parsePacket();

    if (size > 0) {
        while (size--) {
            msgIn.fill(udp.read());
        }
        if (msgIn.hasError()) {
            error = msgIn.getError();
        } else {
            msgIn.dispatch("/setSpd", setSpd);
            msgIn.dispatch("/setAngle", setAngle);
        }
    }
}

//void updateLoop(void* arg) {
//    while (true) {
//
//    }
//}


void setup() {
    setupWiFi();
    setupDriver();
}



void loop() {
    updateOsc();
    updateSpd();
    updateAngle();

    if (pingMetro.check()) {
        ping();
    }
}
