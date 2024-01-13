import serial # pip install pyserial
import roslibpy # pip install roslibpy
import numpy as np # pip install numpy

client = roslibpy.Ros(host='192.168.0.100', port=9090)
client.run()

talker = roslibpy.Topic(client, '/cmd_vel', 'geometry_msgs/Twist')

def readserial(comport, baudrate):

    ser = serial.Serial(comport, baudrate, timeout=0.1)

    while True:
        data = ser.readline().decode().strip().split(',')
        if data:
            try:
                if client.is_connected:
                    print(data)
                    talker.publish(roslibpy.Message({
                        'linear': {
                        'x': float(data[0])*(0.4/90),
                        'y': 0,
                        'z': 0
                        },
                        'angular': {
                        'x': 0,
                        'y': 0,
                        'z': -np.sign(float(data[0]))*float(data[1])*(0.4/90)
                        }
                    }))
            except:
                print('err')


if __name__ == '__main__':
    readserial('COM3', 9600)
