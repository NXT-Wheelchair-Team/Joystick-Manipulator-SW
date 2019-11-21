import zmq
import argparse

port = "5555"
context = zmq.Context()
socket = context.socket(zmq.PAIR)
socket.connect("tcp://localhost:%s" % port)

def main_run(config_name):
    while True:
        msg = socket.recv()
        msg = msg.decode("utf-8")
        print(msg)



if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-config_name", type = str, default = 'main_config.txt')
    args = parser.parse_args()
    config_name = args.config_name
    main_run(config_name)