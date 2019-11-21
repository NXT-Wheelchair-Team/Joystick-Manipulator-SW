#!/usr/bin/python
import zmq

def setup_recv_socket(ip, port):
    context = zmq.Context()
    socket = context.socket(zmq.PAIR)
    socket.connect("tcp://{}:{}".format(ip, port))  # localhost
    return socket


def setup_send_socket(ip, port):
    context = zmq.Context()
    socket = context.socket(zmq.PAIR)
    socket.bind("tcp://{}:{}".format(ip, port))  # *
    return socket