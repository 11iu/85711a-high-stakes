#include "main.h"
#include "autons.hpp"
#include "global.hpp"

// statistics for game
unsigned int pistonNum = 0; // extensions only (not extensions and retractions)
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

void switch_handler_auto(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        isBlue = lv_obj_has_state(obj, LV_STATE_CHECKED); // set blue when switch is flipped
        // std::cout << (isBlue) << std::endl;
    }
}

void auto_selector(lv_obj_t *parent)
{
    lv_obj_clean(parent);

    // red blue team color selector in the center top of the screen
    lv_obj_t *label_red = lv_label_create(parent);
    lv_label_set_text(label_red, "Red");
    lv_obj_set_pos(label_red, 150, 20);

    lv_obj_t *sw_team = lv_switch_create(parent);
    lv_obj_add_event_cb(sw_team, switch_handler_auto, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(sw_team, 195, 12);

    lv_obj_t *label_blue = lv_label_create(parent);
    lv_label_set_text(label_blue, "Blue");
    lv_obj_set_pos(label_blue, 260, 20);

    // auto selector buttons horizontally aligned at the bottom

    std::vector<lv_obj_t *> auto_buttons(6);
    std::vector<lv_obj_t *> auto_labels(6);
    std::vector<std::string> auto_names = {"Auto Far", "Auto Close", "Auto basic", "Auto skills", "Auto Far YY", "Auto awp YY"};

    // initialize buttons on two rows
    for (int i = 0; i < auto_buttons.size(); i++)
    {
        auto_buttons[i] = lv_btn_create(parent);
        lv_obj_add_event_cb(auto_buttons[i], btn_handler_auto, LV_EVENT_PRESSED, (void *)(i + 1));
        int x_val = 0;
        int y_val = 0;
        if (i >= auto_buttons.size() / 2)
        {
            x_val = 120 * (i - auto_buttons.size() / 2) + 30;
            y_val = 140;
        }
        else
        {
            x_val = 120 * i + 30;
            y_val = 80;
        }
        lv_obj_set_pos(auto_buttons[i], x_val, y_val);
        lv_obj_set_height(auto_buttons[i], LV_SIZE_CONTENT);
        auto_labels[i] = lv_label_create(auto_buttons[i]);
        lv_label_set_text(auto_labels[i], auto_names[i].c_str());
        lv_obj_center(auto_labels[i]);
    }

    // Select the first auto by default
    selected_btn_auto = auto_buttons[0];
    lv_obj_add_state(selected_btn_auto, LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(selected_btn_auto, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN | LV_STATE_CHECKED);
    selected_auto = 1;
}

void motor_display(lv_obj_t *parent)
{
    lv_obj_clean(parent);

    // motor name titles, horizontal on top
    std::vector<lv_obj_t *> motor_titles(7);
    for (lv_obj_t *&title : motor_titles)
    {
        title = lv_label_create(parent);
    }
    std::vector<std::string> motor_names = {"lF", "lM", "lB", "rF", "rM", "rB", "intake"};
    for (int i = 0; i < motor_titles.size(); i++)
    {
        lv_label_set_text(motor_titles[i], motor_names[i].c_str());
        lv_obj_set_pos(motor_titles[i], 50 * i + 80, 0);
    }

    // motor category titles, vertical on left
    std::vector<lv_obj_t *> stat_titles(5);
    for (lv_obj_t *&title : stat_titles)
    {
        title = lv_label_create(parent);
    }
    std::vector<std::string> stat_names = {"Temp", "Voltage", "Current", "Torque", "Ok"};
    for (int i = 0; i < stat_titles.size(); i++)
    {
        lv_label_set_text(stat_titles[i], stat_names[i].c_str());
        lv_obj_set_pos(stat_titles[i], 0, 30 * i + 30);
    }

    // motor temp labels in decidegree celcius, TODO: make it red if its too high or add a bar
    std::vector<int> motor_temps = {(int)(lF.get_temperature() * 10), (int)(lM.get_temperature() * 10), (int)(lB.get_temperature() * 10), (int)(rF.get_temperature() * 10), (int)(rM.get_temperature() * 10), (int)(rB.get_temperature() * 10), (int)(intake.get_temperature() * 10)};
    std::vector<lv_obj_t *> motor_temp_titles(7);
    for (lv_obj_t *&title : motor_temp_titles)
    {
        title = lv_label_create(parent);
    }
    for (int i = 0; i < motor_titles.size(); i++)
    {
        if (motor_temps[i] == INT_FAST32_MAX)
            motor_temps[i] = -1; // error value, disconnected

        lv_label_set_text_fmt(motor_temp_titles[i], "%d", motor_temps[i]);
        lv_obj_set_pos(motor_temp_titles[i], 50 * i + 80, 30);
    }

    // motor voltage in centivolt
    std::vector<int> motor_voltages = {(int)(lF.get_voltage() / 10.0), (int)(lM.get_voltage() / 10.0), (int)(lB.get_voltage() / 10.0), (int)(rF.get_voltage() / 10.0), (int)(rM.get_voltage() / 10.0), (int)(rB.get_voltage() / 10.0), (int)(intake.get_voltage() / 10.0)};
    std::vector<lv_obj_t *> motor_voltage_titles(7);
    for (lv_obj_t *&title : motor_voltage_titles)
    {
        title = lv_label_create(parent);
    }
    for (int i = 0; i < motor_titles.size(); i++)
    {
        if (motor_voltages[i] == INT_FAST32_MAX / 10)
            motor_voltages[i] = -1; // error value, disconnected

        lv_label_set_text_fmt(motor_voltage_titles[i], "%d", motor_voltages[i]);
        lv_obj_set_pos(motor_voltage_titles[i], 50 * i + 80, 60);
    }

    // motor current in centiamp
    std::vector<int> motor_currents = {(int)(lF.get_current_draw() / 10.0), (int)(lM.get_current_draw() / 10.0), (int)(lB.get_current_draw() / 10.0), (int)(rF.get_current_draw() / 10.0), (int)(rM.get_current_draw() / 10.0), (int)(rB.get_current_draw() / 10.0), (int)(intake.get_current_draw() / 10.0)};
    std::vector<lv_obj_t *> motor_current_titles(7);
    for (lv_obj_t *&title : motor_current_titles)
    {
        title = lv_label_create(parent);
    }
    for (int i = 0; i < motor_titles.size(); i++)
    {
        if (motor_currents[i] == INT_FAST32_MAX / 10)
            motor_currents[i] = -1; // error value, disconnected

        lv_label_set_text_fmt(motor_current_titles[i], "%d", motor_currents[i]);
        lv_obj_set_pos(motor_current_titles[i], 50 * i + 80, 90);
    }

    // motor torque
    std::vector<int> motor_torques = {(int)lF.get_torque(), (int)lM.get_torque(), (int)lB.get_torque(), (int)rF.get_torque(), (int)rM.get_torque(), (int)rB.get_torque(), (int)intake.get_torque()};
    std::vector<lv_obj_t *> motor_torque_titles(7);
    for (lv_obj_t *&title : motor_torque_titles)
    {
        title = lv_label_create(parent);
    }
    for (int i = 0; i < motor_titles.size(); i++)
    {
        if (motor_torques[i] == INT_FAST32_MAX)
            motor_torques[i] = -1; // error value, disconnected

        lv_label_set_text_fmt(motor_torque_titles[i], "%d", motor_torques[i]);
        lv_obj_set_pos(motor_torque_titles[i], 50 * i + 80, 120);
    }

    // motor connected
    std::vector<bool> motor_connected = {lF.is_installed(), lM.is_installed(), lB.is_installed(), rF.is_installed(), rM.is_installed(), rB.is_installed(), intake.is_installed()};
    std::vector<lv_obj_t *> motor_connected_titles(7);
    for (lv_obj_t *&title : motor_connected_titles)
    {
        title = lv_label_create(parent);
    }
    std::vector<int> disconnected_motors;
    for (int i = 0; i < motor_titles.size(); i++)
    {
        if (!motor_connected[i])
        {
            disconnected_motors.push_back(i);
        }
        if (motor_connected[i])
        {
            lv_label_set_text(motor_connected_titles[i], "Y");
        }
        else
        {
            lv_label_set_text(motor_connected_titles[i], "N");
        }
        lv_obj_set_pos(motor_connected_titles[i], 50 * i + 80, 150);
    }

    // // vibrate controller and show disconnected motors
    // if (disconnected_motors.size() > 0)
    // {
    //     master.rumble(".-.-.-");
    //     std::string disconnected = "Disconnected motors: ";
    //     for (int i = 0; i < disconnected_motors.size(); i++)
    //     {
    //         disconnected += motor_names[disconnected_motors[i]];
    //         if (i != disconnected_motors.size() - 1)
    //         {
    //             disconnected += ", ";
    //         }
    //     }
    //     master.set_text(0, 1, disconnected.c_str());
    // }
}

void sensor_display(lv_obj_t *parent)
{
    lv_obj_clean(parent);

    // Line 1: sensor categories, horizontal on top
    std::vector<lv_obj_t *> sensor_titles(11);
    for (lv_obj_t *&title : sensor_titles)
    {
        title = lv_label_create(parent);
    }
    std::vector<std::string> sensor_names = {"Hue", "Sat", "Bright", "Prox", "Heading", "BatCurr", "BatVolt", "BatCap", "BatTemp", "EncPos", "EncVel"};
    for (int i = 0; i < sensor_titles.size() / 2; i++)
    {
        lv_label_set_text(sensor_titles[i], sensor_names[i].c_str());
        lv_obj_set_pos(sensor_titles[i], 80 * i, 0);
    }

    // Line 2: sensor categories, horizontal on middle
    for (int i = sensor_titles.size() / 2; i < sensor_titles.size(); i++)
    {
        lv_label_set_text(sensor_titles[i], sensor_names[i].c_str());
        lv_obj_set_pos(sensor_titles[i], 80 * (i - sensor_titles.size() / 2), 90);
    }

    // optical hue, sat, bright, proximity | imu heading | battery current, voltage, capacity, temp | encoder position, velocity
    std::vector<int> sensor_values = {(int)optical.get_hue(), (int)optical.get_saturation(), (int)optical.get_brightness(),
                                      optical.get_proximity(), (int)0, (int)(pros::battery::get_current() / 10.0),
                                      (int)(pros::battery::get_voltage() / 10.0), (int)(pros::battery::get_capacity()), (int)(pros::battery::get_temperature() * 10.0),
                                      (int)lF.get_position(), (int)lF.get_actual_velocity()}; // imu get heading changed to zero

    for (int i = 0; i < sensor_titles.size() / 2; i++)
    {
        // FIXME: for battery current and voltage need to check if this is the max divided by 10
        if (sensor_values[i] == INT_FAST32_MAX)
            sensor_values[i] = -1; // error value, disconnected

        lv_obj_t *sensor_value = lv_label_create(parent);
        lv_label_set_text_fmt(sensor_value, "%d", sensor_values[i]);
        lv_obj_set_pos(sensor_value, 80 * i, 25);
    }

    for (int i = sensor_titles.size() / 2; i < sensor_titles.size(); i++)
    {
        if (sensor_values[i] == INT_FAST32_MAX)
            sensor_values[i] = -1; // error value, disconnected

        lv_obj_t *sensor_value = lv_label_create(parent);
        lv_label_set_text_fmt(sensor_value, "%d", sensor_values[i]);
        lv_obj_set_pos(sensor_value, 80 * (i - sensor_titles.size() / 2), 115);
    }
}

// display during a match, piston uses, run time, emergency motor disconects, overheat
void opcontrol_display()
{
    lv_obj_clean(lv_scr_act());
    lv_obj_t *title_driver = lv_label_create(lv_scr_act());
    lv_label_set_text(title_driver, "Running driver control");
    lv_obj_align(title_driver, LV_ALIGN_CENTER, 0, 0);
    master.set_text(0, 0, "Driver control            ");

    // piston activations
    lv_obj_t *piston_title = lv_label_create(lv_scr_act());
    lv_label_set_text(piston_title, "Piston activations");
}

lv_obj_t *tab_autos;
lv_obj_t *tab_motors;
lv_obj_t *tab_sensors;

// FIXME: need to make the variables reactive and have lvgl update, maybe create a thread for this
void create_tab_view()
{

    lv_obj_t *tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 30);

    tab_autos = lv_tabview_add_tab(tabview, "Autos");
    tab_motors = lv_tabview_add_tab(tabview, "Motors");
    tab_sensors = lv_tabview_add_tab(tabview, "Sensors");

    // auto tab content
    auto_selector(tab_autos);

    // stat tab content,  add real time data logging (temp of motors, rings scored, time elapsed, clamp amts)
    motor_display(tab_motors);

    // sensor tab content
    sensor_display(tab_sensors);
}

// image stuff

void lv_example_img_1(void)
{
    lv_obj_clean(lv_scr_act());
    LV_IMG_DECLARE(vex_field);
    lv_obj_t *img1 = lv_img_create(lv_scr_act());
    lv_img_set_src(img1, &vex_field);
    lv_obj_align(img1, LV_ALIGN_TOP_LEFT, 0, -20);
    lv_obj_set_size(img1, 400, 200);
}

/* --------------- LED SHIT ------------------- */
/*
void flashing_seizure(void *param)
{
    while (true)
    {
        leftLeds.set_all(0xFFFFFF);
        rightLeds.set_all(0xFFFFFF);
        pros::delay(80);
        leftLeds.clear_all();
        rightLeds.clear_all();
        pros::delay(80);
    }
}

void sequential_individual(void *param)
{
    uint32_t start = pros::millis();
    leftLeds.clear_all();
    rightLeds.clear_all();

    while (true)
    {
        uint32_t current = pros::millis();
        if (current - start > 60000)
        {
            leftLeds.clear_all();
            rightLeds.clear_all();
            start = current;
        }
        else
        {
            leftLeds[(current - start) / 1000] = 0x0000FF;
            rightLeds[(current - start) / 1000] = 0x0000FF;
            leftLeds.update();
            rightLeds.update();
        }
    }
}
*/

void initialize()
{
    pros::delay(500); // Stop the user from doing anything while
                      // legacy ports configure.
    pros::lcd::initialize();
    chassis.calibrate();

    // lvgl auto selector and info display on brain
    create_tab_view();
    // lv_example_img_1();

    // leftLeds.set_all(0x222222);
    // rightLeds.set_all(0x222222);
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
        master.set_text(0, 0, "Auto Far");
        auto_far(isBlue);
        break;
    case 2:
        master.set_text(0, 0, "Auto Close");
        auto_close(isBlue);
        break;
    case 3:
        master.set_text(0, 0, "Auto Basic");
        auto_basic();
        break;
    case 4:
        master.set_text(0, 0, "Auto Skills");
        auto_skills();
        break;
    case 5:
        master.set_text(0, 0, "Auto Far YY");
        auto_far_yedong();
        break;
    case 6:
        master.set_text(0, 0, "Auto AWP YY");
        solo_awp_yedong();
        break;
    }
}

