/*!
 * @file DFRobot_3DFace.cpp
 * @brief Implemention of DFRobot_3DFace class
 * @copyright	Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [qsjhyy](yihuan.huang@dfrobot.com)
 * @version V1.0
 * @date 2024-01-06
 * @url https://github.com/DFRobot/DFRobot_3DFace
 */
#include "DFRobot_3DFace.h"

DFRobot_3DFace::DFRobot_3DFace()
{
}

bool DFRobot_3DFace::begin()
{
  uint8_t len = 0;
  uint8_t rx_temp[RX_MAX_LENGTH] = {0x00};
  len = readReg(REG_READ_AT_LEN, rx_temp, 0);
  while(len != 0){
    len = readReg(REG_READ_AT_LEN, rx_temp, 0);
    Serial.println(len,HEX);
  }
  return true;
}
bool DFRobot_3DFace::setStandby(void)
{  
  if(0 != getMoudleState()){
    if(true == setStandbyMode()){
      return true;
    }else{
      return false;
    }
  }
  return true;
}

bool DFRobot_3DFace::setStandbyMode(void)
{
  uint16_t len = 0;
  uint8_t timerout = 0;
  int8_t count = -1;
  uint8_t tx_temp[TX_MAX_LENGTH] = {0xEF, 0xAA, 0x23, 0x00, 0x00, 0x23};
  uint8_t rx_temp[RX_MAX_LENGTH] = {0x00};
  writeReg(REG_WRITE_AT, tx_temp, 6);
  while(1){
    if(timerout++ > 100){
      return false;
    }
    delay(100);
    len = readReg(REG_READ_AT_LEN, rx_temp, 0);
    if(len != 0){
      count = waitMatch(rx_temp, len);
      if(count != -1){
        if(rx_temp[count+5] == 0x23 && rx_temp[6] == 0x00){
          return true;
        }
      }
    }
  }
  return true;
}


uint8_t DFRobot_3DFace::getMoudleState(void)
{
  uint16_t len = 0;
  int8_t count = -1;
  uint8_t timerout = 0;
  uint8_t tx_temp[TX_MAX_LENGTH] = {0xEF, 0xAA, 0x11, 0x00, 0x00, 0x11};
  uint8_t rx_temp[RX_MAX_LENGTH] = {0x00};
  writeReg(REG_WRITE_AT, tx_temp, 6);
  while(1){
    if(timerout++ > 30){
      return C_ERROR;
    }
    len = readReg(REG_READ_AT_LEN, rx_temp, 0);
    delay(100);
    if(len != 0){
      count = waitMatch(rx_temp, len);
      if(count != -1){
        delay(100);
        return rx_temp[count+7];
      }
    }
  }
}

bool DFRobot_3DFace::delFaceID(uint16_t number)
{
  uint16_t len = 0;
  uint8_t tx_temp[TX_MAX_LENGTH] = {0xEF, 0xAA, 0x20, 0x00, 0x02};
  uint8_t rx_temp[RX_MAX_LENGTH] = {0x00};
  tx_temp[5] = (uint8_t)(number>>8);
  tx_temp[6] = (uint8_t)(number);
  tx_temp[7] = getParityCheck(tx_temp, 7);
  setStandby();
  delay(100);
  writeReg(REG_WRITE_AT, tx_temp, 8);
  while(1){
    delay(100);
    len = readReg(REG_READ_AT_LEN, rx_temp, 0);
    if(len != 0){
      if(rx_temp[ERROR_CODE] == C_SUCCESS){
        delay(100);
        return true;
      }else{
        delay(100);
        return false;
      }
    }
  }
  return true;
}

bool DFRobot_3DFace::delAllFaceID(void)
{
  uint16_t len = 0;
  uint8_t tx_temp[TX_MAX_LENGTH] = {0xEF, 0xAA, 0x21, 0x00, 0x00, 0x21};
  uint8_t rx_temp[RX_MAX_LENGTH] = {0x00};
  setStandby();
  delay(100);
  writeReg(REG_WRITE_AT, tx_temp, 6);
  while(1){
    delay(100);
    len = readReg(REG_READ_AT_LEN, rx_temp, 0);
    if(len != 0){
      if(rx_temp[ERROR_CODE] == C_SUCCESS){
        return true;
      }else{
        return false;
      }
    }
  }
  return true;
}

