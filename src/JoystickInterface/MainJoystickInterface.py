#!/usr/bin/python
import zmq
import time

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
    while True:
        msg = recv_socket.recv()
        msg = msg.decode("utf-8")

        if is_json(msg):
            json_recv = json.loads(msg)
            if not validate_json(json_recv):
                print("JSON is not valid / not conforming to API, please verify message being sent. Ending program...")
                break

            translator = Translator(config_name)
            x,y = translator.polar_to_cartesian(json_recv['Angle'], json_recv['Magnitude'])
            print("X Coordinate: ",x, "Y Coordinate:", y)
            print(msg)
            coord_dict = {'x': x,
                          'y': y}

            send_socket.send_json(coord_dict)

        else:
            print("There was a problem with loading the json")

        time.sleep(1)

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