// TODO: check if the rejection for red is ok
bool wrong_color()
{
    return (isBlue && optical.get_hue() < 100) || (!isBlue && optical.get_hue() > 30);
}

void opcontrol()
{

    bool clampExt = false;
    bool clampLatch = false;
    // bool rightDoinkerVal = false;
    // bool leftDoinkerVal = false;
    bool rightDoinkerExt = false;
    bool rightDoinkerLatch = false;
    bool leftDoinkerExt = false;
    bool leftDoinkerLatch = false;
    uint32_t sort_start_time = pros::millis();
    int continue_duration = 0; // continue belt after rejection start
    int stop_duration = 300;   // stop belt to reject after continue
    bool in_sorting = false;   // start rejection process
    int ladyBrownState = 1;
    while (true)
    {
        // opcontrol_display(); // FIXME: do not update this as often takes too long
        // drive
        int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        int leftY = log_drive(forward, 3);
        int rightX = log_drive(turn, 3);

        rightX *= 0.75; // slower turning

        // int ladyBrownVal = potentiometer.get_value();

        // std::cout << ladyBrownVal;

        // flip drive direction if l2 holded
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
        {
            leftY *= -1;
        }

        // clamp toggle for single acting piston
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

        // intake and belt hold R1
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {

            // // start the rejection process
            // if (wrong_color() && !in_sorting)
            // {
            //     sort_start_time = pros::millis();
            //     in_sorting = true;
            // }
            // else
            // {
            //     intake.move(127); // otherwise move normal
            // }

            // if (in_sorting && pros::millis() - sort_start_time < stop_duration)
            // {
            //     if (pros::millis() - sort_start_time < continue_duration)
            //         intake.move(127);
            //     else
            //         intake.move(0);
            // }
            // else
            // {
            //     in_sorting = false;
            // }
            intake.move(127);
        }
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) // reverse intake direction
        {
            intake.move(-127);
        }
        else
        {
            intake.move(0);
        }

        // move the chassis with arcade drive
        chassis.arcade(leftY, rightX);

        /*
        // right doinker RIGHT BTN
        (rightDoinkerExt) ? rightDoinker.set_value(HIGH) : rightDoinker.set_value(LOW);
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
        {
            if (!rightDoinkerLatch)
            {
                rightDoinkerExt = !rightDoinkerExt;
                rightDoinkerLatch = true;
            }
        }
        else
        {
            rightDoinkerLatch = false;
        }

        // left doinker LEFT BTN
        (leftDoinkerExt) ? leftDoinker.set_value(HIGH) : leftDoinker.set_value(LOW);
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))
        {
            if (!leftDoinkerLatch)
            {
                leftDoinkerExt = !leftDoinkerExt;
                leftDoinkerLatch = true;
            }
        }
        else
        {
            leftDoinkerLatch = false;
        }


        // ladybrown down button sets to lb_down, up button sets to lb_transfer, l2 sets to lb_score

        int default_state = 650;
        int state1 = 950;
        int state2 = 1900;
        int state3 = 2600;

        //y button

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)){
            intake.move(127);
            pros::delay(100);
            intake.move(-127);
            pros::delay(100);
            intake.move(127);
            pros::delay(100);
            intake.move(-127);
            pros::delay(100);
        }

        //ladybrown code

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && ladyBrownState == 1){
            while (ladyBrownVal < state1){
                    ladyBrown.move(-50);
                    pros::delay(10);
                    ladyBrownVal = potentiometer.get_value();
                    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
                        ladyBrownState = 1;
                        while (ladyBrownVal > default_state){
                            ladyBrownVal = potentiometer.get_value();
                            ladyBrown.move(50);
                            pros::delay(20);
                            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
                                ladyBrownState = 2;
                                while (ladyBrownVal > state2){
                                ladyBrown.move(100);
                                }
                            break;
                            }
                        }
                        ladyBrown.move(0);
                    }
                    else{
                        ladyBrownState = 2;
                        ladyBrown.move(-10);
                    }
                }

        }
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && ladyBrownState == 2){
            while (ladyBrownVal < state2){
                    ladyBrown.move(-70);
                    pros::delay(20);
                    ladyBrownVal = potentiometer.get_value();
                    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
                        ladyBrownState = 1;
                        while (ladyBrownVal > default_state){
                            ladyBrownVal = potentiometer.get_value();
                            ladyBrown.move(50);
                            pros::delay(20);
                        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
                            ladyBrownState = 2;
                            while (ladyBrownVal > state2){
                                ladyBrown.move(100);
                            }
                            break;
                        }
                        }
                        ladyBrown.move(0);
                    }
                    else{
                        ladyBrownState = 3;
                        ladyBrown.move(0);
                    }
                }

        }
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && ladyBrownState == 3){
            while (ladyBrownVal < state3){
                    ladyBrown.move(-127);
                    pros::delay(20);
                    ladyBrownVal = potentiometer.get_value();
                    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
                        ladyBrownState = 1;
                        while (ladyBrownVal > default_state){
                            ladyBrownVal = potentiometer.get_value();
                            ladyBrown.move(50);
                            pros::delay(20);
                        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
                            ladyBrownState = 2;
                            while (ladyBrownVal > state2){
                                ladyBrown.move(100);
                            }
                            break;
                        }
                        }
                        ladyBrown.move(0);
                    }
                    else{
                        ladyBrownState = 4;
                        ladyBrown.move(20); //20
                    }
                }

        }
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && ladyBrownState == 4){
            while (ladyBrownVal > default_state){
                    ladyBrown.move(50);
                    pros::delay(20);
                    ladyBrownVal = potentiometer.get_value();
                    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
                            ladyBrownState = 2;
                            while (ladyBrownVal > state3){//changed from stage 2 to stage 3 so that the motor doesnt force it into the intake and break it
                                ladyBrown.move(100);
                            }
                            break;
                    }
                    else{
                        ladyBrownState = 1;
                        ladyBrown.move(0);
                        }
            }

        }




        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
                        ladyBrownState = 1;
                        while (ladyBrownVal > 670){
                        ladyBrownVal = potentiometer.get_value();
                        ladyBrown.move(50);
                        pros::delay(20);
                        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
                            ladyBrownState = 2;
                            while (ladyBrownVal > state2){
                                ladyBrown.move(100);
                            }
                            break;
                        }
                        }
                    }






        /*if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
            if (ladyBrownState = 4){
                while (ladyBrownVal > 621){
                    ladyBrown.move(50);
                    pros::delay(20);
                    ladyBrownVal = potentiometer.get_value();
                    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
                        ladyBrownState = 1;
                        while (ladyBrownVal > 670){
                        ladyBrownVal = potentiometer.get_value();
                        ladyBrown.move(50);
                        pros::delay(20);
                        }
                    }else{ladyBrownState = 1;}
                }
                ladyBrown.move(0);

            }
        }

            if (ladyBrownState = 3){
                while (ladyBrownVal < 3270){
                    ladyBrown.move(-100);
                    pros::delay(20);
                    ladyBrownVal = potentiometer.get_value();
                    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
                        ladyBrownState = 1;
                        while (ladyBrownVal > 670){
                        ladyBrownVal = potentiometer.get_value();
                        ladyBrown.move(50);
                        pros::delay(20);
                        }
                    }else{ladyBrownState = 4;}
                }
                ladyBrown.move(0);

            }

            if (ladyBrownState = 2){
                while (ladyBrownVal < 2340){
                    ladyBrown.move(-100);
                    pros::delay(20);
                    ladyBrownVal = potentiometer.get_value();
                    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
                        ladyBrownState = 1;
                        while (ladyBrownVal > 670){
                        ladyBrownVal = potentiometer.get_value();
                        ladyBrown.move(50);
                        pros::delay(20);z
                        }
                    }else{ladyBrownState = 3;}
                }
                ladyBrown.move(0);

            }

            if (ladyBrownState = 1){
                while (ladyBrownVal < 1000){
                    ladyBrown.move(-50);
                    pros::delay(10);
                    ladyBrownVal = potentiometer.get_value();
                    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
                        ladyBrownState = 1;
                        while (ladyBrownVal > 670){
                            ladyBrownVal = potentiometer.get_value();
                            ladyBrown.move(50);
                            pros::delay(20);
                        }
                    }else{}ladyBrownState = 2;
                }
                ladyBrown.move(-10);

            }


        }

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
                ladyBrownState = 1;
                while (ladyBrownVal > 670){
                ladyBrownVal = potentiometer.get_value();
                ladyBrown.move(50);
                pros::delay(20);
            }
        }
        */

        // left doinker code (sam)
        /*if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)){
            if(leftDoinkerVal = true){
                leftDoinkerVal = false;
            }
            else {
                leftDoinkerVal = true;
            }
            if (leftDoinkerVal = true){
                leftDoinker.set_value(HIGH);
            }
            if (leftDoinkerVal = false){
                leftDoinker.set_value(LOW);
            }

        }*/

        pros::delay(20);
    }
}

void disabled()
{
    create_tab_view();

    // leftLeds.set_all(ledColors["blue"]);
    // rightLeds.set_all(ledColors["blue"]);
}