sFaceReg_t DFRobot_3DFace::directRegistration(char* name, uint8_t timerout)
{
  return faceRegistration(name, ADMIN, eDirectView, FIVE_REG, true, timerout);
}

sFaceReg_t DFRobot_3DFace::lookUpRegistration(char* name, uint8_t timerout)
{
  return faceRegistration(name, ADMIN, eLookUpView, FIVE_REG, true, timerout);
}

sFaceReg_t DFRobot_3DFace::lookDownRegistration(char* name, uint8_t timerout)
{
  return faceRegistration(name, ADMIN, eLookDownView, FIVE_REG, true, timerout);
}

sFaceReg_t DFRobot_3DFace::turnLeftRegistration(char* name, uint8_t timerout)
{
  return faceRegistration(name, ADMIN, eLookLeftView, FIVE_REG, true, timerout);
}

sFaceReg_t DFRobot_3DFace::turnRightRegistration(char* name, uint8_t timerout)
{
  return faceRegistration(name, ADMIN, elookRightView, FIVE_REG, true, timerout);
}

sFaceReg_t DFRobot_3DFace::faceRegistration(char* name, uint8_t mode, eAngleView_t direction, uint8_t regType, bool repetition, uint8_t timerout)
{
  uint16_t len = 0;
  int8_t count = -1;
  uint8_t tx_temp[TX_MAX_LENGTH] = {0xEF, 0xAA, 0x26, 0x00, 0x28};
  uint8_t rx_temp[RX_MAX_LENGTH] = {0x00};
  sFaceReg_t data;
  tx_temp[5] = mode;
  if(name != NULL){
    memcpy(tx_temp+6,(uint8_t *)name ,strlen((const char *)name));
  }
  tx_temp[38] = direction;
  tx_temp[39] = regType;
  tx_temp[40] = repetition;
  tx_temp[41] = timerout;
  tx_temp[45] = getParityCheck(tx_temp, 45);

  if(regType == ONE_REG || direction == eDirectView){
    delay(100);
    setStandby();
    delay(100);
  }
  writeReg(REG_WRITE_AT, tx_temp, 46);
  while(1){
    delay(500);
    len = readReg(REG_READ_AT_LEN, rx_temp, 0);
    if(len != 0){
      count = waitMatch(rx_temp, len);
      if(count != -1){
        delay(100);
        data.errorCode = rx_temp[count + 6];
        if(rx_temp[count + 6] == C_SUCCESS){
          data.result = true;
          data.userID = (uint16_t)((rx_temp[count+7]) << 8) | rx_temp[count+8];
          data.direction = rx_temp[count + 9];
        }else{
          data.result = false;
        }
        if(regType == ONE_REG || direction == elookRightView){
          delay(100);
          setStandby();
        }
        return data;
      }
    }
  }
  return data;
}

