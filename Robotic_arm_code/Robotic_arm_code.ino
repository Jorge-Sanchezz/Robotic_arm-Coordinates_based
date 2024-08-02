/*Written by Jorge Sánchez Encinas*/
#include <Arduino.h>
#include <ESP32Servo.h>
#include "Led.h"
#include "InverseKinematics.h"

Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;
Servo servo_5;
Servo servo_6;

Led led(15);
InverseKinematics InverseKinematics(12, 12); 

void setup() {
  led.begin();
  Serial.begin(115200);
 
  servo_1.attach(2);
  servo_2.attach(4);
  servo_3.attach(5);
  servo_4.attach(18);
  servo_5.attach(19);
  servo_6.attach(21);
}

void loop() {
  //angleCalculations();
  //moveServoMotorWithCoordinates();
  //ledBlink();
  led.stand_by_mode();

  InverseKinematics.calculate_IK(9.97, 17.34, 20.00);
  //Serial.println(InverseKinematics.servo_2_angle());

  servo_1.write(InverseKinematics.servo_1_angle());
  servo_2.write(InverseKinematics.servo_2_angle());
  servo_3.write(InverseKinematics.servo_3_angle());
  
  servo_4.write(InverseKinematics.servo_4_angle());
  servo_5.write(InverseKinematics.servo_5_angle());
  servo_6.write(InverseKinematics.servo_6_angle());
}