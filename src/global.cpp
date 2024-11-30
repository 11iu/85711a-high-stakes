#include "global.hpp"
#include "main.h"
#include "constants.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::Motor lF(LEFT_MIDDLE_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::Motor lM(LEFT_BACK_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::Motor lB(LEFT_FRONT_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::Motor rF(RIGHT_MIDDLE_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::Motor rM(RIGHT_BACK_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::Motor rB(RIGHT_FRONT_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);

pros::MotorGroup leftMotors({LEFT_MIDDLE_PORT, LEFT_BACK_PORT, LEFT_FRONT_PORT}, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::MotorGroup rightMotors({RIGHT_MIDDLE_PORT, RIGHT_BACK_PORT, RIGHT_FRONT_PORT}, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);

pros::Motor intake(INTAKE_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::Imu imu(IMU_PORT);
pros::adi::DigitalOut clamp(CLAMP_PORT, LOW);
pros::Optical optical(OPTICAL_PORT);

lemlib::Drivetrain drivetrain(
    &leftMotors,                // left motor group
    &rightMotors,               // right motor group
    12.5,                       // 12 inch track width (left to right wheels)
    lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
    360,                        // drivetrain rpm is 360
    8                           // chase power is 2. If we had traction wheels, it would have been 8
);

lemlib::ControllerSettings
    linearController(30,  // proportional gain (kP), 30 works
                     0,   // integral gain (kI), 0 works
                     4,   // derivative gain (kD), 4 works
                     3,   // anti windup
                     1,   // small error range, in inches
                     100, // small error range timeout, in milliseconds
                     3,   // large error range, in inches
                     500, // large error range timeout, in milliseconds
                     40   // maximum acceleration (slew), 40 works
    );

lemlib::ControllerSettings
    angularController(4,   // proportional gain (kP) 3 works
                      0,   // integral gain (kI)
                      20,  // derivative gain (kD) 15 works
                      3,   // anti windup
                      1,   // small error range, in degrees
                      100, // small error range timeout, in milliseconds
                      3,   // large error range, in degrees
                      500, // large error range timeout, in milliseconds
                      0    // maximum acceleration (slew)
    );

lemlib::OdomSensors sensors(
    nullptr, // vertical tracking wheel 1, set to null
    nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
    nullptr, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a
             // second one
    &imu     // inertial sensor
);

lemlib::Chassis chassis(drivetrain, linearController, angularController,
                        sensors);