sUserData_t DFRobot_3DFace::getFaceMessage(void)
{
  sUserData_t data;
  uint16_t len = 0;
  uint8_t tx_temp[TX_MAX_LENGTH] = {0xEF, 0xAA, 0x24, 0x00, 0x00, 0x24};
  uint8_t rx_temp[210] = {0x00};
  setStandby();
  delay(100);
  writeReg(REG_WRITE_AT, tx_temp, 6);
  delay(100);
  len = readReg(REG_READ_AT_LEN, rx_temp, 210);
  if(len != 0){
    if(rx_temp[ERROR_CODE] != C_SUCCESS){
      data.result = false;
    }
    data.user_count = rx_temp[7];
    data.result = true;
    memcpy(data.data, (uint8_t *)(rx_temp+8), 200);
  }else{
    data.result = false;
  }
  return data;
}
sFaceMatching_t DFRobot_3DFace::faceMatching(void)
{
  sFaceMatching_t data;
  uint8_t number = 0;
  uint16_t len = 0;
  int8_t count = -1;
  uint8_t tx_temp[TX_MAX_LENGTH] = {0xEF, 0xAA, 0x12, 0x00, 0x02};
  uint8_t rx_temp[RX_MAX_LENGTH] = {0x00};
  tx_temp[5] = 0x00;
  tx_temp[6] = 0x0A;
  tx_temp[7] = getParityCheck(tx_temp, 7);
  setStandby();
  delay(100);
  writeReg(REG_WRITE_AT, tx_temp, 8);
  while(1){
    if(number++ > 100){
      setStandby();
      data.result = false;
      return data;
    }
    len = readReg(REG_READ_AT_LEN, rx_temp, 0);
    delay(100);
    if(len != 0){
      count = waitMatch(rx_temp, len);
      if(count != -1){
        data.errorCode = rx_temp[count + 6];
        if(rx_temp[count + 6] == C_SUCCESS){
          data.result = true;
          data.userID = (uint16_t)((rx_temp[count+7]) << 8) | rx_temp[count+8];
          memcpy(data.name, &rx_temp[count+9],32);
          data.admin = rx_temp[count+41];
          data.state = rx_temp[count+42];
        }else{
          data.result = false;
        }
        delay(100);
        setStandby();
        return data;
      }
    }
  }
}
const char * DFRobot_3DFace::anaysisCode(eResponseCode_t errorCode)
{
  switch (errorCode){
    case eReject:
      return "rejected the command";
    case eTermination:
      return "input/matching algorithm has been terminated";
    case eMessErr:
      return "Sending message error";
    case eCrameErr:
      return "Camera failed to open";
    case eError:
      return "unknown error";
    case eParamErr:
      return "Invalid parameter";
    case eMemoryErr:
      return "memory less";
    case eUserNoExist:
      return "no recorded users";
    case eUserUserLimit:
      return "record exceeds the maximum user";
    case eFaceExist:
      return "Face has been recorded";
    case eLivingErr:
      return "Biopsy failure";
    case eTimerout:
      return "timer out";
    default:
      return "timer out";
  }
}

int8_t DFRobot_3DFace::waitMatch(uint8_t *data, uint8_t len)
{
  for(uint8_t i = 0; i < len; i++){
    if(data[i] == 0xEF && data[i+1] == 0xAA && data[i+2] == 0x00){
      return i;
    }
  }
  return -1;
}

uint8_t DFRobot_3DFace::getParityCheck(uint8_t * p ,int len)
{
  uint8_t parityCheck = 0;
  for(int i = 2; i < len; i++){
    parityCheck ^= p[i];
  }
  return parityCheck;
}

DFRobot_3DFace_I2C::DFRobot_3DFace_I2C(TwoWire* pWire, uint8_t addr)
{
  _pWire = pWire;
  _deviceAddr = addr;
}

bool DFRobot_3DFace_I2C::begin()
{
  _pWire->begin();
  _pWire->beginTransmission(_deviceAddr);
  if (_pWire->endTransmission() != 0) {
    DBG("I2C init error!!!!");
    return false;
  }
  return DFRobot_3DFace::begin();
}

void DFRobot_3DFace_I2C::writeReg(uint8_t reg, void* pBuf, size_t size)
{
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t* _pBuf = (uint8_t*)pBuf;
  uint16_t len = (uint16_t)size;
  if(len > 31){
    _pWire->beginTransmission(_deviceAddr);
    _pWire->write(REG_WRITE_AT_LONG);
    _pWire->write(_pBuf, 31);
    _pWire->endTransmission(false);
    len -= 31;
    _pWire->beginTransmission(_deviceAddr);
    _pWire->write(reg);
    _pWire->write(_pBuf+31, len);
    _pWire->endTransmission();
  }else{
    _pWire->beginTransmission(_deviceAddr);
    _pWire->write(reg);
    _pWire->write(_pBuf, len);
    _pWire->endTransmission();
  }
}

