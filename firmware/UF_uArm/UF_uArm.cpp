/******************************************************************************
* File Name          : UF_uArm.cpp
* Author             : Evan
* Updated            : Scott Gray
* Version            : V0.0.2 SG-> Scott Gray Modified to fix calibration
* Created Date       : 2 May 2014
* Modified Date      : 22 May 2014
* Description        :
* License            :
* Copyright(C) 2014 UFactory Team. All right reserved.
*******************************************************************************/

#include "UF_uArm.h"

UF_uArm::UF_uArm()
{

	delay_loop = 0;
	heightLst  = 0;
	height     = 0;
	stretch    = 0;
	rotation   = 0;
	handRot    = 0;
    lstTime    = 0;
    minAngle_L = 1023;
    minAngle_R = 1023;
    resetflag  = true;
    adjustFlag = false;
}

void UF_uArm::init()
{
    // read offset data
    offsetL = EEPROM.read(1) - 128; // set 128 to the zero
    offsetR = EEPROM.read(2) - 128; // set 128 to the zero
    offsetSet = EEPROM.read(3);     //SG-> Added read of offset flag
    // initialization the pin
    pinMode(LIMIT_SW, INPUT);
    pinMode(BTN_D4,   INPUT);
    pinMode(BTN_D7,   INPUT);
    pinMode(BUZZER,   OUTPUT); digitalWrite(BUZZER,   LOW);
    pinMode(PUMP_EN,  OUTPUT); digitalWrite(PUMP_EN,  LOW);
    pinMode(VALVE_EN, OUTPUT); digitalWrite(VALVE_EN, LOW);
    // attaches the servo on pin to the servo object
    if (offsetSet == 15)
    {
      servoL.attach(SERVO_L, D150A_SERVO_MIN_PUL, D150A_SERVO_MAX_PUL);
      servoR.attach(SERVO_R, D150A_SERVO_MIN_PUL, D150A_SERVO_MAX_PUL);
    }
    else
    {  // buzzer alert if calibration needed
      digitalWrite(BUZZER, HIGH);
      delay(30);
      digitalWrite(BUZZER, LOW);
      delay(70);
      digitalWrite(BUZZER, HIGH);
      delay(30);
      digitalWrite(BUZZER, LOW);
      delay(70);
      digitalWrite(BUZZER, HIGH);
      delay(30);
      digitalWrite(BUZZER, LOW);
    }
    servoRot.attach(SERVO_ROT, D150A_SERVO_MIN_PUL, D150A_SERVO_MAX_PUL);
    servoHand.attach(SERVO_HAND, D009A_SERVO_MIN_PUL, D009A_SERVO_MAX_PUL);
    servoHandRot.attach(SERVO_HAND_ROT, D009A_SERVO_MIN_PUL, D009A_SERVO_MAX_PUL);
    servoHand.write(HAND_ANGLE_OPEN);delay(50);
    servoHand.detach();
    // initialization postion
    setPosition(stretch, height, rotation, handRot);
}

