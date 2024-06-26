# -*- coding: utf-8 -*
'''!
  @file face_recoognition.py
  @brief Register and recognize faces
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license    The MIT License (MIT)
  @author     [ZhixinLiu](zhixin.liu@dfrobot.com)
  @version    V1.0
  @date       2024-02-27
  @url https://github.com/DFRobot/DFRobot_3DFace
'''
from __future__ import print_function
import sys
import os
import time
import random
sys.path.append("../")
import RPi.GPIO as GPIO
from DFRobot_3DFace import *

'''
  Select to use i2c or UART
  I2C_MODE
  UART_MODE
'''
ctype = UART_MODE

if ctype == I2C_MODE:
  I2C_1 = 0x01
  face = DFRobot_3DFace_I2C (I2C_1, DEVICE_ADDR)
elif ctype == UART_MODE:
  face = DFRobot_3DFace_UART(57600)


def setup():
  while (face.begin() == False):
    print("Sensor initialize failed!!")
    time.sleep(1)

  print("Sensor initialize success!!")  
  data = face.get_face_message()
  if data.result == True:
    print("user count = " + str(data.user_count))
    
  print("face resgistering !")
  random_int = random.randint(1, 1000)
  result_string = "raspberry:" + str(random_int)
  face_reg = face.face_registration(result_string)
  if face_reg.result == True:
    print("face resgistering success!")
    print("register name = "+str(result_string))
    print("register user id = " + str(face_reg.user_id));
    print("register direction = " + str(face_reg.direction))
  else:
    print("five register faild cause = "+ str(face.anaysis_code(face_reg.error_code)))
    
def loop():
  time.sleep(1)

if __name__ == "__main__":
  setup()
