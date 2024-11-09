#include "autons.hpp"
#include "field.hpp"
#include "global.hpp"

// goes straight
void auto1() {
    chassis.moveToPose(0, 24, 0, 4000);
}

// turns right
void auto2() {
    chassis.moveToPose(0, 0, 90, 4000);
}

// turns left
void auto3() {
    chassis.moveToPose(0, 0, -90, 4000);
}