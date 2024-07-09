#ifndef AUTONS_HPP
#define AUTONS_HPP

#include "lemlib/api.hpp"
#include "constants.hpp"
#include "pros/distance.hpp"

class Autons
{
private:
    lemlib::Chassis &chassis;

public:
    Autons(lemlib::Chassis &chassis);
};

#endif // AUTONS_HPP
