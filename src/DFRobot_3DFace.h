/*!
 * @file DFRobot_3DFace.h
 * @brief Define the basic structure of the DFRobot_3DFace class, the implementation of the basic methods
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [ZhixinLiu](zhixin.liu@dfrobot.com)
 * @version V1.0
 * @date 2023-03-07
 * @url https://github.com/DFRobot/DFRobot_3DFace
 */
#ifndef __DFROBOT_FACE_RECOGNITION_H__
#define __DFROBOT_FACE_RECOGNITION_H__

#include <Arduino.h>
#include <Wire.h>

#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
#include "SoftwareSerial.h"
#else
#include "HardwareSerial.h"
#endif



typedef struct {
  uint16_t userID;
  bool result;
  uint8_t direction;
  uint8_t errorCode;
}sFaceReg_t;


typedef struct {
  uint16_t userID;
  bool result;
  uint8_t name[32] = {0};
  bool admin;
  uint8_t state;
  uint8_t errorCode;
}sFaceMatching_t;


typedef struct {
  uint8_t user_count;
  uint8_t data[200];
  bool result;
}sUserData_t;

typedef enum{
  eDirectView = 0x01,
  eLookUpView = 0x10,
  eLookDownView = 0x08,
  eLookLeftView = 0x04,
  elookRightView = 0x02,
}eAngleView_t;

typedef enum{
  eResDirect = 0x01,
  eResLookUp = 0x11,
  eLookDown = 0x19,
  eLookLeft = 0x1d,
  elookRight = 0x1f,
}eResponseView_t;


typedef enum{
  eSuccess        = 0x00,
  eReject         = 0x01,
  eTermination    = 0x02,
  eMessErr        = 0x03,
  eCrameErr       = 0x04,
  eError          = 0x05,
  eParamErr       = 0x06,
  eMemoryErr      = 0x07,
  eUserNoExist    = 0x08,
  eUserUserLimit  = 0x09,
  eFaceExist      = 0x0A,
  eLivingErr      = 0x0C,
  eTimerout       = 0x0D,
}eResponseCode_t;

class DFRobot_3DFace{
public:
  #define REG_GERENEL_CMD 0x10
  #define DEVICE_ADDR     0x20
  #define TIME_OUT        200     ///< time out
  #define I2C_FLAG        1
  #define UART_FLAG       2
  #define I2C_ID          30
  #define MAX_SIZE        200

  #define DATA_CODE   0x6

  #define ERROR_CODE  0x6
  #define C_SUCCESS   0x0
  #define C_REJECT    0x1
  #define C_REJECT    0x1

  // 注册模式是管理员模式还是用户模式
  #define USER 0
  #define ADMIN 1

  // 单项注册或五项注册
  #define ONE_REG  1
  #define FIVE_REG 0


  #define REG_WRITE_AT_LONG       0x39
  #define REG_WRITE_AT            0x40
  #define REG_READ_AT_LEN         0x41
  #define REG_READ_AT             0x42






  DFRobot_3DFace();
  ~DFRobot_3DFace();
  uint8_t  uartI2CFlag = 0;

  bool setStandby(void);
  bool delFaceID(uint16_t number);
  bool delAllFaceID(void);
  sFaceMatching_t faceMatching(void);

  /**
   * @brief 单项注册
   * 
   * @param mode 是否是管理员模式
   * @param name 注册的名字
   * @param direction 注册的方向，单项注册一定是直视注册
   * @param regType 单项注册
   * @param repetition 是否重复注册
   * @param timerout 超时时间
   * @return sFaceReg_t 
   */
  sFaceReg_t faceRegistration(char* name, uint8_t mode=ADMIN, eAngleView_t direction=eDirectView, uint8_t regType=ONE_REG, bool repetition=true, uint8_t timerout=0x0A);

  sFaceReg_t directRegistration(char* name=NULL, uint8_t timerout=0x0A);
  sFaceReg_t lookUpRegistration(char* name=NULL, uint8_t timerout=0x0A);
  sFaceReg_t lookDownRegistration(char* name=NULL, uint8_t timerout=0x0A);
  sFaceReg_t turnLeftRegistration(char* name=NULL, uint8_t timerout=0x0A);
  sFaceReg_t turnRightRegistration(char* name=NULL, uint8_t timerout=0x0A);

  sUserData_t getFaceMassage(void);

  uint8_t getParityCheck(uint8_t * p ,int len);
  int8_t waitTrueData(uint8_t *data, uint8_t len);
private:
  uint8_t  _addr;
  uint8_t  _M_Flag = 0;

  virtual void writeReg(uint8_t reg, uint8_t *data, uint8_t len) = 0;
  virtual int16_t readReg(uint8_t reg, uint8_t *data, uint8_t len) = 0;
};

class DFRobot_3DFace_I2C:public DFRobot_3DFace{
public:
  DFRobot_3DFace_I2C(TwoWire *pWire=&Wire, uint8_t addr = DEVICE_ADDR);
  bool begin(void);
protected:
  virtual void writeReg(uint8_t reg, uint8_t *data, uint8_t len);
  virtual int16_t readReg(uint8_t reg, uint8_t *data, uint8_t len);
private:
  TwoWire *_pWire;
  uint8_t _I2C_addr;
};

class DFRobot_3DFace_UART:public DFRobot_3DFace{
public:
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
  DFRobot_3DFace_UART(SoftwareSerial *sSerial, uint16_t Baud);
#else
  DFRobot_3DFace_UART(HardwareSerial *hSerial, uint16_t Baud ,uint8_t rxpin = 0, uint8_t txpin = 0);
#endif

  bool begin(void);
protected:
  virtual void writeReg(uint8_t reg, uint8_t *data, uint8_t len);
  virtual int16_t readReg(uint8_t reg, uint8_t *data, uint8_t len);
private:

#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
  SoftwareSerial *_serial;
#else
  HardwareSerial *_serial;
#endif
  uint32_t _baud;
  uint8_t _rxpin;
  uint8_t _txpin;
};
#endif
