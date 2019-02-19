import math
from machine import Pin, I2C
from pca9685 import PCA9685
from servos import PCASG90
from configs import config
from robot_arm import *

# 创建一个I2C对象
i2c = I2C(scl=Pin(config['I2C_SCL']), sda=Pin(config['I2C_SDA']), 
    freq=config['I2C_FREQUENCY'])
# 初始化PCA9685对象
pca9685 = PCA9685(i2c, config['PCA9685_ADDRESS'])
pca9685.freq(config['PCA9685_PWM_FREQUENCY'])


# 生成Joint1
joint1 = Joint1(pca9685)
# 生成Joint2
joint2 = Joint2(pca9685)
# 生成Joint3
joint3 = Joint3(pca9685)
