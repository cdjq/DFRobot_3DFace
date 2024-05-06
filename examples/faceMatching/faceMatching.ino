 /*!
  * @file  faceMatching.ino
  * @brief  face matching demo
  * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  * @license The MIT License (MIT)
  * @author ZhixinLiu(zhixin.liu@dfrobot.com)
  * @version V1.0
  * @date 2024-04-25
  * @url https://github.com/DFRobot/DFRobot_3DFace
  */

#include "DFRobot_3DFace.h"

#define I2C_COMMUNICATION  //use I2C for communication, but use the serial port for communication if the line of codes were masked

#ifdef  I2C_COMMUNICATION
  DFRobot_3DFace_I2C face(&Wire ,DEVICE_ADDR);
#else
/* ---------------------------------------------------------------------------------------------------------------------
 *    board   |             MCU                | Leonardo/Mega2560/M0 |    UNO    | ESP8266 | ESP32 |  microbit  |   m0  |
 *     VCC    |            3.3V/5V             |        VCC           |    VCC    |   VCC   |  VCC  |     X      |  vcc  |
 *     GND    |              GND               |        GND           |    GND    |   GND   |  GND  |     X      |  gnd  |
 *     RX     |              TX                |     Serial1 TX1      |     5     |   5/D6  |  D2   |     X      |  tx1  |
 *     TX     |              RX                |     Serial1 RX1      |     4     |   4/D7  |  D3   |     X      |  rx1  |
 * ----------------------------------------------------------------------------------------------------------------------*/
/* Baud rate cannot be changed */
  #if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
    SoftwareSerial mySerial(4, 5);
    DFRobot_3DFace_UART face(&mySerial ,57600);
  #elif defined(ESP32)
    DFRobot_3DFace_UART face(&Serial1 ,57600 ,/*rx*/D2 ,/*tx*/D3);
  #else
    DFRobot_3DFace_UART face(&Serial1 ,57600);
  #endif
#endif

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  while(!face.begin()){
    Serial.println("NO Deivces !");
    delay(1000);
  } Serial.println("Device connected!");

  sUserData_t data = face.getFaceMessage();
  if(data.result == true){
    Serial.print("user number = ");
    Serial.println(data.user_count);
  }
}

void loop()
{
  Serial.println("face matching ..............");
  sFaceMatching_t matching = face.faceMatching();
  if(matching.result){
    Serial.println("face matching success!");
    Serial.print("matching   user id = ");
    Serial.println(matching.userID);
    Serial.print("matching   name  = ");
    Serial.println((char*)matching.name);
    Serial.println();
  }else{
    Serial.print("face matching faild cause is ");
    Serial.println(face.anaysisCode((eResponseCode_t)matching.errorCode));
  }
  delay(1000);
}