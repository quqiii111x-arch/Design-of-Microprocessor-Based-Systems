import serial
import numpy as np
import cv2

from serial import Serial, SerialException
import time

# try:
#     ser = Serial(
#         port='COM3',
#         baudrate=9600,
#         timeout=1
#     )
#     print(f"已连接 {ser.portstr}")
#
#     # 测试通信
#     ser.write(b'AT\r\n')
#     time.sleep(0.1)
#     response = ser.read_all()
#     print(response.decode('ascii', errors='ignore'))
#
# except SerialException as e:
#     print(f"串口错误: {str(e)}")
#     print("请检查：1. 设备是否连接 2. 端口号是否正确 3. 是否有其他程序占用")
#
# finally:
#     if 'ser' in locals():
#         ser.close()

ser = serial.Serial('COM3', baudrate=115200, timeout=1)

# WIDTH = 20
# HEIGHT = 15
WIDTH = 160
HEIGHT = 120
FRAME_SIZE = WIDTH * HEIGHT * 2
SYNC_HEADER = b'\xAA\x55'

def rgb565_to_bgr888(data):
    img = np.zeros((HEIGHT, WIDTH, 3), dtype=np.uint8)
    for i in range(HEIGHT):
        for j in range(WIDTH):
            idx = 2 * (i * WIDTH + j)
            pixel = (data[idx] << 8) | data[idx + 1]
            r = ((pixel >> 11) & 0x1F) << 3 | ((pixel >> 11) & 0x1F) >> 2
            g = ((pixel >> 5) & 0x3F) << 2 | ((pixel >> 5)  & 0x3F) >> 4
            b = (pixel & 0x1F) << 3 | ((pixel >> 0)  & 0x1F) >> 2
            img[i, j] = [b, g, r]
    return img

def grayscale_to_bgr888(data):
    img = np.zeros((HEIGHT, WIDTH, 3), dtype=np.uint8)

    for i in range(HEIGHT):
        for j in range(WIDTH):
            # 获取当前像素的灰度值（8-bit）
            gray_value = data[i * WIDTH + j]

            # 灰度值直接复制到 B、G、R 通道（生成灰度效果的彩色图）
            img[i, j] = [gray_value, gray_value, gray_value]  # BGR顺序

    return img

def wait_for_frame_start():
    while True:
        byte1 = ser.read(1)
        if byte1 != b'\xAA':
            continue
        byte2 = ser.read(1)
        if byte2 == b'\x55':
            return  

while True:
    #wait_for_frame_start()
    header = ser.read_until(SYNC_HEADER)  # 自动同步帧头
    if not header.endswith(SYNC_HEADER):
        continue

    raw = bytearray()
    while len(raw) < FRAME_SIZE:
        raw += ser.read(FRAME_SIZE - len(raw))

    #帧尾    
    ser.read(2)

    # actual frame num
    num = ser.read(1)
    print(num)

    #img = grayscale_to_bgr888(raw)
    img = rgb565_to_bgr888(raw)
    cv2.imshow('OV7670 Output', img)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

ser.close()
cv2.destroyAllWindows()
