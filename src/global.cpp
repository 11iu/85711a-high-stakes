#include "global.hpp"
#include "main.h"
#include "constants.hpp"
#include <string>
#include <unordered_map>

// Variables
bool isBlue = false;

std::unordered_map<std::string, int> ledColors = {
    {"red", 0xFF0000},
    {"green", 0x00FF00},
    {"blue", 0x0000FF},
    {"yellow", 0xFFFF00},
    {"purple", 0x800080},
    {"white", 0xFFFFFF},
    {"off", 0x000000}};

// Objects
pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::Motor lF(LEFT_UPPER_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::Motor lM(LEFT_BACK_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::Motor lB(LEFT_FRONT_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::Motor rF(RIGHT_UPPER_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::Motor rM(RIGHT_BACK_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::Motor rB(RIGHT_FRONT_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);

pros::MotorGroup leftMotors({LEFT_UPPER_PORT, LEFT_BACK_PORT, LEFT_FRONT_PORT}, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::MotorGroup rightMotors({RIGHT_UPPER_PORT, RIGHT_BACK_PORT, RIGHT_FRONT_PORT}, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);

pros::Motor intake(INTAKE_PORT, pros::MotorGears::blue, pros::MotorEncoderUnits::degrees);
pros::Imu imu(IMU_PORT);
pros::adi::DigitalOut clamp(CLAMP_PORT, LOW);
pros::Optical optical(OPTICAL_PORT);
// pros::Vision frontCam(VISION_PORT);
pros::adi::Ultrasonic backUltra(B_ULTRA_OUT_PORT, B_ULTRA_IN_PORT);
pros::adi::Ultrasonic leftUltra(L_ULTRA_OUT_PORT, L_ULTRA_IN_PORT);
// pros::adi::Led leftLeds(L_LED_PORT, L_LED_LENGTH);
// pros::adi::Led rightLeds(R_LED_PORT, R_LED_LENGTH);

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