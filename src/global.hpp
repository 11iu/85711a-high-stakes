#include "main.h"

#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

extern pros::Controller master;
extern pros::Motor lF;
extern pros::Motor lM;
extern pros::Motor lB;
extern pros::Motor rF;
extern pros::Motor rM;
extern pros::Motor rB;
extern pros::Motor intake;
extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;
extern pros::Imu imu;
extern pros::adi::DigitalOut clamp;
extern pros::Optical optical;
extern lemlib::Chassis chassis;
extern bool isBlue;

#endif