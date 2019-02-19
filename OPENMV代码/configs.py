config = {
    # I2C的相关配置
    'I2C_SCL': 'P4', # 用户IIC SCL
    'I2C_SDA': 'P5', # 用户IIC SDA
    'I2C_FREQUENCY': 10000,
    # PCA9685相关的配置
    'PCA9685_ADDRESS': 0x40, # PCA9685的I2C地址(默认)
    'PCA9685_PWM_FULL_DUTY': 4096,#PWM信号的分辨率
    'PCA9685_PWM_FREQUENCY': 50, # PWM的频率
    
    # 舵机通用配置SG90 单位us
    'SERVO_CONTROL_PERIOD': 20000, # PWM控制周期 us
    'SERVO_PWM_MIN_PW': 500, # 最小脉冲 us
    'SERVO_PWM_MAX_PW': 2500, # 最大脉冲 us
    'SERVO_ANGLE_RANGE': 180, # 舵机的角度范围
    'SERVO_ROTATE_SPEED': 0.12, # 0.12秒/60度(4.8V)；0.1秒/60度(6.0V)（无负载条件下）
    
}