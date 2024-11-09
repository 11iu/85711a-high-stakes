#include "main.h"
#include "autons.hpp"
#include "field.hpp"
#include "global.hpp"

// statistics for game
unsigned int pistonNum = 0;
unsigned int ringNum = 0;

double log_drive(double v, double pow)
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

static lv_obj_t * selected_btn_auto = NULL;
int selected_auto = 0;

void btn_handler_auto(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_target(e);
    selected_auto = (int) lv_event_get_user_data(e);

    if (e->code == LV_EVENT_PRESSED) {
        if (selected_btn_auto != btn) {
            // Deselect the previously selected button and set back to blue
            if (selected_btn_auto) {
                lv_obj_clear_state(selected_btn_auto, LV_STATE_CHECKED);
                lv_obj_set_style_bg_color(selected_btn_auto, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT); // bitwise or to combine both states
            }
            // Select the new button and set to red
            selected_btn_auto = btn;
            lv_obj_add_state(selected_btn_auto, LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(selected_btn_auto, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN | LV_STATE_CHECKED);
        }
    }
}

void auto_selector(lv_obj_t *parent) {
    lv_obj_clean(parent);

    lv_obj_t * label_btn;

    lv_obj_t * btn_auto1 = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_auto1, btn_handler_auto, LV_EVENT_PRESSED, (void *) 1);
    lv_obj_align(btn_auto1, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_set_height(btn_auto1, LV_SIZE_CONTENT);
    label_btn = lv_label_create(btn_auto1);
    lv_label_set_text(label_btn, "Auto 1");
    lv_obj_center(label_btn);

    lv_obj_t * btn_auto2 = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_auto2, btn_handler_auto, LV_EVENT_PRESSED, (void *) 2);
    lv_obj_align(btn_auto2, LV_ALIGN_TOP_LEFT, 10, 60);
    lv_obj_set_height(btn_auto2, LV_SIZE_CONTENT);
    label_btn = lv_label_create(btn_auto2);
    lv_label_set_text(label_btn, "Auto 2");
    lv_obj_center(label_btn);

    lv_obj_t * btn_auto3 = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_auto3, btn_handler_auto, LV_EVENT_PRESSED, (void *) 3);
    lv_obj_align(btn_auto3, LV_ALIGN_TOP_LEFT, 10, 110);
    lv_obj_set_height(btn_auto3, LV_SIZE_CONTENT);
    label_btn = lv_label_create(btn_auto3);
    lv_label_set_text(label_btn, "Auto 3");
    lv_obj_center(label_btn);

    // Select the first auto by default 
    selected_btn_auto = btn_auto1; 
    lv_obj_add_state(selected_btn_auto, LV_STATE_CHECKED); 
    lv_obj_set_style_bg_color(selected_btn_auto, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN | LV_STATE_CHECKED); 
    selected_auto = 1;
}

void stat_display(lv_obj_t *parent) {
    lv_obj_clean(parent);

    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, "Stats");

    // motor temp (bar to show or change color)

    // motor voltage

    // motor current

    // motor torque
}

void sensor_display(lv_obj_t *parent) {
    lv_obj_clean(parent);

    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, "Sensors");

    // optical sensor hue, sat, bright

    // imu heading / rotation rate

    // battery current, voltage, capacity (bar to show)

    // encoder position, velocity
}

lv_obj_t *tab_autos;
lv_obj_t *tab_stats;
lv_obj_t *tab_sensors;

void create_tab_view() {

    lv_obj_t *tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 30);

    tab_autos = lv_tabview_add_tab(tabview, "Autos");
    tab_stats = lv_tabview_add_tab(tabview, "Statistics");
    tab_sensors = lv_tabview_add_tab(tabview, "Sensors");
    
    // auto tab content
    auto_selector(tab_autos);

    // stat tab content,  add real time data logging (temp of motors, rings scored, time elapsed, clamp amts)
    stat_display(tab_stats);

    // sensor tab content
    sensor_display(tab_sensors);
}

void initialize()
{
    pros::delay(500); // Stop the user from doing anything while
                      // legacy ports configure.
    pros::lcd::initialize();
    chassis.calibrate();

    // lvgl auto selector and info display on brain
    create_tab_view();
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
            master.set_text(0, 0, "Auto 1            ");
            auto1();
            break;
        case 2:
            master.set_text(0, 0, "Auto 2            ");
            auto2();
            break;
        case 3:
            master.set_text(0, 0, "Auto 3            ");
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
    master.set_text(0, 0, "Driver control            ");

    bool clampExt = false;
    
    while (true)
    {

        // drive
        int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        int leftY = log_drive(forward, 3);
        int rightX = log_drive(turn, 3);

        // intake and belt hold R1
        master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) ? intake.move(127) : intake.move(0);

        // clamp toggle for double acting piston (still 1 wire signal, one solenoid is inverted)
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        {
            clampExt ? clamp.set_value(false) : clamp.set_value(true);
            clampExt = !clampExt;
            pistonNum++;
        }

        // move the chassis with arcade drive
        chassis.arcade(leftY, rightX);

        pros::delay(20);
    }
}

void disabled() {
    create_tab_view();
}
