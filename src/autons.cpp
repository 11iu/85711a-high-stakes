#include "autons.hpp"
#include "field.hpp"
#include "global.hpp"

// All created for red side (positive right side)

// Upper side
void auto1()
{
    chassis.setPose(startUpper);
    chassis.moveToPose(mogoTop.x, mogoTop.y, 90, 2000, {}, false);
    clamp.set_value(HIGH); // pick up
    intake.move(127);      // start intaking
    chassis.moveToPose(ringTopLower.x, ringTopLower.y, -30, 2000, {}, false);
    chassis.moveToPose(ringTopUpper.x, ringTopUpper.y, 0, 2000, {}, false);
    chassis.moveToPose(tile * 2, tile * 2, 90, 2000, {}, false);
}

// Lower side
void auto2()
{
    // chassis.setPose(startLower);
    // chassis.moveToPose(mogoBottom.x + 6, mogoBottom.y, 90, 5000, {.forwards=false, .maxSpeed=40}, false);
    // clamp.set_value(HIGH); // pick up
    // pros::delay(500);
    // intake.move(127);      // start intaking
    // chassis.moveToPose(ringBottomOuter.x + 5, ringBottomOuter.y, 120, 5000, {.maxSpeed=40}, false);
    chassis.setPose(startLowerFar);
    intake.move(127);
    chassis.moveToPose(ringBottomOuter.x-10, ringBottomInner.y+10, -45, 2000, {}, false);
    chassis.moveToPose(ringBottomOuter.x-10, ringBottomInner.y+10, 135, 2000, {}, false); // spin around
    chassis.moveToPose(mogoBottom.x, mogoBottom.y, 135, 2000, {.forwards = false, .maxSpeed = 50}, false);
    clamp.set_value(HIGH);
    pros::delay(2000);  
    intake.move(0);
}

// Test, moves in a 1 tile square
void auto3()
{
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(0, tile, 0, 2000, {}, false);
    chassis.moveToPose(0, tile, 90, 2000, {}, false);
    chassis.moveToPose(tile, tile, 90, 2000, {}, false);
    chassis.moveToPose(tile, tile, 180, 2000, {}, false);
    chassis.moveToPose(tile, 0, 180, 2000, {}, false);
    chassis.moveToPose(tile, 0, 270, 2000, {}, false);
    chassis.moveToPose(0, 0, 270, 2000, {}, false);
    chassis.moveToPose(0, 0, 0, 2000, {}, false);
}