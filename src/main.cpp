#include "main.h"
#include "autons.hpp"
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

static lv_obj_t * selected_btn = NULL;
int selected_auto = 0;

void button_handler(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_target(e);
    selected_auto = (int) lv_event_get_user_data(e);

    if (e->code == LV_EVENT_PRESSED) {
        if (selected_btn != btn) {
            // Deselect the previously selected button and set back to blue
            if (selected_btn) {
                lv_obj_clear_state(selected_btn, LV_STATE_CHECKED);
                lv_obj_set_style_bg_color(selected_btn, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT); // bitwise or to combine both states
            }
            // Select the new button and set to red
            selected_btn = btn;
            lv_obj_add_state(selected_btn, LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(selected_btn, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN | LV_STATE_CHECKED);
        }
    }
}

void auto_selector(void) {
    lv_obj_clean(lv_scr_act());

    lv_obj_t * title_auto = lv_label_create(lv_scr_act());
    lv_label_set_text(title_auto, "Select auto:");
    lv_obj_align(title_auto, LV_ALIGN_TOP_LEFT, 10, 10);

    lv_obj_t * label_btn;

    lv_obj_t * btn_auto1 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn_auto1, button_handler, LV_EVENT_PRESSED, (void *) 1);
    lv_obj_align(btn_auto1, LV_ALIGN_TOP_LEFT, 10, 40);
    lv_obj_set_height(btn_auto1, LV_SIZE_CONTENT);
    label_btn = lv_label_create(btn_auto1);
    lv_label_set_text(label_btn, "Auto 1");
    lv_obj_center(label_btn);

    lv_obj_t * btn_auto2 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn_auto2, button_handler, LV_EVENT_PRESSED, (void *) 2);
    lv_obj_align(btn_auto2, LV_ALIGN_TOP_LEFT, 10, 100);
    lv_obj_set_height(btn_auto2, LV_SIZE_CONTENT);
    label_btn = lv_label_create(btn_auto2);
    lv_label_set_text(label_btn, "Auto 2");
    lv_obj_center(label_btn);

    lv_obj_t * btn_auto3 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn_auto3, button_handler, LV_EVENT_PRESSED, (void *) 3);
    lv_obj_align(btn_auto3, LV_ALIGN_TOP_LEFT, 10, 160);
    lv_obj_set_height(btn_auto3, LV_SIZE_CONTENT);
    label_btn = lv_label_create(btn_auto3);
    lv_label_set_text(label_btn, "Auto 3");
    lv_obj_center(label_btn);

    // Select the first auto by default 
    selected_btn = btn_auto1; 
    lv_obj_add_state(selected_btn, LV_STATE_CHECKED); 
    lv_obj_set_style_bg_color(selected_btn, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN | LV_STATE_CHECKED); 
    selected_auto = 1;
}

void initialize()
{
    pros::delay(500); // Stop the user from doing anything while
                      // legacy ports configure.
    pros::lcd::initialize();
    chassis.calibrate();

    // lvgl stuff
    //TODO: add real time data logging (temp of motors, rings scored, time elapsed, clamp amts)
    auto_selector();
}

// runs after initialize and before comp switch / field control, e.g. auto selector
void competition_initialize()
{
}

void autonomous()
{
    lv_obj_clean(lv_scr_act());

    lv_obj_t * title_auto = lv_label_create(lv_scr_act());
    std::string title = "Running auto " + std::to_string(selected_auto);
    lv_label_set_text(title_auto, title.c_str());
    lv_obj_align(title_auto, LV_ALIGN_CENTER, 0, 0);

    switch (selected_auto) {
        case 1:
            master.set_text(0, 0, "Auto 1");
            auto1();
            break;
        case 2:
            master.set_text(0, 0, "Auto 2");
            auto2();
            break;
        case 3:
            master.set_text(0, 0, "Auto 3");
            auto3();
            break;
        default:
            break;
    }
}

void opcontrol()
{
    lv_obj_clean(lv_scr_act());
    lv_obj_t * title_driver = lv_label_create(lv_scr_act());
    lv_label_set_text(title_driver, "Running driver control");
    lv_obj_align(title_driver, LV_ALIGN_CENTER, 0, 0);
    master.set_text(0, 0, "Driver control");
    
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

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            master.rumble("... ---");
        }


        // move the chassis with arcade drive
        chassis.arcade(leftY, rightX);

        pros::delay(20);
    }
}
