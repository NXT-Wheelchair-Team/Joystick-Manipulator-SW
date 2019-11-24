#!/usr/bin/python
import zmq

port = "5555"
context = zmq.Context()
socket = context.socket(zmq.PAIR)
socket.connect("tcp://localhost:%s" % port)

while True:
    msg = socket.recv_json()
    print(msg)