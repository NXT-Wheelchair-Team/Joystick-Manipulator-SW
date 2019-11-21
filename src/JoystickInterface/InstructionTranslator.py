#!/usr/bin/python
import sys
import argparse
import math
import json

def deg_to_rad(degree):
    """
    Takes in a degree and returns it in radians
    :param degree: Number in degrees
    :return: Returns number in radians
    """
    return degree * math.pi / 180

class Config:
    """
    Configuration class - Takes the configuration file in and parses the data
    """
    center = None
    radius = None

    def __init__(self, config_name):
        """
        Initializes the configuration via the config name
        :param config_name: A config file (.txt) that has configuration information
        """
        with open(config_name) as config_file:
            data_dict = json.load(config_file)
            self.center = data_dict['center']
            self.center_x = self.center[0]
            self.center_y = self.center[1]
            self.radius = data_dict['radius']

class Translator:
    """
    Translator class to translate coordinates from polar to cartesian
    TODO: Maybe add features to translate in the opposite direction
    """
    config = None
    def __init__(self, config_name):
        """
        Creates a config object in initialization
        :param config_name:
        """
        self.config = Config(config_name)

    def percent_to_mag(self, magnitude_percentage):
        """
        Change a percentage magnitude (0.0 -> 1.0) and turns it into an actual magnitude based on the configuration's radius
        :param magnitude_percentage: Magnitude in (0.0 -> 1.0)
        :return: Returns a magnitude
        """
        return self.config.radius * magnitude_percentage

    def polar_to_cartesian(self, angle, magnitude_percentage):
        """
        Converts polar parameters to a cartesian format where the parameters are directed outwards from the config center x/y
        :param angle: An angle in degrees
        :param magnitude: A magnitude percentage
        :return: Returns an x coordinate and a y coordinate from the center of the joystick
        """
        rad_angle = deg_to_rad(angle)
        magnitude = self.percent_to_mag(magnitude_percentage)
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