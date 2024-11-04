#include "autons.hpp"
#include "main.h"
#include "field.hpp"
#include "global.hpp"

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

// runs after initialize and before comp switch / field control, e.g. auto selector
void competition_initialize()
{
}

void autonomous()
{
    // // moves in a square
    // chassis.setPose(0, 0, 0);
    // chassis.moveToPose(0, tile * 3, 0, 2000);
    // chassis.moveToPose(0, tile * 3, 90, 2000);
    // chassis.moveToPose(tile * 3, tile * 3, 90, 2000);
    // chassis.moveToPose(tile * 3, tile * 3, 180, 2000);
    // chassis.moveToPose(tile * 3, 0, 180, 2000);
    // chassis.moveToPose(tile * 3, 0, -90, 2000);
    // chassis.moveToPose(0, 0, -90, 2000);
    // chassis.moveToPose(0, 0, 0, 2000);
}

void opcontrol()
{

    while (true)
    {

        // drive
        int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        int leftY = logDrive(forward, 3);
        int rightX = logDrive(turn, 3);

        // intake
        bool intakeOn = master.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
        if (intakeOn)
        {
            intake.move(127);
        }
        else
        {
            intake.move(0);
        }

        // clamp
        bool clampOn = master.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
        if (clampOn)
        {
            clamp.set_value(true);
        }
        else
        {
            clamp.set_value(false);
        }

        // move the chassis with arcade drive
        chassis.arcade(leftY, rightX);

        pros::delay(20);
    }
}
