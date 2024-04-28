#ifndef AUTONS_HPP
#define AUTONS_HPP

#include "constants.hpp"
#include "lemlib/api.hpp"

class Autons
{
private:
    lemlib::Chassis &chassis;

public:
    Autons(lemlib::Chassis &chassis);

    void autoDisabled();
};

#endif // AUTONS_HPP