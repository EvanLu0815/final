import serial

import time

import matplotlib.pyplot as plt

import numpy as np

import paho.mqtt.client as paho

mqttc = paho.Client()


# Settings for connection

host = "localhost"

topic= "final_106061123"

port = 1883


# Callbacks

def on_connect(self, mosq, obj, rc):

    print("Connected rc: " + str(rc))


def on_message(mosq, obj, msg):

    print("[Received] Topic: " + msg.topic + ", Message: " + str(msg.payload) + "\n");


def on_subscribe(mosq, obj, mid, granted_qos):

    print("Subscribed OK")


def on_unsubscribe(mosq, obj, mid, granted_qos):

    print("Unsubscribed OK")


# Set callbacks

mqttc.on_message = on_message

mqttc.on_connect = on_connect

mqttc.on_subscribe = on_subscribe

mqttc.on_unsubscribe = on_unsubscribe


# Connect and subscribe

print("Connecting to " + host + "/" + topic)

mqttc.connect(host, port=1883, keepalive=60)

mqttc.subscribe(topic, 0)


# XBee setting

serdev = '/dev/ttyUSB0'

s = serial.Serial(serdev, 9600)

s.write("+++".encode())

char = s.read(2)

print("Enter AT mode.")

print(char.decode())


s.write("ATMY 0x130\r\n".encode())

char = s.read(3)

print("Set MY 0x130.")

print(char.decode())


s.write("ATDL 0x230\r\n".encode())

char = s.read(3)

print("Set DL 0x230.")

print(char.decode())


s.write("ATID 0x1\r\n".encode())

char = s.read(3)

print("Set PAN ID 0x1.")

print(char.decode())


s.write("ATWR\r\n".encode())

char = s.read(3)

print("Write config.")

print(char.decode())


s.write("ATMY\r\n".encode())

char = s.read(4)

print("MY :")

print(char.decode())


s.write("ATDL\r\n".encode())

char = s.read(4)

print("DL : ")

print(char.decode())


s.write("ATCN\r\n".encode())

char = s.read(3)

print("Exit AT mode.")

print(char.decode())


# print("start sending RPC")
s.write("\r".encode())
time.sleep(1)
# ind = np.arange(0, 20, 1)
# num = np.zeros(20)
# x = np.zeros(20)
# y = np.zeros(20)
# z = np.zeros(20)
# i = 0
while (1):
    char = s.readline()
    print(char.decode())
    mqttc.publish(topic, char.decode())
    time.sleep(1)
    
    # send RPC to remote

    # print("In While\n");

    # s.write("/MyPrint/run\r".encode())

    # char = s.readline()

    # print(char.decode())

    # num[i] = int(char.decode())

    # char = s.readline()
    # print(char.decode())
    # x[i] = float(char.decode())
    
    # char = s.readline()
    # print(char.decode())
    # y[i] = float(char.decode())
    # char = s.readline()
    # print(char.decode())
#     z[i] = float(char.decode())

#     i += 1

#     time.sleep(1)

# for i in range (20):
#     if ((x[i] > 0.707107)| (y[i] > 0.707107)| (x[i] < -0.707107)| (y[i] < -0.707107)):
#         tmp = "1"
#     else:
#         tmp = "0"
#     mqttc.publish(topic, tmp)
#     time.sleep(1)

# plt.figure()
# plt.plot(ind, num)
# plt.xlabel('timestamp')
# plt.ylabel('number')
# plt.title('# collected data plot')
# plt.show()
s.close()
