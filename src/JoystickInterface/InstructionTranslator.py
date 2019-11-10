#!/usr/bin/python
import sys
import argparse
import math
import json

def deg_to_rad(degree):
    return degree * math.pi / 180

class Config:
    center = None
    radius = None

    def __init__(self, config_name):
        with open(config_name) as config_file:
            data_dict = json.load(config_file)
            self.center = data_dict['center']
            self.center_x = self.center[0]
            self.center_y = self.center[1]
            self.radius = data_dict['radius']

class Translator:
    config = None
    def __init__(self, config_name):
        self.config = Config(config_name)

    def percent_to_mag(self, magnitude_percentage):
        return self.config.radius * magnitude_percentage

    def polar_to_cartesian(self, angle, magnitude):
        rad_angle = deg_to_rad(angle)
        x_coordinate = magnitude * math.cos(rad_angle)
        y_coordinate = magnitude * math.sin(rad_angle)
        translated_x_coordinate = x_coordinate + self.config.center_x
        translated_y_coordinate = y_coordinate + self.config.center_y

        return translated_x_coordinate, translated_y_coordinate







if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-config_name", type = str, default = 'main_config.txt')
    args = parser.parse_args()
    config_name = args.config_name

    new_translator = Translator(config_name)
    print(new_translator.polar_to_cartesian(0,new_translator.percent_to_mag(1)))