void UF_uArm::calibration()
{
//    int initPosL = INIT_POS_L;     //SG-> Commented out this line
//    int initPosR = INIT_POS_R;     //SG-> Commented out this line
    int initPosL = INIT_POS_L + 20;  //SG-> Added 20 degrees here to start at reasonable point
    int initPosR = INIT_POS_R  + 20; //SG-> Added 20 degrees here to start at reasonable point
    if(adjustFlag)
    {
        //SG-> Commentary: While user adjusts for minimum angles, keep track of angle and add
        //                 margin of analog reading value of 12, which is about 3 degrees.
        minAngle_L = readAngle(SERVO_L) + 12;
        minAngle_R = readAngle(SERVO_R) + 12;
    }
    if(!digitalRead(BTN_D7))
    {
        // buzzer alert
        digitalWrite(BUZZER, HIGH);
        delay(20);
        digitalWrite(BUZZER, LOW);
        if(adjustFlag)
        {
            delay(200);                   //SG-> Added to delay for user to remove hand
            servoL.attach(SERVO_L, D150A_SERVO_MIN_PUL, D150A_SERVO_MAX_PUL);
            servoR.attach(SERVO_R, D150A_SERVO_MIN_PUL, D150A_SERVO_MAX_PUL);
// SG-> The next two lines are incorrect as it would just get ovberwritten in next lines
//      with initPosL and initPosR.  So start with the modified initial conditions to start with.
//            servoL.write(INIT_POS_L + 10);
//            servoR.write(INIT_POS_R + 10);
            servoL.write(initPosL);   //SG-> Changed to use modified initial position
            servoR.write(initPosR);   //SG-> Changed to use modified initial position
            delay(500);

            //SG-> Commentary: Get the angle above minimum as quickly as possible!
            //                 This should not really happen.
            while(readAngle(SERVO_L) < minAngle_L - SAMPLING_DEADZONE)
            {
                servoL.write(++initPosL);
                delay(15);
            }

            //SG-> Moved the while loop for the right servo to get above minimum before
            //     doing the work of finding the adjustment values
            while(readAngle(SERVO_R) < minAngle_R - SAMPLING_DEADZONE)
            {
                servoR.write(++initPosR);
                delay(15);
            }

            //SG-> Commentary: Now creep down to the user-defined minimum angles
            while(readAngle(SERVO_L) > minAngle_L + SAMPLING_DEADZONE)
            {
                servoL.write(--initPosL);
//                delay(15);          //SG-> Commented out this line
                delay(50);           //SG-> Changed to use longer delay
            }

            while(readAngle(SERVO_R) > minAngle_R + SAMPLING_DEADZONE)
            {
                servoR.write(--initPosR);
//                delay(15);          //SG-> Commented out this line
                delay(50);           //SG-> Changed to use longer delay
            }

//            offsetL = initPosL - INIT_POS_L + 128;  //SG-> Commented out this line
//            offsetR = initPosR - INIT_POS_R + 128;  //SG-> Commented out this line
            offsetL = initPosL - INIT_POS_L + 128 - 7;  //SG-> Subtracted 7 to get offsets to work
            offsetR = initPosR - INIT_POS_R + 128 - 7;  //SG-> Subtracted 7 to get offsets to work
            EEPROM.write(1, offsetL);  //offsetL
            EEPROM.write(2, offsetR);  //offsetR
            EEPROM.write(3, 15);  //SG-> Added flag to know if offset is done
            adjustFlag = false;

            // buzzer alert
            digitalWrite(BUZZER, HIGH);
            delay(500);
            digitalWrite(BUZZER, LOW);

            // reset postion
            init();
        }

        lstTime = millis();
        //
        while(!digitalRead(BTN_D7))
        {
            if(millis() - lstTime > BTN_TIMEOUT_MS)
            {
                // buzzer alert
                digitalWrite(BUZZER, HIGH); delay(50);
                digitalWrite(BUZZER, LOW);  delay(100);
                digitalWrite(BUZZER, HIGH); delay(50);
                digitalWrite(BUZZER, LOW);
                //
                while(!digitalRead(BTN_D7))
                {
                    servoL.detach();
                    servoR.detach();
                    adjustFlag = true;
                }
            }
        }
    }
}

void UF_uArm::setPosition(double _stretch, double _height, int _armRot, int _handRot)
{
  _armRot = -_armRot;
  if(!digitalRead(LIMIT_SW) && _height < heightLst) //limit switch protection
    _height = heightLst;
  // input limit
  _stretch = _stretch < ARM_STRETCH_MIN  ? ARM_STRETCH_MIN  : _stretch;
  _stretch = _stretch > ARM_STRETCH_MAX  ? ARM_STRETCH_MAX  : _stretch;
  _height  = _height  < ARM_HEIGHT_MIN   ? ARM_HEIGHT_MIN   : _height;
  _height  = _height  > ARM_HEIGHT_MAX   ? ARM_HEIGHT_MAX   : _height;
  _armRot  = _armRot  < ARM_ROTATION_MIN ? ARM_ROTATION_MIN : _armRot;
  _armRot  = _armRot  > ARM_ROTATION_MAX ? ARM_ROTATION_MAX : _armRot;
  _handRot = _handRot < HAND_ROTATION_MIN? HAND_ROTATION_MIN: _handRot;
  _handRot = _handRot > HAND_ROTATION_MAX? HAND_ROTATION_MAX: _handRot;
  _armRot  += 90; // change -90~90 to 0~180
  _handRot += 90; // change -90~90 to 0~180
  _stretch += 55; // set stretch zero

  // angle calculation
  double stretch2height2 = _stretch * _stretch + _height * _height;              //
  double angleA = (acos( (ARM_A2B2 - stretch2height2) / ARM_2AB )) * RAD_TO_DEG; // angle between the upper and the lower
  double angleB = (atan(_height/_stretch)) * RAD_TO_DEG;                         //
  double angleC = (acos((ARM_A2 + stretch2height2 -ARM_B2)/(2 * ARM_A * sqrt(stretch2height2)))) * RAD_TO_DEG; //
  int angleR = 180 - angleA - angleB - angleC + FIXED_OFFSET_R + offsetR;        //
  int angleL = angleB + angleC + FIXED_OFFSET_L + offsetL;                       //
  // angle limit
  angleL = angleL>(135+offsetL)? 135+offsetL:angleL; //
  if(angleL<(10+offsetL)){angleR = angleR<(80 +offsetR)?( 80+offsetR):angleR;}
  angleR = angleR<(angleL-95 + 30 +offsetR)? (angleL-95 +30+offsetR):angleR;
  angleR = angleR<(40 +offsetR)?( 40+offsetR):angleR;
  angleR = angleR>(152 +offsetR)?( 152+offsetR):angleR;
  // set servo position
  servoR.write(angleR);
  servoL.write(angleL);
  servoRot.write(_armRot);
  servoHandRot.write(_handRot);
  heightLst = _height;
  /*
  Serial.print("x: ");Serial.print( x-55 );Serial.print("  ");
  Serial.print("y: ");Serial.print( y );Serial.print("  ");
  Serial.print("angleR: ");Serial.print( angleR );Serial.print("  ");
  Serial.print("angleL: ");Serial.print( angleL );Serial.print("  ");
  Serial.print("servoRot: ");Serial.println( angleRot );
  */
}

