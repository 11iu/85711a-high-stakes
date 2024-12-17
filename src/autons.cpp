#include "autons.hpp"
#include "field.hpp"
#include "global.hpp"

// All created for blue (positive right side)

// Auto far FIXME: untested
void auto_far(bool blue = true)
{
    int mult = (blue) ? 1 : -1;

    chassis.setPose(mult * startUpper.x, startUpper.y, startUpper.theta);
    chassis.moveToPose(mult * mogoTop.x, mogoTop.y, mult * 45, 2000, {.forwards = false, .maxSpeed = 70}, false); // step 1
    clamp.set_value(HIGH);
    pros::delay(50);                                                                                      // may be unnecessary
    chassis.moveToPose(mult * mogoTop.x, mogoTop.y, mult * -40, 2000, {.maxSpeed = 70}, false);           // step 2
    intake.move(127);                                                                                     // start intaking
    chassis.moveToPose(mult * ringTopLower.x, ringTopLower.y, mult * -40, 2000, {.maxSpeed = 70}, false); // step 3
    chassis.moveToPose(mult * ringTopUpper.x, ringTopUpper.y, 0, 2000, {.maxSpeed = 70}, false);          // step 4
    chassis.moveToPose(mult * ringTopOuter.x, ringTopOuter.y, mult * 90, 2000, {.maxSpeed = 70}, false);  // step 5
    chassis.moveToPose(mult * ringRightOuter.x, ringRightOuter.y, 180, 2000, {.maxSpeed = 70}, false);    // step 6
    clamp.set_value(LOW);
}

// Auto close
void auto_close(bool blue = true)
{
    int mult = (blue) ? 1 : -1;

    chassis.setPose(startLower.x * mult, startLower.y, startLower.theta * mult);
    chassis.moveToPose(mult * mogoBottom.x + mult * 0, mogoBottom.y - mult * 2, mult * 120, 2000, {.forwards = false, .maxSpeed = 70}, false);
    clamp.set_value(HIGH); // clamp low mogo
    pros::delay(1000);
    chassis.moveToPose(mult * mogoBottom.x + mult * 0, mogoBottom.y - mult * 2, 180, 2000, {.maxSpeed = 70}, false);
    intake.move(127);                                                                                                   // start intaking
    chassis.moveToPose(mult * ringBottomInner.x + mult * 4, ringBottomInner.y - 2, 180, 2000, {.maxSpeed = 50}, false); // make sure we don't get the red one on the top
    pros::delay(3000);
    intake.move(0);
    chassis.moveToPose(mult * tile * 2, -tile * 2, mult * 135, 2000, {.maxSpeed = 70}, false); // end close to the positive corner
    chassis.moveToPose(mult * tile * 2, -tile * 2, mult * -45, 2000, {.maxSpeed = 70}, false); // spin around so mogo faces corner
}

// only moves forward 1 tile
void auto_basic()
{
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(0, tile, 0, 2000, {.maxSpeed = 70}, false); // move forward one tile
}

void auto_sam()
{
    //auton for far blue
    chassis.setPose(tile * 2, tile, 90, false);
    chassis.moveToPose(tile, tile, 90, 2000, {.forwards = false, .maxSpeed = 70}, false);
    clamp.set_value(HIGH);
    pros::delay(500);
    chassis.turnToPoint(tile, tile * 2, 2000, {.maxSpeed = 70}, false);
    chassis.moveToPose(tile, tile * 2, 0, 2000, {.maxSpeed = 70}, false);
    intake.move(127);
    pros::delay(2000);
    intake.move(0);
    chassis.moveToPose(tile, tile * 2, 280, 2000, {.maxSpeed = 70}, false);
    chassis.moveToPose(7, tile * 2, 280, 2000, {.maxSpeed = 70}, false);
    intake.move(127);
    pros::delay(2000);
    intake.move(0);


}