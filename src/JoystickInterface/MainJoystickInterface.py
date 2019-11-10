#!/usr/bin/python
import sys
import argparse
import zmq
import random
import sys
import time
import json

from src.JoystickInterface.InstructionTranslator import *

port = "5556"
context = zmq.Context()
socket = context.socket(zmq.PAIR)
socket.connect("tcp://localhost:%s" % port)


def is_json(myjson):
  try:
    json.loads(myjson)
  except ValueError as e:
    return False
  return True



def validate_json(json_dict):

    def check_angle(angle):
        if type(angle) != int:
            print("Angle is not an integer!")
            print("Broken Input: {}".format(angle))
            return False
        if not 0 <= angle <= 359:
            print("Angle is not within 0 and 359!")
            print("Broken Input: {}".format(angle))
            return False
        return True
    def check_magnitude(magnitude):
        if type(magnitude) != float and type(magnitude) != int:
            print("Magnitude is not a float!")
            print("Broken Input: {}".format(magnitude))
            return False
        if not 0.0 <= magnitude <= 1.0:
            print("Magnnitude is not within 0.0 and 1.0 range")
            print("Broken Input: {}".format(magnitude))
            return False
        return True



    if not check_angle(json_dict['Angle']):
        return False
    if not check_magnitude(json_dict['Magnitude']):
        return False
    return True


def main_run(config_name):
    while True:
        msg = socket.recv()
        msg = msg.decode("utf-8")

        socket.send_string("client message to server1")
        socket.send_string("client message to server2")


        if is_json(msg) == True:
            json_recv = json.loads(msg)
            if not validate_json(json_recv):
                print("JSON is not valid / not conforming to API, please verify message being sent. Ending program...")
                break

            translator = Translator(config_name)
            true_magnitude = translator.percent_to_mag(json_recv['Magnitude'])
            x,y = translator.polar_to_cartesian(json_recv['Angle'], true_magnitude)
            print("X Coordinate: ",x, "Y Coordinate:", y)
            print(msg)
        else:
            print("There was a problem with loading the json")

        time.sleep(1)




if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-config_name", type = str, default = 'main_config.txt')
    args = parser.parse_args()
    config_name = args.config_name
    main_run(config_name)