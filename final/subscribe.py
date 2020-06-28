import paho.mqtt.client as paho

import matplotlib.pyplot as plt

import time

import numpy as np




mqttc = paho.Client()

result = np.zeros(20)

ind = np.arange(0, 20, 1)
iii = 0
# Settings for connection

host = "localhost"

topic= "final_106061123"

port = 1883


# Callbacks

def on_connect(self, mosq, obj, rc):

    print("Connected rc: " + str(rc))


def on_message(mosq, obj, msg):
    global iii, result
    # print("[Received] Topic: " + msg.topic + ", Message: " + str(msg.payload) + "\n")
    print(str(msg.payload) + "\n")

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




while (1):

    # mesg = "Hello, world!"

    # result[i] 
    mqttc.loop()
    # print(a)
    # print(mesg)


    time.sleep(1)

# plt.figure()
# plt.stem(ind, result)
# plt.xlabel('timestamp')
# plt.ylabel('tilt or not')
# plt.title('Tilt Plot')
# plt.show()