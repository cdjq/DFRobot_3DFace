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
sys.path.append("../")
import time
import RPi.GPIO as GPIO
from python.raspberrypi.DFRobot_3DFace import *

'''
  Select to use i2c or UART
  I2C_MODE
  UART_MODE
'''
ctype = I2C_MODE

if ctype == I2C_MODE:
  I2C_1 = 0x01
  face = DFRobot_3DFace_I2C (I2C_1, DEVICE_ADDR)
elif ctype == UART_MODE:
  face = DFRobot_3DFace_UART(57600)


def setup():
  while (face.begin() == False):
    print("Sensor initialize failed!!")
    time.sleep(1)
  face.set_standby()
  face.get_face_message()
  face.delete_all_face_id()
  time.sleep(1)
  face.face_registration("raspberry test")
def loop():
  face.face_matching()
  time.sleep(0.1)

if __name__ == "__main__":
  setup()
  while True:
    loop()
