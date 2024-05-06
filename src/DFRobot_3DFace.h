/*!
 * @file DFRobot_3DFace.h
 * @brief Define the basic structure of class DFRobot_3DFace
 * @details Set the command word type to Chinese. Set the wake-up time. Add command words.
 * @n Delete command words. Retrieve the recognized command word ID.

 * @copyright	Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [qsjhyy](yihuan.huang@dfrobot.com)
 * @version V1.0
 * @date 2024-01-06
 * @url https://github.com/DFRobot/DFRobot_3DFace
 */
#ifndef _DFRobot_3DFACE_H_
#define _DFRobot_3DFACE_H_

#include "Arduino.h"
#include "Wire.h"


#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
#include "SoftwareSerial.h"
#else
#include "HardwareSerial.h"
#endif

// #define ENABLE_DBG ///< Enable this macro to see the detailed running process of the program
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

/**
 * @struct sFaceReg_t
 * @brief Contains some face information
 * @attention Registered face returns results
 */
typedef struct {
  uint16_t userID;
  bool result;
  uint8_t direction;
  uint8_t errorCode;
}sFaceReg_t;

/**
 * @struct sFaceMatching_t
 * @brief Information containing face matching results
 * @attention Face matching results
 */
typedef struct { 
  uint8_t errorCode;
  uint16_t userID;
  bool result;
  uint8_t name[32] = {0};
  bool admin;
  uint8_t state;
}sFaceMatching_t;

/**
 * @struct sUserData_t
 * @brief Recorded user information
 */
typedef struct {
  uint8_t user_count;
  uint8_t data[200];
  bool result;
}sUserData_t;


/**
 * @enum eAngleView_t
 * @brief Face recognition direction
 */
typedef enum{
  eDirectView = 0x01,
  eLookUpView = 0x10,
  eLookDownView = 0x08,
  eLookLeftView = 0x04,
  elookRightView = 0x02,
}eAngleView_t;

/**
 * @enum eResponseView_t
 * @brief Register the direction of the response
 */
typedef enum{
  eResDirect = 0x01,
  eResLookUp = 0x11,
  eLookDown = 0x19,
  eLookLeft = 0x1d,
  elookRight = 0x1f,
}eResponseView_t;

/**
 * @enum eResponseCode_t
 * @brief Cause of the error code
 */
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

class DFRobot_3DFace
{
public:
  #define DEVICE_ADDR 0x20
  #define TIME_OUT    0x64     ///< time out
  #define DATA_CODE   0x06
  #define ERROR_CODE  0x06
  #define C_SUCCESS   0x00
  #define C_ERROR     0x01
  #define C_REJECT    0x01
  // The registration mode is administrator mode or user mode
  #define USER  0x00
  #define ADMIN 0x01
  // Single registration or five-item registration
  #define ONE_REG  0x01
  #define FIVE_REG 0x00
  // register address
  #define REG_WRITE_AT_LONG 0x39
  #define REG_WRITE_AT      0x40
  #define REG_READ_AT_LEN   0x41
  #define REG_READ_AT       0x42
  #define TX_MAX_LENGTH  50
  #define RX_MAX_LENGTH  150
  #define UART_BAUDRATE  57600
  DFRobot_3DFace();
  //~DFRobot_3DFace();

  /**
   * @fn anaysisCode
   * @brief anaysis error code
   * @param errorCode
   * @n       eSuccess
   * @n       eReject
   * @n       eTermination
   * @n       eMessErr
   * @n       eCrameErr
   * @n       eError
   * @n       eParamErr
   * @n       eMemoryErr
   * @n       eUserNoExist
   * @n       eUserUserLimit
   * @n       eFaceExist
   * @n       eLivingErr
   * @n       eTimerout
   * @return cause string
   */
  const char *anaysisCode(eResponseCode_t errorCode);
  
  /**
   * @fn delFaceID
   * @brief Deletes the specified face id
   * @param number 
   * @return true or false
   */
  bool delFaceID(uint16_t number);

  /**
   * @fn delAllFaceID
   * @brief Delete all facial information
   * @return true or false
   */
  bool delAllFaceID(void);

  /**
   * @fn faceMatching
   * @brief Face matching
   * @return sFaceMatching_t match result
   */
  sFaceMatching_t faceMatching(void);

