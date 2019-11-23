#!/usr/bin/python
import time

import zmq
import time
import serial

from ipykernel import serialize
from serial.urlhandler.protocol_alt import serial_class_for_url

from src.JoystickInterface import Communications as comms
from src.JoystickInterface.InstructionTranslator import *

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


def main_run(config_name, recv_socket, send_socket):
    time_of_last_msg = 0
    with serial.Serial("COM9", 9600) as ser:
        while True:
            msg = recv_socket.recv_json()
            # print(msg)

            # recv_socket.send_string("client message to server1")
            # recv_socket.send_string("client message to server2")

            if not validate_json(msg):
                print("JSON is not valid / not conforming to API, please verify message being sent. Ending program...")
                break

            translator = Translator(config_name)
            x,y = translator.polar_to_cartesian(msg['Angle'], msg['Magnitude'])
            if time.time() - time_of_last_msg > 4:
                if x > 2:
                    x = 2
                elif x < 1:
                    x = 1
                if y > 2:
                    y = 2
                elif y < 1:
                    y = 1

                # print("X Coordinate: ", x, "Y Coordinate:", y)
                command_to_send = "<{},{}>".format(x, y)
                print("Before")
                ser.write(command_to_send.encode())
                time_of_last_msg = time.time()
                print(command_to_send)

            time.sleep(.001)

def setup_comms():

    recv_socket = comms.setup_recv_socket('localhost', 9784)
    send_socket = comms.setup_send_socket('*', 5555)

    return recv_socket, send_socket



if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-config_name", type = str, default = 'main_config.txt')
    args = parser.parse_args()
    config_name = args.config_name

    recv_socket, send_socket = setup_comms()


    main_run(config_name, recv_socket, send_socket)