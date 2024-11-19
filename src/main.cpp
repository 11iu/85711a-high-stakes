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

static lv_obj_t *selected_btn_auto = NULL;
int selected_auto = 0;

void btn_handler_auto(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);
    selected_auto = (int)lv_event_get_user_data(e);

    if (e->code == LV_EVENT_PRESSED)
    {
        if (selected_btn_auto != btn)
        {
            // Deselect the previously selected button and set back to blue
            if (selected_btn_auto)
            {
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

void auto_selector(lv_obj_t *parent)
{
    lv_obj_clean(parent);

    lv_obj_t *label_btn;

    lv_obj_t *btn_auto1 = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_auto1, btn_handler_auto, LV_EVENT_PRESSED, (void *)1);
    lv_obj_align(btn_auto1, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_set_height(btn_auto1, LV_SIZE_CONTENT);
    label_btn = lv_label_create(btn_auto1);
    lv_label_set_text(label_btn, "Auto 1");
    lv_obj_center(label_btn);

    lv_obj_t *btn_auto2 = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_auto2, btn_handler_auto, LV_EVENT_PRESSED, (void *)2);
    lv_obj_align(btn_auto2, LV_ALIGN_TOP_LEFT, 10, 60);
    lv_obj_set_height(btn_auto2, LV_SIZE_CONTENT);
    label_btn = lv_label_create(btn_auto2);
    lv_label_set_text(label_btn, "Auto 2");
    lv_obj_center(label_btn);

    lv_obj_t *btn_auto3 = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_auto3, btn_handler_auto, LV_EVENT_PRESSED, (void *)3);
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

void stat_display(lv_obj_t *parent)
{
    lv_obj_clean(parent);

    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, "Stats");

    // motor name titles, horizontal on top
    std::vector<lv_obj_t *> motor_titles(7, lv_label_create(lv_scr_act()));
    std::vector<std::string> motor_names = {"lF", "lM", "lB", "rF", "rM", "rB", "intake"};
    for (int i = 0; i < motor_titles.size(); i++)
    {
        lv_label_set_text(motor_titles[i], motor_names[i].c_str());
        lv_obj_align(motor_titles[i], LV_ALIGN_TOP_LEFT, 50 * i + 30, 10);
    }

    // stat category titles, vertical on left
    std::vector<lv_obj_t *> stat_titles(5, lv_label_create(lv_scr_act()));
    std::vector<std::string> stat_names = {"Temp", "Voltage", "Current", "Torque", "Connected"};
    for (int i = 0; i < stat_titles.size(); i++)
    {
        lv_label_set_text(stat_titles[i], stat_names[i].c_str());
        lv_obj_align(stat_titles[i], LV_ALIGN_TOP_LEFT, 0, 10 + 30 * i);
    }

    // motor temp labels, TODO: make it red if its too high or add a bar
    std::vector<double> motor_temps = {lF.get_temperature(), lM.get_temperature(), lB.get_temperature(), rF.get_temperature(), rM.get_temperature(), rB.get_temperature(), intake.get_temperature()};
    std::vector<lv_obj_t *> motor_temp_titles(7, lv_label_create(lv_scr_act()));
    for (int i = 0; i < motor_titles.size(); i++)
    {
        lv_label_set_text_fmt(motor_temp_titles[i], "%f", motor_temps[i]);
        lv_obj_align(motor_temp_titles[i], LV_ALIGN_TOP_LEFT, 50 * i + 30, 40);
    }

    // motor voltage
    std::vector<double> motor_voltages = {lF.get_voltage() / 1000.0, lM.get_voltage() / 1000.0, lB.get_voltage() / 1000.0, rF.get_voltage() / 1000.0, rM.get_voltage() / 1000.0, rB.get_voltage() / 1000.0, intake.get_voltage() / 1000.0};
    std::vector<lv_obj_t *> motor_voltage_titles(7, lv_label_create(lv_scr_act()));
    for (int i = 0; i < motor_titles.size(); i++)
    {
        lv_label_set_text_fmt(motor_voltage_titles[i], "%f", motor_voltages[i]);
        lv_obj_align(motor_voltage_titles[i], LV_ALIGN_TOP_LEFT, 50 * i + 30, 70);
    }

    // motor current
    std::vector<double> motor_currents = {lF.get_current_draw() / 1000.0, lM.get_current_draw() / 1000.0, lB.get_current_draw() / 1000.0, rF.get_current_draw() / 1000.0, rM.get_current_draw() / 1000.0, rB.get_current_draw() / 1000.0, intake.get_current_draw() / 1000.0};
    std::vector<lv_obj_t *> motor_current_titles(7, lv_label_create(lv_scr_act()));
    for (int i = 0; i < motor_titles.size(); i++)
    {
        lv_label_set_text_fmt(motor_current_titles[i], "%f", motor_currents[i]);
        lv_obj_align(motor_current_titles[i], LV_ALIGN_TOP_LEFT, 50 * i + 30, 100);
    }

    // motor torque
    std::vector<double> motor_torques = {lF.get_torque(), lM.get_torque(), lB.get_torque(), rF.get_torque(), rM.get_torque(), rB.get_torque(), intake.get_torque()};
    std::vector<lv_obj_t *> motor_torque_titles(7, lv_label_create(lv_scr_act()));
    for (int i = 0; i < motor_titles.size(); i++)
    {
        lv_label_set_text_fmt(motor_torque_titles[i], "%f", motor_torques[i]);
        lv_obj_align(motor_torque_titles[i], LV_ALIGN_TOP_LEFT, 50 * i + 30, 130);
    }

    // motor connected
    std::vector<bool> motor_connected = {lF.is_installed(), lM.is_installed(), lB.is_installed(), rF.is_installed(), rM.is_installed(), rB.is_installed(), intake.is_installed()};
    std::vector<lv_obj_t *> motor_connected_titles(7, lv_label_create(lv_scr_act()));
    std::vector<int> disconnected_motors;
    for (int i = 0; i < motor_titles.size(); i++)
    {
        if (!motor_connected[i])
        {
            disconnected_motors.push_back(i);
        }
        lv_label_set_text_fmt(motor_connected_titles[i], "%d", motor_connected[i]);
        lv_obj_align(motor_connected_titles[i], LV_ALIGN_TOP_LEFT, 50 * i + 30, 160);
    }

    // vibrate controller and show disconnected motors
    if (disconnected_motors.size() > 0)
    {
        master.rumble(".-.-.-");
        std::string disconnected = "Disconnected motors: ";
        for (int i = 0; i < disconnected_motors.size(); i++)
        {
            disconnected += motor_names[disconnected_motors[i]];
            if (i != disconnected_motors.size() - 1)
            {
                disconnected += ", ";
            }
        }
        master.set_text(0, 1, disconnected.c_str());
    }
    
}

void sensor_display(lv_obj_t *parent)
{
    lv_obj_clean(parent);

    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, "Sensors");

    // sensor categories, horizontal on top
    std::vector<lv_obj_t *> sensor_titles(11, lv_label_create(lv_scr_act()));
    std::vector<std::string> sensor_names = {"Hue", "Sat", "Bright", "Prox", "Heading", "BatCurr", "BatVolt", "BatCap", "BatTemp", "EncPos", "EncVel"};
    for (int i = 0; i < sensor_titles.size(); i++)
    {
        lv_label_set_text(sensor_titles[i], sensor_names[i].c_str());
        lv_obj_align(sensor_titles[i], LV_ALIGN_TOP_LEFT, 10 + 30 * i, 10);
    }
    // optical hue, sat, bright, proximity | imu heading | battery current, voltage, capacity, temp | encoder position, velocity
    std::vector<double> sensor_values = {optical.get_hue(), optical.get_saturation(), optical.get_brightness(),
                                         (double)optical.get_proximity(), imu.get_heading(), pros::battery::get_current() / 1000.0, 
                                         pros::battery::get_voltage() / 1000.0, pros::battery::get_capacity(), pros::battery::get_temperature(), 
                                         lF.get_position(), lF.get_actual_velocity()};

    for (int i = 0; i < sensor_titles.size(); i++)
    {
        lv_obj_t *sensor_value = lv_label_create(lv_scr_act());
        lv_label_set_text_fmt(sensor_value, "%f", sensor_values[i]);
        lv_obj_align(sensor_value, LV_ALIGN_TOP_LEFT, 10 + 30 * i, 40);
    }
}

lv_obj_t *tab_autos;
lv_obj_t *tab_stats;
lv_obj_t *tab_sensors;

void create_tab_view()
{

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

    lv_obj_t *title_auto = lv_label_create(lv_scr_act());
    std::string title = "Running auto " + std::to_string(selected_auto);
    lv_label_set_text(title_auto, title.c_str());
    lv_obj_align(title_auto, LV_ALIGN_CENTER, 0, 0);

    switch (selected_auto)
    {
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
    lv_obj_t *title_driver = lv_label_create(lv_scr_act());
    lv_label_set_text(title_driver, "Running driver control");
    lv_obj_align(title_driver, LV_ALIGN_CENTER, 0, 0);
    master.set_text(0, 0, "Driver control            ");

    bool clampExt = false;
    bool clampLatch = false;

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
        (clampExt) ? clamp.set_value(HIGH) : clamp.set_value(LOW);

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        {
            if (!clampLatch)
            {
                clampExt = !clampExt;
                clampLatch = true;
            }
        }
        else
        {
            clampLatch = false;
        }

        // move the chassis with arcade drive
        chassis.arcade(leftY, rightX);

        pros::delay(20);
    }
}

void disabled()
{
    create_tab_view();
}
