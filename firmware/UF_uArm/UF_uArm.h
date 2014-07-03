/******************************************************************************
* File Name          : UF_uArm.h
* Author             : Evan
* Updated            : Scott Gray
* Version            : V0.0.3 SG Version (BETA)
* Created Date       : 2 MAY, 2014
* Modified Date      : 22 MAY, 2014
* Description        :
* License            :
* Copyright(C) 2014 UFactory Team. All right reserved.
*******************************************************************************/

#include <Arduino.h>
#include <Servo.h>
#include <EEPROM.h>

#ifndef UF_uArm_h
#define UF_uArm_h

/****************  Macro definitions  ****************/
#define ARM_A                   148    // upper arm
#define ARM_B                   160    // lower arm
#define ARM_2AB                 47360  // 2*A*B
#define ARM_A2                  21904  // A^2
#define ARM_B2                  25600  // B^2
#define ARM_A2B2                47504  // A^2 + B^2
#define ARM_STRETCH_MIN         0
#define ARM_STRETCH_MAX         210
#define ARM_HEIGHT_MIN          -180
#define ARM_HEIGHT_MAX          150
#define ARM_ROTATION_MIN        -90
#define ARM_ROTATION_MAX        90
#define HAND_ROTATION_MIN       -90
#define HAND_ROTATION_MAX       90
#define HAND_ANGLE_OPEN         25
#define HAND_ANGLE_CLOSE        70
#define FIXED_OFFSET_L          18
#define FIXED_OFFSET_R          36
#define D150A_SERVO_MIN_PUL     535
#define D150A_SERVO_MAX_PUL     2415
#define D009A_SERVO_MIN_PUL     600
#define D009A_SERVO_MAX_PUL     2550
#define SAMPLING_DEADZONE       2
#define INIT_POS_L              30
#define INIT_POS_R              24
#define BTN_TIMEOUT_MS          3000
#define CATCH					0x01
#define RELEASE					0x02

/*****************  Port definitions  *****************/
#define BTN_D4                  4     //
#define BTN_D7                  7     //
#define BUZZER                  3     //
#define LIMIT_SW                2     // Limit Switch
#define PUMP_EN                 6     //
#define VALVE_EN                5     //
#define SERVO_HAND              9     //
#define SERVO_HAND_ROT          10    //
#define SERVO_ROT               11    //
#define SERVO_R                 12    //
#define SERVO_L                 13    //

class UF_uArm
{
public:
	UF_uArm();
	void init();    // initialize the uArm position
    void calibration();  //
    int readAngle(char _servoNum);
	void setPosition(double _stretch, double _height, int _armRot, int _handRot);    //
	void gripperCatch();    //
	void gripperRelease();  //
	void gripperDetach();   //
    void gripperDirectDetach(); //
    void pumpOn();          // pump enable
    void pumpOff();         // pump disnable
    void valveOn();         // valve enable, decompression
    void valveOff();        // valve disnable
    void detachServo(char _servoNum);
	void sendData(byte _dataAdd, int _dataIn); //

private:
	/*******************  Servo offset  *******************/
	char offsetL;
	char offsetR;
	char offsetSet;        //SG-> Added to know if offset is set
	/*****************  Define variables  *****************/
	int heightLst;
	int height;
	int stretch;
	int rotation;
	int handRot;
    int minAngle_L;
    int minAngle_R;
    boolean adjustFlag;
	boolean resetflag;
    unsigned long delay_loop;
    unsigned long lstTime;  //limit: 50days
	/***************  Create servo objects  ***************/
	Servo servoR;
	Servo servoL;
	Servo servoRot;
	Servo servoHand;
	Servo servoHandRot;
};

#endif

