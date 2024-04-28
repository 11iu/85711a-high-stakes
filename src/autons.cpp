#include "autons.hpp"
#include "field.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/rtos.hpp"
#include <utility>

Autons::Autons(lemlib::Chassis &chassis)
    : chassis(chassis) {}

void Autons::autoDisabled()
{
    // do nothing
}
