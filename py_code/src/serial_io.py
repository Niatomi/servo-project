import serial  # библиотека для работы с serial портом
import sys
import numpy as np

import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt


def connection_plata(list_comports='COM1'):
    # Подключение к плате
    speed = 115200
    com_port = '/dev/ttyUSB0'
    ArduinoSerial = serial.Serial(com_port, speed, xonxoff=False, timeout = None)  # Установка параметров платы
    return ArduinoSerial

def get_serial_data(arduino, send_cmd, cols, rows):
    massive = []
    arduino.write(send_cmd.encode()) #Отправка данных на ардуино
    for i in range(cols*rows):  # Тут в скобках указать размерность принимаемого массива сейчас 32 на 32.
        data = arduino.readline()
        data = data.decode('UTF-8')
        massive.append(int(data))
    massive = np.array(massive)
    massive = np.reshape(massive, (-1, cols)) #Для изменения размера масива, поскольку изначально все значения в 1 строку
    #massive = massive.T
    return np.rot90(massive, 2)



def main():
    # list_comports = serial_ports()
    # print(list_comports)
    
    arduino = connection_plata()
    
    min_fi = 70
    max_fi = 110
    
    min_tetta = 60
    max_tetta = 100
    
    cols = 3
    rows = 3
    
    stop_time = 0 #не менее 300-400 иначе не успевает измерить
    speed = 9 #желательно 8-9 иначе может не успевать измерить
    
    send_cmd = f'start {min_fi} {max_fi} {min_tetta} {max_tetta} {cols} {rows} {stop_time} {speed}'
    lidar_data = get_serial_data(arduino, send_cmd, cols, rows)
    
    plt.imshow(lidar_data, cmap='hot', interpolation='nearest', )
    plt.colorbar()
    plt.show()

if __name__ == '__main__':
    main()
