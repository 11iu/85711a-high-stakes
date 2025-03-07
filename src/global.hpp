#include "main.h"
#include <string>

#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

// Variables
extern bool isBlue;
// extern std::unordered_map<std::string, int> ledColors;

// Objects
extern pros::Controller master;
extern pros::Motor lT;
extern pros::Motor lM;
extern pros::Motor lB;
extern pros::Motor rT;
extern pros::Motor rM;
extern pros::Motor rB;
extern pros::Motor intake;
extern pros::Motor belt;
extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;

extern pros::adi::DigitalOut clamp;
extern pros::Optical optical;
extern pros::Imu imu;

extern lemlib::Chassis chassis;

// extern pros::adi::DigitalOut leftDoinker;
// extern pros::adi::DigitalOut rightDoinker;
// extern pros::Vision frontCam;
// extern pros::Motor ladyBrown;
// extern pros::adi::Ultrasonic backUltra;
// extern pros::adi::Ultrasonic leftUltra;
// extern pros::adi::AnalogIn potentiometer;
// extern pros::adi::Led leftLeds;
// extern pros::adi::Led rightLeds;

#endif