import sensor, image, time, struct
from pyb import UART
import math
import utime
from robot_arm import RobotArm3DoF
red_threshold_01=(21,29,-72,6,-11,17)
blue_threshold_01=(26, 12, -15, 25, -46, -6)
orang_threshold_01=(68, 34, -14, 52, 100, 27)
sensor.reset()
uart = UART(1, 57600)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
clock = time.clock()
qr_finish_flag=1
buff=[]
arm = RobotArm3DoF()
img=sensor.snapshot()
blobs=img.find_blobs([orang_threshold_01],area_threshold=150)
last_blobs=blobs
k=1900
x_scale=26.8
y_scale=28.7
scale=(x_scale+y_scale)/4
init_frame=(0.1,15,2.5)
target_x_err=0
length=0
arm.move(init_frame[0],init_frame[1],init_frame[2])
def Send_Data(qr_code,xerr):
    '''串口发送函数'''
    send_data=struct.pack("<BBBB",0x5a,xerr,xerr>>8,qr_code)
    uart.write(send_data)
def QR_Data_Transform(buff):
    '''qr数据转换'''
    qrcodes=0
    if buff == 123:
        qrcodes=1
    elif buff == 132:
        qrcodes=2
    elif buff == 213:
        qrcodes=3
    elif buff == 231:
        qrcodes=4
    elif buff == 312:
        qrcodes=5
    elif buff == 321:
        qrcodes=6
    else:
       qrcodes=0
    return qrcodes
while(True):
    clock.tick()
    img = sensor.snapshot()
    if qr_finish_flag:
        img.lens_corr(1.8)
        codes=img.find_qrcodes()
        for code in codes:
            img.draw_rectangle(code.rect(), color = (255, 0, 0))
            buff=int(code.payload())
            qrcodes=QR_Data_Transform(buff)
        if codes:
           qr_finish_flag=0
           sensor.set_vflip(True)
           Send_Data(qrcodes,target_x_err)
    if not qr_finish_flag:
        if last_blobs:
            for b in blobs:
                x1=b[0]-7
                y1=b[1]-7
                w1=b[2]+12
                h1=b[3]+12
            roi2=(x1,y1,w1,h1)
            blobs=img.find_blobs([orang_threshold_01],roi=roi2,area_threshold=1000)
            last_blobs=blobs
        else:
            blobs=img.find_blobs([orang_threshold_01],area_threshold=1000)
            last_blobs=blobs
        if last_blobs:
            for b in blobs:
                if b[4]>1500:
                    img.draw_rectangle(b[0:4],color=(255,0,0))
                    img.draw_cross(b[5],b[6])
                    target_x_err=((160-b[5])+160)
                    target_y_err=(120-b[6])/scale
                    LM=(b[2]+b[3])/2
                    length=k/LM
                    arm.move(init_frame[0],init_frame[1],init_frame[2]+target_y_err)
                    Send_Data(qrcodes,target_x_err)
        else:
            arm.move(init_frame[0],init_frame[1],init_frame[2])
        #Send_Data(qrcodes,target_x_err)


    print('fps:',clock.fps())
    #print(qrcodes)