  /**
   * @fn faceRegistration
   * @brief Single registered face
   * @param mode Administrator mode or not
   * @param name Registered name
   * @param direction The direction of registration, single registration must be directly registered
   * @param regType  ONE_REG
   * @param repetition Whether to repeat registration
   * @param timerout
   * @return sFaceReg_t Result of registration
   */
  sFaceReg_t faceRegistration(char* name, uint8_t mode=ADMIN, eAngleView_t direction=eDirectView, uint8_t regType=ONE_REG, bool repetition=true, uint8_t timerout=0x0A);

  /**
   * @fn directRegistration
   * @brief Look direct at face registration
   * @param name name must null
   * @param timerout
   * @return sFaceReg_t Result of registration
   */
  sFaceReg_t directRegistration(char* name=NULL, uint8_t timerout=0x0A);

  /**
   * @fn lookUpRegistration
   * @brief Look up at face registration
   * @param name name nust null
   * @param timerout
   * @return sFaceReg_t Result of registration
   */
  sFaceReg_t lookUpRegistration(char* name=NULL, uint8_t timerout=0x0A);

  /**
   * @fn lookDownRegistration
   * @brief Look down at face registration
   * @param name name must null
   * @param timerout
   * @return sFaceReg_t Result of registration
   */
  sFaceReg_t lookDownRegistration(char* name=NULL, uint8_t timerout=0x0A);

  /**
   * @fn turnLeftRegistration
   * @brief Look left at face registration
   * @param name name must null
   * @param timerout
   * @return sFaceReg_t Result of registration
   */
  sFaceReg_t turnLeftRegistration(char* name=NULL, uint8_t timerout=0x0A);

  /**
   * @fn turnLeftRegistration
   * @brief Look right at face registration
   * @param name register name
   * @param timerout
   * @return sFaceReg_t Result of registration
   */
  sFaceReg_t turnRightRegistration(char* name=NULL, uint8_t timerout=0x0A);

  /**
   * @fn getFaceMessage
   * @brief get face message
   * @return sUserData_t face id ,face result
   */
  sUserData_t getFaceMessage(void);

  virtual bool begin(void);
protected:
  bool setStandby(void);
  uint8_t getMoudleState(void);
  bool setStandbyMode(void);
  uint8_t getParityCheck(uint8_t * p ,int len);
  int8_t waitMatch(uint8_t *data, uint8_t len);
private:
  /**
   * @fn writeReg
   * @brief Write register function, designed as virtual function, implemented by derived class
   * @param reg  Register address 8bits
   * @param pBuf Storage and buffer for data to be written
   * @param size Length of data to be written
   * @return None
   */
  virtual void writeReg(uint8_t reg, void* pBuf, size_t size) = 0;

  /**
   * @fn readReg
   * @brief Read register function, designed as virtual function, implemented by derived class
   * @param reg  Register address 8bits
   * @param pBuf Storage and buffer for data to be read
   * @param size Length of data to be read
   * @return uint8_t type, indicates returning read register status
   * @retval 0 Reading succeeded
   * @retval 1 Reading failed
   */
  virtual uint8_t readReg(uint8_t reg, void* pBuf, size_t size) = 0;

};

class DFRobot_3DFace_I2C:public DFRobot_3DFace
{
public:
  DFRobot_3DFace_I2C(TwoWire* pWire = &Wire, uint8_t addr = 0x20);
  bool begin(void);
protected:
  virtual void writeReg(uint8_t reg, void* pBuf, size_t size);
  virtual uint8_t readReg(uint8_t reg, void* pBuf, size_t size);
private:
  TwoWire* _pWire;
  uint8_t _deviceAddr;
  uint8_t _I2C_addr = 0;
};

class DFRobot_3DFace_UART:public DFRobot_3DFace
{
public:
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
  DFRobot_3DFace_UART(SoftwareSerial* sSerial, uint32_t Baud = UART_BAUDRATE);
#else
  DFRobot_3DFace_UART(HardwareSerial* hSerial, uint32_t Baud = UART_BAUDRATE, uint8_t rxpin = 0, uint8_t txpin = 0);
#endif

  bool begin(void);
protected:
  virtual void writeReg(uint8_t reg, void* pBuf, size_t size);
  virtual uint8_t readReg(uint8_t reg, void* pBuf, size_t size);
private:
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
  SoftwareSerial* _serial;
#else
  HardwareSerial* _serial;
#endif
  uint32_t _baud;
  uint8_t _rxpin;
  uint8_t _txpin;
};

#endif/* _DFRobot_3DFACE_H_ */
