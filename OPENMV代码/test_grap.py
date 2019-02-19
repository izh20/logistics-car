import utime
from robot_arm import RobotArm3DoF

arm = RobotArm3DoF()


def test_grap_obj():
    '''测试机械臂抓取物块'''
    global arm
    # 缓慢接近物块
    arm.gripper.open_slowly()
    arm.move(0.1, -0.03, 0.0)
    utime.sleep_ms(300)
    arm.move(0.1, -0.055, 0.0)
    utime.sleep_ms(300)
    arm.move(0.125, -0.07, 0.0)
    utime.sleep_ms(300)
    # 到达物块所在的位置
    arm.move(0.15, -0.09, 0.0)
    utime.sleep_ms(500)
    # 缓慢夹取物块
    arm.gripper.grab_slowly()
    utime.sleep_ms(500)
    # 向上抬起一点点
    arm.move(0.15, -0.09, 0.02)
    utime.sleep_ms(300)
    # 中间点
    arm.move(0.135, -0.02, -0.01)
    utime.sleep_ms(300)
    # 到达投放点
    arm.move(0.12, 0.04, -0.04)
    # 缓慢松开爪子
    utime.sleep_ms(500)
    arm.gripper.open_slowly()

test_grap_obj()
