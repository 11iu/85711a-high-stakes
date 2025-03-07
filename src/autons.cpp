#include "autons.hpp"
#include "field.hpp"
#include "global.hpp"

// All created for blue (positive right side)

// Auto far FIXME: untested
void auto_far(bool blue = true)
{
    // int mult = (blue) ? 1 : -1;

    // chassis.setPose(mult * startUpper.x, startUpper.y, startUpper.theta);
    // chassis.moveToPose(mult * mogoTop.x, mogoTop.y, mult * 45, 2000, {.forwards = false, .maxSpeed = 70}, false); // step 1
    // clamp.set_value(HIGH);
    // pros::delay(50);                                                                                      // may be unnecessary
    // chassis.moveToPose(mult * mogoTop.x, mogoTop.y, mult * -40, 2000, {.maxSpeed = 70}, false);           // step 2
    // intake.move(127);                                                                                     // start intaking
    // chassis.moveToPose(mult * ringTopLower.x, ringTopLower.y, mult * -40, 2000, {.maxSpeed = 70}, false); // step 3
    // chassis.moveToPose(mult * ringTopUpper.x, ringTopUpper.y, 0, 2000, {.maxSpeed = 70}, false);          // step 4
    // chassis.moveToPose(mult * ringTopOuter.x, ringTopOuter.y, mult * 90, 2000, {.maxSpeed = 70}, false);  // step 5
    // chassis.moveToPose(mult * ringRightOuter.x, ringRightOuter.y, 180, 2000, {.maxSpeed = 70}, false);    // step 6
    // clamp.set_value(LOW);

    leftMotors.move(-80);
    rightMotors.move(-80);
    pros::delay(1000);
    leftMotors.move(0);
    rightMotors.move(0);
    pros::delay(600);
    clamp.set_value(HIGH); // clamp
    pros::delay(400);
    intake.move(127);
    pros::delay(1000);
    intake.move(0);
    pros::delay(1000);
}

// Auto close
void auto_close(bool blue = true)
{
    // int mult = (blue) ? 1 : -1;

    // chassis.setPose(startLower.x * mult, startLower.y, startLower.theta * mult);
    // chassis.moveToPose(mult * mogoBottom.x + mult * 0, mogoBottom.y - mult * 2, mult * 120, 2000, {.forwards = false, .maxSpeed = 70}, false);
    // clamp.set_value(HIGH); // clamp low mogo
    // pros::delay(1000);
    // chassis.moveToPose(mult * mogoBottom.x + mult * 0, mogoBottom.y - mult * 2, 180, 2000, {.maxSpeed = 70}, false);
    // intake.move(127);                                                                                                   // start intaking
    // chassis.moveToPose(mult * ringBottomInner.x + mult * 4, ringBottomInner.y - 2, 180, 2000, {.maxSpeed = 50}, false); // make sure we don't get the red one on the top
    // pros::delay(3000);
    // intake.move(0);
    // chassis.moveToPose(mult * tile * 2, -tile * 2, mult * 135, 2000, {.maxSpeed = 70}, false); // end close to the positive corner
    // chassis.moveToPose(mult * tile * 2, -tile * 2, mult * -45, 2000, {.maxSpeed = 70}, false); // spin around so mogo faces corner

    leftMotors.move(-80);
    rightMotors.move(-80);
    pros::delay(900);
    leftMotors.move(0);
    rightMotors.move(0);
    pros::delay(500);
    clamp.set_value(HIGH); // clamp
    pros::delay(400);
    intake.move(127);
    pros::delay(1000);
    intake.move(127);
    leftMotors.move(60);
    rightMotors.move(70);
    pros::delay(1800);
    leftMotors.move(0);
    rightMotors.move(0);
    pros::delay(2000);
    intake.move(0);
}

// only moves forward 1 tile
void auto_basic()
{
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(0, tile, 0, 2000, {}, false); // move forward one tile
}

