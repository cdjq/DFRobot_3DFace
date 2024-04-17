# -*- coding: utf-8 -*
'''!
  @file DFRobot_3DFace.py
  @brief DFRobot_3DFace Class infrastructure, implementation of underlying methods
  @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license The MIT License (MIT)
  @author [ZhixinLiu](zhixin.liu@dfrobot.com)
  @version V1.0
  @date 2024-2-18
  @url https://github.com/DFRobot/DFRobot_3DFace
'''
import serial
import time
import smbus

I2C_MODE  = 0x01
UART_MODE = 0x02


DEVICE_ADDR     = 0X20

REG_GERENEL_CMD = 0X10



class DFRobot_3DFace(object):
  __uart_i2c     =  0
  __speed_null_count = 0
  __all_data = struct_all_data()
  def __init__(self, bus, Baud):
    if bus != 0:
      self.i2cbus = smbus.SMBus(bus)
      self.__uart_i2c = I2C_MODE
    else:
      self.ser = serial.Serial("/dev/ttyAMA0", baudrate=Baud,stopbits=1, timeout=0.5)
      self.__uart_i2c = UART_MODE
      if self.ser.isOpen == False:
        self.ser.open()

  def begin(self):
    '''!
      @brief begin 
    '''
    return True


  def set_standby(self):
    '''!
      @brief set_standby
      @return True or False
    '''
    return True
  
  def delete_face_id(self, number):
    '''!
      @brief delete_face_id
      @return True or False
    '''
    return True

  def delete_all_face_id(self):
    '''!
      @brief delete_all_faceid
      @return True or False
    '''
    return True

  def direct_registration(self, name, timerout):
    '''!
      @brief direct_registration
      @param name
      @param timerout
      @return 0
    '''
    return 0


  def look_up_registration(self, name, timerout):
    '''!
      @brief look_up_registration
      @param name
      @param timerout
      @return 0
    '''
    return 0
  
  def look_down_registration(self, name, timerout):
    '''!
      @brief look_down_registration
      @param name
      @param timerout
      @return 0
    '''
    return 0

  def turn_left_registration(self, name, timerout):
    '''!
      @brief turn_left_registration
      @param name
      @param timerout
      @return 0
    '''
    return 0
  
  def turn_right_registration(self, name, timerout):
    '''!
      @brief turn_left_registration
      @param name
      @param timerout
      @return 0
    '''
    return 0


  def face_registration(self, name, mode, direction, reg_type ,repetition ,timerout):
    '''!
      @brief face_registration
      @return 0
    '''
    return 0


  def face_matching(self):
    '''!
      @brief face_matching
      @return match
    '''
    return 0

  def wait_true_data(self, data, len):
    '''!
      @brief wait_true_data
      @return 0
    '''
    return 0
    
  def get_face_message(self):
    '''!
      @brief get_face_message
      @return 0
    '''
    return 0

  def get_parity_check(self, p, len):
    '''!
      @brief get_parity_check
      @param p data point
      @param len
      @return check_result
    '''
    return True


class DFRobot_3DFace_I2C(DFRobot_3DFace): 
  def __init__(self, bus, addr):
    self.__addr = addr
    super(DFRobot_3DFace_I2C, self).__init__(bus,0)

  def write_reg(self, reg, data):
    while 1:
      try:
        self.i2cbus.write_i2c_block_data(self.__addr, reg, data)
        return
      except:
        print("please check connect!")
        time.sleep(1)

  def read_reg(self, reg, len):
    try:
      rslt = self.i2cbus.read_i2c_block_data(self.__addr, reg, len)
    except:
      rslt = -1
    return rslt

class DFRobot_3DFace_UART(DFRobot_3DFace):

  def __init__(self, Baud):
    self.__uart_i2c = UART_MODE
    self.__Baud = Baud 
    super(DFRobot_3DFace_UART, self).__init__(0, Baud)

  def write_reg(self, reg, data):
    test = bytes(data, encoding='ascii')
    self.ser.flushInput()
    self.ser.write(test)
    return

  def read_reg(self, reg, len):
    recv = [0]*len
    timenow = time.time()
    
    while(time.time() - timenow) <= 1:
      count = self.ser.inWaiting()
      if count != 0:
        recv = self.ser.read(len)
        self.ser.flushInput()
        return recv
    return recv