uint8_t DFRobot_3DFace_I2C::readReg(uint8_t reg, void* pBuf, size_t size)
{
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t* _pBuf = (uint8_t*)pBuf;
  uint8_t i = 0, rxLen = 0;
  uint8_t max_len = 0;
  if(size == 0){
    max_len = RX_MAX_LENGTH;
  }else{
    max_len = size;
  }
  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(REG_READ_AT_LEN);
  if(_pWire->endTransmission() != 0){
    DBG("iic connect failed");
    return 0;
  }
  _pWire->requestFrom((uint8_t)_deviceAddr,(uint8_t)5);
  while (_pWire->available()){
    _pBuf[i++]=_pWire->read();
  }
  rxLen = _pBuf[0];
  i = 0;
  if(rxLen == 0){
    return 0;
  }else{
    while(rxLen){
      if(rxLen > 32){
        _pWire->beginTransmission(_deviceAddr);
        _pWire->write(REG_READ_AT);
        if(_pWire->endTransmission() != 0){
          DBG("iic connect failed");
          return 0;
        }
        _pWire->requestFrom((uint8_t)_deviceAddr,(uint8_t)32);
        while(_pWire->available()){
          _pBuf[i++]=_pWire->read();
          if(i == max_len){
            i = 0;
          }
        }
        rxLen -= 32;
      }else{
        _pWire->beginTransmission(_deviceAddr);
        _pWire->write(REG_READ_AT);
        if(_pWire->endTransmission() != 0){
          DBG("iic connect failed");
          return 0;
        }
        _pWire->requestFrom((uint8_t)_deviceAddr,(uint8_t)rxLen);
        while(_pWire->available()){
          _pBuf[i++]=_pWire->read();
          if(i == max_len){
            i = 0;
          }
        }
        rxLen = 0;
      }
    }
  }
  reg = size;
  size = reg;
  return i;
}

#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
DFRobot_3DFace_UART::DFRobot_3DFace_UART(SoftwareSerial* sSerial, uint32_t Baud)
{
  _serial = sSerial;
  _baud = Baud;
  // _serial->begin(_baud);
}
#else
DFRobot_3DFace_UART::DFRobot_3DFace_UART(HardwareSerial* hSerial, uint32_t Baud, uint8_t txpin, uint8_t rxpin)
{
  _serial = hSerial;
  _baud = Baud;
  _txpin = txpin;
  _rxpin = rxpin;
}
#endif

bool DFRobot_3DFace_UART::begin(void)
{
  uint16_t len = 0;
  uint8_t tx_temp[TX_MAX_LENGTH] = {0x55};
  uint8_t rx_temp[RX_MAX_LENGTH] = {0x00};
#ifdef ESP32
  _serial->begin(_baud, SERIAL_8N1, _txpin, _rxpin);
  // #elif defined(ARDUINO_AVR_UNO) || defined(ESP8266)
    // nothing use software
#else
  _serial->begin(_baud);  // M0 cannot create a begin in a construct
#endif

  writeReg(REG_WRITE_AT, tx_temp, 1);
  while(1){
    len = readReg(REG_READ_AT_LEN, rx_temp, 0);
    delay(100);
    if(len != 0){
      if(rx_temp[0] == 0x55){
        return true;
      }else{
        return false;
      }
    }
  }
  return true;
}

void DFRobot_3DFace_UART::writeReg(uint8_t reg, void* pBuf, size_t size)
{
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t* _pBuf = (uint8_t*)pBuf;
  for(uint8_t i = 0; i < size; i++){
    _serial->write(_pBuf[i]);  
  }
  size = reg;
}

uint8_t DFRobot_3DFace_UART::readReg(uint8_t reg, void* pBuf, size_t size)
{
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t* _pBuf = (uint8_t*)pBuf;
  uint8_t i = 0;
  uint8_t max_len = 0;
  if(size == 0){
    max_len = RX_MAX_LENGTH;
  }else{
    max_len = size;
  }
  uint32_t nowtime = millis();
  while(millis() - nowtime < TIME_OUT){
    while(_serial->available() > 0){
      _pBuf[i++] = _serial->read();
      if(i == max_len){
        i = 0;
      }
    }
  }
  size = reg;
  reg = size;
  return i;
}