void auto_skills()
{
    chassis.setPose(-2.5 * tile, 0, 90); // setup not touching alliance

    // PRELOAD
    belt.move(127); // score alliance
    pros::delay(500);
    belt.move(0);

    // BOTTOM MOGO
    chassis.moveToPose(-2 * tile - 2, 0, 90, 2000, {.maxSpeed=60}, false);                                         // move forward to line up with mogo
    chassis.moveToPose(-2 * tile - 2, 0, 0, 2000, {.maxSpeed=60}, false);                                          // turn to back up into mogo on bottom
    chassis.moveToPose(-2 * tile - 2, -tile + 4, 0, 2000, {.forwards = false, .minSpeed=80}, false); // drive into mogo
    pros::delay(500);
    clamp.set_value(HIGH); // clamp mogo
    pros::delay(400);
    chassis.moveToPose(-2 * tile + 1, -tile, 90, 2000, {}, false); // turn towards ring
    intake.move(127);
    belt.move(127);
    chassis.moveToPose(-tile, -tile, 90, 2000, {}, false);                              // intake ring
    chassis.moveToPose(-tile, -tile, 180, 2000, {}, false);                             // turn towards 2nd ring
    chassis.moveToPose(-tile, -1.8 * tile, 180, 2000, {}, false);                         // intake 2nd ring below that one
    chassis.moveToPose(-tile, -1.8 * tile, -90, 2000, {}, false);                         // turn towards 3rd and 4th ring
    chassis.moveToPose(-2.2 * tile, -2 * tile, -90, 2000, {}, false);                   // intake 3rd and 4th ring in a line
    chassis.moveToPose(-2.2 * tile, -2 * tile, 135, 2000, {}, false);                   // turn towards 5th ring
    chassis.moveToPose(-1.8 * tile, -2.3 * tile, 135, 2000, {}, false);                 // intake 5th ring
    chassis.moveToPose(-1.8 * tile, -2.3 * tile, 60, 2000, {}, false);                  // back up to have mogo face corner
    chassis.moveToPose(-2.5 * tile, -2.5 * tile, 45, 2000, {.forwards = false}, false); // put mogo in corner
    clamp.set_value(LOW);
    pros::delay(400);
    intake.move(0);
    belt.move(0);

    // // TOP MOGO
    // chassis.moveToPose(-2.3 * tile, -2.3 * tile, 45, 2000, {.minSpeed=80}, false); // go out of the corner
    // chassis.moveToPose(-2 * tile, 1 * tile, 0, 2000, {}, false); // rush to top mogo
    // chassis.moveToPose(-2 * tile, 1 * tile, 180, 2000, {}, false); // turn away from mogo
    // chassis.moveToPose(-2 * tile, 1 * tile, 180, 2000, {.forwards=false}, false); // clamp mogo
    // clamp.set_value(HIGH);
    // pros::delay(400);
}

void auto_far_yedong()
{
    chassis.setPose(tile * 2.5, tile * 1.5, 90, false);
    chassis.moveToPose(tile, tile, 45, 2000, {.forwards = false, .maxSpeed = 70}, false);
    clamp.set_value(HIGH);
    pros::delay(500);
    chassis.moveToPose(tile, tile, 0, 2000, {.maxSpeed = 70}, false);
    intake.move(127);
    chassis.moveToPose(tile, tile * 2, 0, 2000, {.maxSpeed = 70}, false); // get first ring
    chassis.moveToPose(tile, tile * 2, 270, 2000, {.forwards = false, .maxSpeed = 70}, false);
    chassis.moveToPose(tile * 0.2, tile * 1.8, 0, 2000, {.maxSpeed = 70}, false);
    chassis.moveToPose(tile * 0.2, tile * 2.2, 0, 2000, {.maxSpeed = 70}, false);
    // do a back and forth manuver for the two rings near the border
    chassis.moveToPose(tile, tile * 2, 90, 2000, {.maxSpeed = 70}, false);
    chassis.moveToPose(tile, tile * 2, 180, 2000, {.maxSpeed = 70}, false);
    // chassis.moveToPose(tile, tile * 2, 153, 2000, {.maxSpeed = 70}, false);
    // chassis.moveToPose(tile * 2, 0, 153, 2000, {.maxSpeed = 70}, false);
    // going for the ring at (2,0) last
    chassis.moveToPose(tile, tile * 0.15, 180, 2000, {.maxSpeed = 70}, false);
}

void solo_awp_yedong()
{
    chassis.setPose(tile * 2.5, -tile * 1.5, 90, false);
    chassis.moveToPose(tile, -tile, 135, 2000, {.forwards = false, .maxSpeed = 70}, false);
    clamp.set_value(HIGH);
    pros::delay(500);
    chassis.moveToPose(tile, -tile, 180, 2000, {.maxSpeed = 70}, false);
    intake.move(127);
    chassis.moveToPose(tile, -tile * 2, 180, 2000, {.maxSpeed = 70}, false);
    clamp.set_value(LOW);
    chassis.moveToPose(tile, -tile * 2, 45, 2000, {.maxSpeed = 70}, false);
    chassis.moveToPose(tile * 2, 0, 0, 2000, {.maxSpeed = 70}, false);
    pros::delay(500);
    intake.move(0);
    chassis.moveToPose(tile * 2, 0, 135, 2000, {.maxSpeed = 70}, false);
    chassis.moveToPose(tile, tile, 135, 2000, {.forwards = false, .maxSpeed = 70}, false);
    clamp.set_value(HIGH);
    intake.move(127);
    chassis.moveToPose(tile, tile, 0, 2000, {.forwards = false, .maxSpeed = 70}, false);
    chassis.moveToPose(tile, tile * 2, 0, 2000, {.maxSpeed = 70}, false);
    chassis.moveToPose(tile, tile * 2, 180, 2000, {.maxSpeed = 70}, false);
    intake.move(0);
    clamp.set_value(LOW);
    chassis.moveToPose(tile, 0, 180, 2000, {.maxSpeed = 70}, false);
}
