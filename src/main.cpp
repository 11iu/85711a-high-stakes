#include "autons.hpp"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"

///////////////////////////////////////////////////
// Chassis
///////////////////////////////////////////////////

pros::Controller master(pros::E_CONTROLLER_MASTER);

//  chassis motors
pros::Motor lF(LEFT_FRONT_LOWER_PORT, pros::MotorGears::green, pros::MotorEncoderUnits::degrees);
pros::Motor lM(LEFT_FRONT_UPPER_PORT, pros::MotorGears::green, pros::MotorEncoderUnits::degrees);
pros::Motor lB(LEFT_BACK_PORT, pros::MotorGears::green, pros::MotorEncoderUnits::degrees);
pros::Motor rF(RIGHT_FRONT_LOWER_PORT, pros::MotorGears::green, pros::MotorEncoderUnits::degrees);
pros::Motor rM(RIGHT_FRONT_UPPER_PORT, pros::MotorGears::green, pros::MotorEncoderUnits::degrees);
pros::Motor rB(RIGHT_BACK_PORT, pros::MotorGears::green, pros::MotorEncoderUnits::degrees);

pros::MotorGroup leftMotors({LEFT_FRONT_LOWER_PORT, LEFT_FRONT_UPPER_PORT, LEFT_BACK_PORT}, pros::MotorGears::green, pros::MotorEncoderUnits::degrees);
pros::MotorGroup rightMotors({RIGHT_FRONT_LOWER_PORT, RIGHT_FRONT_UPPER_PORT, RIGHT_BACK_PORT}, pros::MotorGears::green, pros::MotorEncoderUnits::degrees);

// Inertial Sensor
pros::Imu imu(IMU_PORT);

// drivetrain settings
lemlib::Drivetrain drivetrain(
    &leftMotors,                // left motor group
    &rightMotors,               // right motor group
    12,                         // 12 inch track width (left to right wheels)
    lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
    360,                        // drivetrain rpm is 360
    8                           // chase power is 2. If we had traction wheels, it would have been 8
);

lemlib::ControllerSettings
    linearController(15,  // proportional gain (kP) 20 works
                     0,   // integral gain (kI)
                     4,   // derivative gain (kD) 4 works
                     3,   // anti windup
                     1,   // small error range, in inches
                     100, // small error range timeout, in milliseconds
                     3,   // large error range, in inches
                     500, // large error range timeout, in milliseconds
                     40   // maximum acceleration (slew)
    );

// angular motion controller
lemlib::ControllerSettings
    angularController(2,   // proportional gain (kP) 3 works
                      0,   // integral gain (kI)
                      10,  // derivative gain (kD) 15 works
                      3,   // anti windup
                      2,   // small error range, in degrees
                      100, // small error range timeout, in milliseconds
                      3,   // large error range, in degrees
                      500, // large error range timeout, in milliseconds
                      0    // maximum acceleration (slew)
    );

// sensors for odometry
// note that in this example we use internal motor encoders (IMEs), so we don't
// pass vertical tracking wheels
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


///////////////////////////////////////////////////
// Utility Functions
///////////////////////////////////////////////////

double logDrive(double v, double pow)
{
    if (v > 0)
    {
        return (std::pow(std::abs(v), pow) / std::pow(127, pow)) * 127;
    }
    else
    {
        return -1 * (std::pow(std::abs(v), pow) / std::pow(127, pow)) * 127;
    }
}

///////////////////////////////////////////////////
// Main Functions
///////////////////////////////////////////////////

void initialize()
{
    pros::delay(500); // Stop the user from doing anything while
                      // legacy ports configure.
    pros::lcd::initialize();
    chassis.calibrate();
}

void competition_initialize()
{

}

void autonomous()
{

}

void opcontrol()
{
 
    while (true)
    {

        // drive
        int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        int leftY = logDrive(forward, 2);
        int rightX = logDrive(turn, 2);

        // move the chassis with arcade drive
        chassis.arcade(leftY, rightX);

        pros::delay(20);
    }
}
