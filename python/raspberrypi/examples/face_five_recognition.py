# -*- coding: utf-8 -*
'''!
  @file face_five_recognition.py
  @brief Five things to register and recognize faces
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license    The MIT License (MIT)
  @author     [ZhixinLiu](zhixin.liu@dfrobot.com)
  @version    V1.0
  @date       2024-2-28
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
  face.direct_registration()
  face.look_up_registration()
  face.look_down_registration()
  face.turn_left_registration()
  face.turn_right_registration("21111")

def loop():
  face.face_matching()
  time.sleep(0.1)

if __name__ == "__main__":
  setup()
  while True:
    loop()
