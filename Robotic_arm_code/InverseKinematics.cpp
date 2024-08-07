/*
  Led.h - Library for flashing led's
  Created by Jorge E. Sánchez
*/
#include "InverseKinematics.h"

const int shoulderOffset = 0;//-4;
const int elbowOffset = 0;//-8;
const byte twistRistOffset = 8;
const byte pitchWristOffset = 21;//25;

//Begining of angle calculations according to x, y, z coordinates
float rotationAngle = 90;
float shoulderAngle = 90;
float elbowAngle = 90;
float wristPitchAngle = 90;

int gripperAngle = 170;
float gripperLength = 8.5;

byte currentCuadrant; //(I, II, III, IV)

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

void InverseKinematics::calculate_IK(float x, float y, float z, int gripperState) {
  _x = x;
  _y = y;
  _z = z;
  _gripperState = gripperState;

//Beginning of calculations to get the current length of the gripper (as it oscillates depending on it's state)
  if(_gripperState > 5){
    _gripperState = 5;
  }
  else if(_gripperState < 0){
    _gripperState = 0;
  }

  gripperAngle = (21 * _gripperState) + 65;

  gripperLength = (-(gripperAngle / 27) + (360 / 27))+1.5;
  //Gripper limits
  if(gripperAngle >= 170){gripperAngle = 170;}
  else if(gripperAngle <= 65){gripperAngle = 65;}
//End of calculations to get the current length of the gripper (as it oscillates depending on it's state)


//Beginning of localization of CUADRANT on the plane (I, II, III, IV)
    rotationAngle = 180 * (atan(abs(y/x))) / PI;
    if(_x>=0){
      if(_y>0){    //If angle is located on the ++ CUADRANT of the plane CUADRANT I
        currentCuadrant = 1;
      }
      else if(_y<=0){   //If angle is located on the +- CUADRANT of the plane CUADRANT IV 
        rotationAngle = 180 - rotationAngle;
        currentCuadrant = 4;
      }
    }
    else if(_x<0){
      if(+y>=0){   //If angle is located on the -+ CUADRANT of the plane CUADRANT II
        rotationAngle = 180 - rotationAngle;
        currentCuadrant = 2;
      }
      else if(_y<0){  //If angle is located on the -- CUADRANT of the plane CUADRANT III
        currentCuadrant = 3;
      }
    }
//End of localization of CUADRANT on the plane (I, II, III, IV)


//Beginning of calculations to get the angles required for the servo 2,3,4
  if(currentCuadrant == 3 || currentCuadrant == 4){
    _x = abs(_x);
    _y = abs(_y);
    //_z = abs(_z);
    hST1 = sqrt(sq(_x) + sq(_y));
    hST2 = sqrt(sq(hST1) + sq(_z));
    K = asin(_z/hST2);
    ak = sqrt(sq(gripperLength) +  sq(hST2) - 2*(gripperLength)*(hST2) * cos(K));
    BT1 = acos((sq(gripperLength) + sq(ak) - sq(hST2)) / (2 * gripperLength * ak));
    AT1 = PI - (BT1 + K);
    AT2 = acos((sq(12) + sq(12) - sq(ak)) / (2 * 12 *  12));
    BT2 = acos((sq(ak) + sq(12) - sq(12)) / (2 * ak * 12));
    CT2 = BT2;

    shoulderAngle = 180 - ((K + AT1 + CT2) * (180 / PI));
    elbowAngle = (((AT2) * (180 / PI)) - 90);
    wristPitchAngle = 180 - ((BT2 + BT1) * (180 / PI) - 75);
  }
  else if(currentCuadrant == 1 || currentCuadrant == 2){
    hST1 = sqrt(sq(_x) + sq(_y));
    hST2 = sqrt(sq(hST1) + sq(_z));
    K = asin(_z/hST2);
    ak = sqrt(sq(gripperLength) +  sq(hST2) - 2*(gripperLength)*(hST2) * cos(K));
    BT1 = acos((sq(gripperLength) + sq(ak) - sq(hST2)) / (2 * gripperLength * ak));
    AT1 = PI - (BT1 + K);
    AT2 = acos((sq(12) + sq(12) - sq(ak)) / (2 * 12 *  12));
    BT2 = acos((sq(ak) + sq(12) - sq(12)) / (2 * ak * 12));
    CT2 = BT2;

    shoulderAngle = (((K + AT1 + CT2) * (180 / PI)));
    elbowAngle = 180 - (((AT2) * (180 / PI)) - 90);
    wristPitchAngle = (BT2 + BT1) * (180 / PI) - 75;
  }

  if(elbowAngle < 0){
    elbowAngle = 0;
    wristPitchAngle = abs(shoulderAngle - 180);
  }
  else if(elbowAngle > 180){
    elbowAngle = 180;
    wristPitchAngle = abs(shoulderAngle - 180);
  }

Serial.println(gripperAngle);
//End of calculations to get the angles required for the servo 2,3,4

}

double InverseKinematics::servo_1_angle(){
  return rotationAngle;
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

