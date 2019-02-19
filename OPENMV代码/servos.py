from configs import config

class PCASG90:
    PCA9685_PWM_FULL_DUTY = config['PCA9685_PWM_FULL_DUTY']
    SERVO_CONTROL_PERIOD = config['SERVO_CONTROL_PERIOD']
    SERVO_PWM_MIN_PW = config['SERVO_PWM_MIN_PW']
    SERVO_PWM_MAX_PW = config['SERVO_PWM_MAX_PW']

    def __init__(self, pca9685, servo_idx, kp=11.11, b=500, min_angle=0, max_angle=180):
        '''
        舵机的构造器
        '''
        self.pca9685 = pca9685 # PCA9685对象
        self.servo_idx = servo_idx # 舵机控制板上的舵机需要
        self._angle = None # 记录当前舵机的角度
                           # 注：下划线 _ 代表是私有变量
        # pw = kp*angle + b
        self.kp = kp
        self.b = b
        # 角度限制
        self.min_angle = min_angle # 舵机最小角度
        self.max_angle = max_angle # 舵机最大角度

    def calibration(self, kp, b):
        '''
        更新舵机的标定数据
        '''
        self.kp = kp
        self.b = b
    
    def set_angle(self, angle):
        '''
        设定角度
        '''
        # 判断角度angle范围是否合法
        if angle > self.max_angle or angle < self.min_angle:
            return False
        # 将角度转换为脉宽
        pw = self.kp * angle + self.b
        # 设置脉宽
        self.pulse_width(pw)
        # 更新角度数据
        self._angle = angle
        return True

    def get_angle(self):
        '''
        获取当前的舵机角度
        '''
        return self._angle
    
    def pulse_width(self, pw):
        '''
        设置舵机控制信号的脉宽
        '''
        # 判断脉宽的范围是否合理 500 - 2500之间
        if pw < self.SERVO_PWM_MIN_PW:
            pw = self.SERVO_PWM_MIN_PW
        elif pw > self.SERVO_PWM_MAX_PW:
            pw = self.SERVO_PWM_MAX_PW

        # 将脉宽转换为PCA9685的占空比
        duty = self.pw2duty(pw)
        # 调用pca9685的duty方法， 设置占空比
        self.pca9685.duty(self.servo_idx, duty)

    def pw2duty(self, pw):
        '''
        将脉宽转换为占空比
        注：脉宽的单位是ns
        '''
        return int(pw / self.SERVO_CONTROL_PERIOD * self.PCA9685_PWM_FULL_DUTY)
    
    def duty2pw(self, duty):
        '''
        将占空比转换为脉宽
        '''
        return self.SERVO_CONTROL_PERIOD * (duty / self.PCA9685_PWM_FULL_DUTY)
