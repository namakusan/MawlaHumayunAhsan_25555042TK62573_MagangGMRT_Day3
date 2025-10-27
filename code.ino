#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ESP32Servo.h>

Adafruit_MPU6050 mpu;

Servo pitch1;
Servo pitch2;
Servo yaw1;
Servo roll1;
Servo roll2;

int motionSensor = 27;

void setup() {
  Serial.begin(115200);

  // memastikan sensor gyronya hidup
  if (!mpu.begin()) { 
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  Serial.println("Ready");

  // menerima sinyal dari motion sensor
  pinMode(motionSensor, INPUT);

  // mengattach servo apa mendapatkan pin apa
  pitch1.attach(13);
  pitch2.attach(14);
  yaw1.attach(23);
  roll1.attach(17);
  roll2.attach(16);

  // posisi inisial semua servo (menghadap ketengah)
  pitch1.write(90);
  pitch2.write(90);
  yaw1.write(90);
  roll1.write(90);
  roll2.write(90);

  delay(100);
}

float tempx = 0;
float tempy = 0;
float tempz = 0;

void loop() {
  // mendapatkan hasil bacaan dari sensor
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // untuk motion sensor
  int sensor_output;
  sensor_output = digitalRead(motionSensor);
  if( sensor_output == HIGH ){ //jika motion sensor mendeteksi pergerakan,
    pitch1.write(45); // ganti semua posisi menjadi -45 derajat dari titik awal
    pitch2.write(45);
    yaw1.write(45);
    roll1.write(45);
    roll2.write(45);
    delay(2000);  // delay 2 detik
    pitch1.write(90); // kembali lagi ke posisi inisial
    pitch2.write(90);
    yaw1.write(90);
    roll1.write(90);
    roll2.write(90);
    delay(2000); // delay lagi 2 detik
  }

  // perhitungan untuk gyro

  // roll (x-axis)
  if (g.gyro.x > tempx){ // jika angle roll lebih besar dari posisi status quo,
    roll1.write(180); // tabrak ke kanan
    roll2.write(180);
  } else if (g.gyro.x < tempx){ //jika angle roll lebih kecil dari posisi status quo,
    roll1.write(0); // tabrak ke kiri
    roll2.write(0);
  } else{ // jika sama seperti status quo,
    delay(1000); // diam untuk 1 detik
    roll1.write(90); // kembali ke posisi initial servo
    roll2.write(90);
  }

  // mendefinisikan status quo sebagai rotasi stabil yang terjadi sekarang
  tempx = g.gyro.x;

  // ulang hal yang sama untuk z dan y
  if (g.gyro.z > tempz){
    pitch1.write(180);
    pitch2.write(180);
  } else if (g.gyro.z < tempz){
    pitch1.write(0);
    pitch2.write(0);
  } else{
    delay(1000);
    pitch1.write(90);
    pitch2.write(90);
  }

  tempz = g.gyro.z;

  if (g.gyro.y > tempy){
    yaw1.write(180);
  } else if (g.gyro.y < tempy){
    yaw1.write(0);
  } else{
    delay(1000);
    yaw1.write(90);
  }

  tempy = g.gyro.y;

  delay(100);
}