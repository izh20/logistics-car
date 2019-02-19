import math
import utime
from machine import Pin, I2C

from pca9685 import PCA9685
from servos import PCASG90
from configs import config

class Joint:
    '''关节类'''
    ID = 0  # 关节的ID
    MIN_RADIUS = 0 # 关节的最小弧度
    MAX_RADIUS = math.pi # 关节的最大弧度

    def __init__(self):
        self.theta = 0 # 当前关节的角度
        self.servo = None

    def is_radius_legal(self, theta):
        '''判断弧度是否合法'''
        if theta > self.MAX_RADIUS or theta < self.MIN_RADIUS:
            return False
        return True

    def theta2angle(self, theta):
        pass


    def set_radius(self, theta):
        '''设置关节的弧度'''
        if self.is_radius_legal(theta):
            # 将theta转换为舵机的角度
            angle = self.theta2angle(theta)
            # 驱动舵机旋转到特定的角度
            ret = self.servo.set_angle(angle)
            if ret:
                self.theta = theta
                return True
            else:
                # 角度超出了舵机的范围
                print('[ERROR]: Joint_{}, servo angle out of range'.format(self.ID))
        else:
            # 弧度超出了关节的范围
            print('[ERROR]: Joint_{}, joint radius out of range'.format(self.ID))
        return False

    def get_radius(self):

        return self.theta


class Joint1(Joint):
    ID = 1
    MIN_RADIUS = -math.pi / 2
    MAX_RADIUS = math.pi / 2

    def __init__(self, pca9685):
        self.servo = PCASG90(pca9685, 0, min_angle=0, max_angle=180)
        self.servo.set_angle(90)

    def theta2angle(self, theta):
        return math.degrees(theta + math.pi/2)

class Joint2(Joint):
    ID = 2
    MIN_RADIUS = -3*math.pi / 4
    MAX_RADIUS = 0

    def __init__(self, pca9685):
        self.servo = PCASG90(pca9685, 1, min_angle=30, max_angle=138)
        self.servo.set_angle(90)

    def theta2angle(self, theta):
        return  138 + theta*(108/(3*math.pi/4))



class Joint3(Joint):

    ID = 3
    MIN_RADIUS = math.pi/6#math.pi/3
    MAX_RADIUS = 5*math.pi/6

    def __init__(self, pca9685):
        self.servo = PCASG90(pca9685, 2, min_angle=18, max_angle=130)
        self.servo.set_angle(40)

    def set_radius(self, theta2, theta3):
        '''设置关节的弧度
        Joint3比较特别, 他的角度是由两个舵机的角度共同决定的
        '''

        if self.is_radius_legal(theta3):
            # alpha的取值范围是-pi/4 到 pi/2
            alpha = theta2 + theta3

            # 将alpha转换为舵机的角度
            angle = alpha* ((18-90)/(math.pi/2-0)) + 90

            # 驱动舵机旋转到特定的角度
            ret = self.servo.set_angle(angle)
            if ret:
                self.theta3 = theta3
                return True
            else:
                # 角度超出了舵机的范围
                print('[ERROR]: Joint_{}, servo angle out of range'.format(self.ID))
        else:
            # 弧度超出了关节的范围
            print('[ERROR]: Joint_{}, joint radius out of range'.format(self.ID))

        return False

class Gripper:
    '''机械臂的末端执行器-机械爪'''
    def __init__(self, pca9685):
        self.servo = PCASG90(pca9685, 3, min_angle=100, max_angle=150)

    def open(self, angle=150):
        '''机械爪松开'''
        self.servo.set_angle(angle)

    def open_slowly(self):
        '''缓慢释放夹子'''
        for angle in range(100, 150, 1):
            self.grab(angle=angle)
            utime.sleep_ms(20)

    def grab(self, angle=100):
        '''机械臂抓取'''
        self.servo.set_angle(angle)

    def grab_slowly(self):
        '''缓慢夹取'''
        for angle in range(150, 100, -1):
            self.grab(angle=angle)
            utime.sleep_ms(20)

#class block:
 #   '''机械臂所要抓取的色块'''
 #   def __init__(self,)

