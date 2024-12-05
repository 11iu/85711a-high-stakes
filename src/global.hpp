#include "main.h"
#include <string>

#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

// Variables
extern bool isBlue;
extern std::unordered_map<std::string, int> ledColors;

// Objects
extern pros::Controller master;
extern pros::Motor lF;
extern pros::Motor lM;
extern pros::Motor lB;
extern pros::Motor rF;
extern pros::Motor rM;
extern pros::Motor rB;
extern pros::Motor intake;
extern pros::Imu imu;
extern pros::adi::DigitalOut clamp;
extern pros::Optical optical;
extern lemlib::Chassis chassis;
extern pros::adi::Ultrasonic backUltra;
extern pros::adi::Ultrasonic leftUltra;
extern pros::adi::Led leds;

#endif