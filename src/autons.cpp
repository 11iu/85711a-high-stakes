#include "autons.hpp"
#include "field.hpp"
#include "global.hpp"

// goes straight
void auto1() {
    chassis.moveToPose(chassis.getPose().x, chassis.getPose().y + 24, 0, 10000);
}

// turns right
void auto2() {
    chassis.moveToPose(chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta + 90, 10000);
}

// turns left
void auto3() {
    chassis.moveToPose(chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta - 90, 10000);
}