class RobotArm3DoF:

    def __init__(self, L2=8, L3=8):
        # 创建一个I2C对象
        i2c = I2C(scl=Pin(config['I2C_SCL']), sda=Pin(config['I2C_SDA']),
            freq=config['I2C_FREQUENCY'])
        # 初始化PCA9685对象
        pca9685 = PCA9685(i2c, config['PCA9685_ADDRESS'])
        pca9685.freq(config['PCA9685_PWM_FREQUENCY'])

        self.L2 = L2 # MK1 Link2的长度
        self.L3 = L3 # MK2 Link3的长度
        self.joint1 = Joint1(pca9685) # 定义Joint1
        self.joint2 = Joint2(pca9685) # 定义Joint2
        self.joint3 = Joint3(pca9685) # 定义Joint3
        self.gripper = Gripper(pca9685) # 机械爪

        self.x = 0
        self.y = 0
        self.z = 0
        self.init_joints()

    def init_joints(self):
        '''初始化各个关节'''
        pass

    def inverse_kinematics(self, x, y, z):
        '''MK1机械臂逆向运动学'''
        # 计算目标点举例机械臂基坐标的距离
        distance = math.sqrt(x**2 + y**2 + z**2)
        if distance > (self.L2 + self.L3):
            print('目标点举例机械臂基坐标距离过大')
            return False, None
        # 点在基坐标系的z轴上面
        # MK1机械结构限制 到达不了
        if x==0 and y==0:
            print('目标点在基坐标系的z轴上')
            return False, None

        theta1 = math.atan2(y, x)
        # 判断theta_1的取值范围是满足条件.
        if theta1 < -math.pi/2 or theta1 > math.pi/2:
             print('theta1 角度超出范围')
        #     return False, None

        b = None
        if math.cos(theta1) != 0:
            b = x/math.cos(theta1)
        else:
            b = y/math.sin(theta1)

        # 求解theta3
        cos_theta3 = (z**2 + b**2 - self.L2**2 - self.L3**2)/(2*self.L2*self.L3)
        sin_theta3 = math.sqrt(1 - cos_theta3**2)
        theta3 = math.atan2(sin_theta3, cos_theta3)
        if theta3 < math.pi/6 or theta3 > 5*math.pi/6:
             print('theta3 角度超出范围')
        #     return False, None

        # 求解theta2
        k1 = self.L2 + self.L3 * math.cos(theta3)
        k2 = self.L3 * math.sin(theta3)

        r = math.sqrt(k1**2+k2**2)

        theta2 = math.atan2(-z/r, b/r) - math.atan2(k2/r, k1/r)

        if theta2 < -3*math.pi/4 or theta2 > 0:
            print('theta2 角度超出范围')
        #     return False, None
        return True, (theta1, theta2, theta3)

    def gripper2wrist(self, x, y, z):
        '''将末端执行器的坐标转换成腕关节的坐标'''
        theta = math.atan2(x, y)
        wrist_x = x - 6 * math.sin(theta)
        wrist_y = y - 6 * math.cos(theta)
        wrist_z = z + 1.5 # 末端执行器距离腕关节的垂直距离

        print('Wrist Posi: {}'.format((wrist_x, wrist_y, wrist_z)))
        return wrist_x, wrist_y, wrist_z

    def move(self, x, y, z):
        '''
        让机械臂的末端执行器移动到特定的位置
        '''
        
        if x==0 and y==0:
            print('[Error] fail to move to ({}, {},{})'.format(x, y, z))
            return False

        wrist_x, wrist_y, wrist_z = self.gripper2wrist(x, y, z)
        # 逆向运动学求解
        ret, result = self.inverse_kinematics(wrist_x, wrist_y, wrist_z)
        if ret:
            print('Joint thetas: {}'.format(result))
            theta1, theta2, theta3 = result
            ret1 = self.joint1.set_radius(theta1)
            ret2 = self.joint2.set_radius(theta2)
            ret3 = self.joint3.set_radius(theta2,theta3)
            print('Joint angle: ({},{},{})'.format(theta1*180/math.pi,theta2*180/math.pi,theta3*180/math.pi))
            if ret1 and ret2 and ret3:
                self.x = x
                self.y = y
                self.z = z
                return True

        print('[Error] fail to move to ({}, {},{})'.format(x, y, z))
        return False
