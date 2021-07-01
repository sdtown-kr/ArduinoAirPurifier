#include <SoftwareSerial.h>
#define no_dust 0.35

//BT
int blueTx = 6;
int blueRx = 5;
SoftwareSerial mySerial(blueTx, blueRx);

//LED
int r = 11;
int g = 12;
int b = 13;

//fan
int fan1 = 8;
int fan2 = 9;
int fan3 = 10;

int dustout = A0;
int v_led = 7;
float vo_value = 0;
float sensor_voltage = 0;
float dust_density = 0;

int cnt = 0;

int value = 0;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(v_led,OUTPUT);
  pinMode(fan1, OUTPUT);
  pinMode(fan2, OUTPUT);
  pinMode(fan3, OUTPUT);
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
}

void RGB_color (int rv, int gv, int bv) {
  analogWrite(r, rv);
  analogWrite(g, gv);
  analogWrite(b, bv);
}

void loop() {
  if (mySerial.available()) {
    digitalWrite(v_led,LOW);
    delayMicroseconds(280);
    vo_value = analogRead(dustout);
    delayMicroseconds(40);
    digitalWrite(v_led,HIGH);
    delayMicroseconds(9680);

    sensor_voltage = get_voltage(vo_value);
    dust_density = get_dust_density(sensor_voltage);    

    mySerial.println(dust_density);
    Serial.print("Dust Density = ");
    Serial.print(dust_density);
    Serial.println(" [ug/m^3]");
      
    value = mySerial.read();
    delay(1000);

    if (dust_density > 500) {
      RGB_color(255,0,0);
      delay(2000);
    }
    else if (dust_density > 200) {
      RGB_color(255,255,0);
    }
    else {
      RGB_color(0,255,0);
    }

    if (value == 'h') {
      analogWrite(fan1, 210);
      analogWrite(fan2, 210);
      analogWrite(fan3, 210);
      delay(10000);
      value=0;
      return value;
    }
    else if (value == 'w') {
      analogWrite(fan1, 255);
      analogWrite(fan2, 255);
      analogWrite(fan3, 0);
      delay(10000);
      value=0;
      return value;
    }

    if (dust_density >= 500) {
      analogWrite(fan1, 210);
      analogWrite(fan2, 210);
      analogWrite(fan3, 210);
    }
    else {
      analogWrite(fan1, 255);
      analogWrite(fan2, 255);
      analogWrite(fan3, 0);
    }
  }
  else {
    digitalWrite(v_led,LOW);
    delayMicroseconds(280);
    vo_value=analogRead(dustout);
    delayMicroseconds(40);
    digitalWrite(v_led,HIGH);
    delayMicroseconds(9680);
    delay(1000);

    sensor_voltage = get_voltage(vo_value);
    dust_density = get_dust_density(sensor_voltage);

    Serial.print("Dust Density = ");
    Serial.print(dust_density);
    Serial.println(" [ug/m^3]");

    if (dust_density > 500) {
      RGB_color(255,0,0);
      delay(2000);
    }
    else if (dust_density > 200) {
      RGB_color(255,255,0);
    }
    else {
      RGB_color(0,255,0);
    }

    if (dust_density >= 500) {
      analogWrite(fan1, 210);
      analogWrite(fan2, 210);
      analogWrite(fan3, 210);
    }
    else {
      analogWrite(fan1, 255);
      analogWrite(fan2, 255);
      analogWrite(fan3, 0);
    }
  }
}
float get_voltage(float value) {
  float V = vo_value * 5.0 / 1024;
  return V;
  }
float get_dust_density(float voltage) {
  float dust = (voltage-no_dust) / 0.005;
  if (dust<0) {
    dust = dust + 65;
  }
  return dust;
  }
