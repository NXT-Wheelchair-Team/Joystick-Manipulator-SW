#!/usr/bin/python
import sys
import argparse
import json

def generate_config(config_name):
    config_dict = { 'radius' : 1.5,
                    'center' : (1.5, 1.5)}

    with open(config_name, 'w+') as out_file:
        json_obj = json.dumps(config_dict, indent=4)
        out_file.write(json_obj)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-config_name", type = str, default = 'main_config.txt')
    args = parser.parse_args()
    generate_config(args.config_name)