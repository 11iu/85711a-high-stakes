#include "autons.hpp"
#include "main.h"
#include "field.hpp"
#include "global.hpp"

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

static lv_obj_t * label;

static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);

    /*Refresh the text*/
    lv_label_set_text_fmt(label, "%"LV_PRId32, lv_slider_get_value(slider));
    lv_obj_align_to(label, slider, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/
}

void initialize()
{
    pros::delay(500); // Stop the user from doing anything while
                      // legacy ports configure.
    pros::lcd::initialize();
    chassis.calibrate();

    // lvgl stuff
    //TODO: add real time data logging (temp of motors, rings scored, time elapsed, clamp amts)

    lv_obj_clean(lv_scr_act()); // clear the screen of vex crap

    /*Create a slider in the center of the display*/
    lv_obj_t * slider = lv_slider_create(lv_scr_act());
    lv_obj_set_width(slider, 200);                          /*Set the width*/
    lv_obj_center(slider);                                  /*Align to the center of the parent (screen)*/
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);     /*Assign an event function*/

    /*Create a label above the slider*/
    label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "0");
    lv_obj_align_to(label, slider, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/
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
