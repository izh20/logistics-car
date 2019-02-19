from machine import Pin, I2C
from pca9685 import PCA9685
from servos import PCASG90
from configs import config

# 创建一个I2C对象
i2c = I2C(scl=Pin(config['I2C_SCL']), sda=Pin(config['I2C_SDA']),
    freq=config['I2C_FREQUENCY'])
# 初始化PCA9685对象
pca9685 = PCA9685(i2c, config['PCA9685_ADDRESS'])
pca9685.freq(config['PCA9685_PWM_FREQUENCY'])

# 底部舵机
servo_btm = PCASG90(pca9685, 0, min_angle=0, max_angle=180)
#servo_btm.set_angle(40)
# 左侧舵机
servo_left = PCASG90(pca9685, 1, min_angle=30, max_angle=150)
#servo_left.set_angle()
# 右侧舵机
servo_right = PCASG90(pca9685, 2, min_angle=18, max_angle=120)
servo_right.set_angle(18)

# 爪子舵机
servo_crew = PCASG90(pca9685, 3, min_angle=100, max_angle=150)
#servo_crew.set_angle(100)
