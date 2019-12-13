#!/usr/bin/python
import zmq
import random
import sys
import json
import time


port = "9784"
context = zmq.Context()
socket = context.socket(zmq.PAIR)
socket.bind("tcp://*:%s" % port)

test_dict = { 'Angle': 0,
              'Magnitude': .5}

broken_dict = { 'Angle': -150,
                'Magnitude': .5}

broken_dict2 = { 'Angle': 150,
                 'Magnitude': -2.0}

broken_dict3 = { 'Angle': 'brother',
                 'Magnitude': -2.0}

broken_dict4 = { 'Angle': 150,
                 'Magnitude': 'mother'}

json_obj = json.dumps(test_dict)
# json_obj = json.dumps(broken_dict)
# json_obj = json.dumps(broken_dict2)
# json_obj = json.dumps(broken_dict3)
# json_obj = json.dumps(broken_dict4)


while True:
    # socket.send_string("Server message to client3")
    print("Sending: " + json_obj)
    socket.send_string(json_obj)
    # msg = socket.recv()
    # print(msg)
    time.sleep(1)