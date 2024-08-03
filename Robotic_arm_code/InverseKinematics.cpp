/*
  Led.h - Library for flashing led's
  Created by Jorge E. Sánchez
*/
#include "InverseKinematics.h"

const int shoulderOffset = -4;
const int elbowOffset = -8;
const byte twistRistOffset = 16;
const byte pitchWristOffset = 12;

//Begining of angle calculations according to x, y, z coordinates
float rotationAngle = 90;
float shoulderAngle = 90;
float elbowAngle = 90;
float wristPitchAngle = 90;

int gripperAngle = 150;
float gripperLength = 8.5;

double hST1; //Hypotenuse of Subtriangle 1
double hST2; //Hypotenuse of Subtriangle 2
double K; //Angle located in  Sub Triangle 2 and Triangle 1 (angle mirrored)
double ak; //Line which is divisor in between Triangle 1 and Triangle 2
double BT1;
double AT1;
double AT2;
double BT2;
double CT2;
//End of angle calculations according to x, y, z coordinates


InverseKinematics::InverseKinematics(double armLength, double forearmLength){
  _armLength = armLength;
  _forearmLength = forearmLength;
}

void InverseKinematics::calculate_IK(float x, float y, float z) {
  _x = x;
  _y = y;
  _z = z;

//Beginning of calculations to get the current length of the gripper (as it oscillates depending on it's state)
  gripperLength = (-0.357 * gripperAngle + 150)/10;
  //Gripper limits
  //if(gripperAngle >= 180){gripperAngle = 180;}
  if(gripperAngle <= 70){gripperAngle = 70;}
//End of calculations to get the current length of the gripper (as it oscillates depending on it's state)


//Beginning of calculations to get the angles required for the servo 2,3,4
  hST1 = sqrt(sq(x) + sq(y));
  hST2 = sqrt(sq(hST1) + sq(z));
  K = asin(abs(z/hST2));
  ak = sqrt(sq(gripperLength) +  sq(hST2) - 2*(gripperLength)*(hST2) * cos(K));
  BT1 = acos((sq(gripperLength) + sq(ak) - sq(hST2)) / (2 * gripperLength * ak));
  AT1 = PI - (BT1 + K);
  AT2 = acos((sq(12) + sq(12) - sq(ak)) / (2 * 12 *  12));
  
  //CHECK ak

  //Check from now and on
  BT2 = acos((sq(23.03) + sq(12) - sq(12)) / (2 * 23.03 * 12));
  CT2 = PI - (AT2 + BT2);

  shoulderAngle = 180 - (((K + AT1 + CT2) * (180 / PI)) / 2);
  elbowAngle = 180 - ((AT2) * (180 / PI)) - 90;
  wristPitchAngle = (BT2 + BT1) * (180 / PI) +90;
//End of calculations to get the angles required for the servo 2,3,4


  Serial.println(ak);
}

double InverseKinematics::servo_1_angle(){
  return 90;
}

double InverseKinematics::servo_2_angle(){
  return shoulderAngle + shoulderOffset;
}

double InverseKinematics::servo_3_angle(){
  return elbowAngle + elbowOffset;
}

double InverseKinematics::servo_4_angle(){
  return 90 + twistRistOffset;
}

double InverseKinematics::servo_5_angle(){
  return wristPitchAngle + pitchWristOffset;
}

double InverseKinematics::servo_6_angle(){
  return gripperAngle; //70-172
}