int UF_uArm::readAngle(char _servoNum)
{
    int portAd;
    switch(_servoNum)
    {
        case SERVO_L:
            portAd = A0;
            break;
        case SERVO_R:
            portAd = A1;
            break;
        case SERVO_ROT:
            portAd = A2;
            break;
        case SERVO_HAND_ROT:
            portAd = A3;
            break;
        case SERVO_HAND:
            portAd = A4;
            break;
        default: return 0; break;
    }
    int adAdd = 0;
    for(char i=0; i<10; i++)
    {
        adAdd += analogRead(portAd);
    }
    return adAdd/10;
}

void UF_uArm::gripperCatch()
{
	servoHand.attach(SERVO_HAND, D009A_SERVO_MIN_PUL, D009A_SERVO_MAX_PUL);
    servoHand.write(HAND_ANGLE_CLOSE);
    digitalWrite(VALVE_EN, LOW); // valve disnable
    digitalWrite(PUMP_EN, HIGH); // pump enable
    resetflag = true;
}

void UF_uArm::gripperRelease()
{
	if(resetflag)
    {
      servoHand.attach(SERVO_HAND, D009A_SERVO_MIN_PUL, D009A_SERVO_MAX_PUL);
      servoHand.write(HAND_ANGLE_OPEN);
      digitalWrite(VALVE_EN, HIGH); // valve enable, decompression
      digitalWrite(PUMP_EN, LOW);   // pump disnable
      resetflag = false;
      delay_loop = 0;
    }
}

void UF_uArm::gripperDetach()
{
    if(++delay_loop > 100000)        // delay release valve
    {
        servoHand.detach();
        digitalWrite(VALVE_EN, LOW); // valve disnable
        delay_loop=0;
    }
}

void UF_uArm::gripperDirectDetach()
{
    servoHand.detach();
    digitalWrite(VALVE_EN, LOW); // valve disnable
}

void UF_uArm::pumpOn()
{
    digitalWrite(PUMP_EN, HIGH);    // pump enable
}

void UF_uArm::pumpOff()
{
    digitalWrite(PUMP_EN, LOW);     // pump disnable
}

void UF_uArm::valveOn()
{
    digitalWrite(VALVE_EN, HIGH);   // valve enable, decompression
}

void UF_uArm::valveOff()
{
    digitalWrite(VALVE_EN, LOW);    // valve disnable
}

void UF_uArm::detachServo(char _servoNum)
{
    switch(_servoNum)
    {
        case SERVO_L:
        servoL.detach();
        break;
        case SERVO_R:
        servoR.detach();
        break;
        case SERVO_ROT:
        servoRot.detach();
        break;
        case SERVO_HAND_ROT:
        servoHandRot.detach();
        break;
        case SERVO_HAND:
        servoHand.detach();
        break;
        default: break;
    }


}

void UF_uArm::sendData(byte _dataAdd, int _dataIn)
{
  Serial.write(0xFF); Serial.write(0xAA); // send data head
  Serial.write(_dataAdd);
  Serial.write(*((char *)(&_dataIn) + 1));
  Serial.write(*((char *)(&_dataIn)));
}
