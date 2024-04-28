#include "autons.hpp"
#include "display/lv_misc/lv_color.h"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"

///////////////////////////////////////////////////
// Chassis
///////////////////////////////////////////////////

pros::Controller master(pros::E_CONTROLLER_MASTER);

//  chassis motors
pros::Motor lF(LEFT_FRONT_LOWER_PORT, pros::E_MOTOR_GEARSET_06, true);
pros::Motor lM(LEFT_FRONT_UPPER_PORT, pros::E_MOTOR_GEARSET_06, false);
pros::Motor lB(LEFT_BACK_PORT, pros::E_MOTOR_GEARSET_06, true);
pros::Motor rF(RIGHT_FRONT_LOWER_PORT, pros::E_MOTOR_GEARSET_06, false);
pros::Motor rM(RIGHT_FRONT_UPPER_PORT, pros::E_MOTOR_GEARSET_06, true);
pros::Motor rB(RIGHT_BACK_PORT, pros::E_MOTOR_GEARSET_06, false);

pros::MotorGroup leftMotors({lF, lM,
							 lB}); // Creates a motor group with forwards ports
pros::MotorGroup rightMotors({rF, rM,
							  rB}); // Creates a motor group with forwards port

// Inertial Sensor
pros::Imu imu(IMU_PORT);

// drivetrain settings
lemlib::Drivetrain drivetrain(
	&leftMotors,				// left motor group
	&rightMotors,				// right motor group
	12,							// 12 inch track width (left to right wheels)
	lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
	360,						// drivetrain rpm is 360
	8							// chase power is 2. If we had traction wheels, it would have been 8
);

lemlib::ControllerSettings
	linearController(15,  // proportional gain (kP) 20 works
					 0,	  // integral gain (kI)
					 4,	  // derivative gain (kD) 4 works
					 3,	  // anti windup
					 1,	  // small error range, in inches
					 100, // small error range timeout, in milliseconds
					 3,	  // large error range, in inches
					 500, // large error range timeout, in milliseconds
					 40	  // maximum acceleration (slew)
	);

// angular motion controller
lemlib::ControllerSettings
	angularController(2,   // proportional gain (kP) 3 works
					  0,   // integral gain (kI)
					  10,  // derivative gain (kD) 15 works
					  3,   // anti windup
					  2,   // small error range, in degrees
					  100, // small error range timeout, in milliseconds
					  3,   // large error range, in degrees
					  500, // large error range timeout, in milliseconds
					  0	   // maximum acceleration (slew)
	);

// sensors for odometry
// note that in this example we use internal motor encoders (IMEs), so we don't
// pass vertical tracking wheels
lemlib::OdomSensors sensors(
	nullptr, // vertical tracking wheel 1, set to null
	nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
	nullptr, // horizontal tracking wheel 1
	nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a
			 // second one
	&imu	 // inertial sensor
);

lemlib::Chassis chassis(drivetrain, linearController, angularController,
						sensors);

pros::ADILed leds(LED_PORT, LED_LENGTH);

Autons autons(chassis);

struct Auto
{
	std::string name;
	std::function<void()> function;
	int color;
};

Auto autoDisabledAuton{"Disabled", std::bind(&Autons::autoDisabled, autons), 0x000000};

std::vector<Auto> autos = {autoDisabledAuton};
int currentAuto = 1;

///////////////////////////////////////////////////
// Utility Functions
///////////////////////////////////////////////////

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

void set_braking(bool brakeCoast = true)
{
	if (brakeCoast)
	{
		leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
		rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
	}
	else
	{
		leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
		rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
	}
}

void pgUp()
{
	currentAuto++;
	if (currentAuto > autos.size() - 1)
		currentAuto = 0;
	pros::lcd::print(0, "%s", autos[currentAuto].name);
	leds.set_all(autos[currentAuto].color);
}

void pgDown()
{
	currentAuto--;
	if (currentAuto < 0)
		currentAuto = autos.size() - 1;
	pros::lcd::print(0, "%s", autos[currentAuto].name);
	leds.set_all(autos[currentAuto].color);
}

/* LED SHIT */
void flashing_seizure(void *param)
{
	while (true)
	{
		leds.set_all(0xFFFFFF);
		pros::delay(80);
		leds.clear_all();
		pros::delay(80);
	}
}

void sequential_individual(void *param)
{
	uint32_t start = pros::millis();
	leds.clear_all();

	while (true)
	{
		uint32_t current = pros::millis();
		if (current - start > 60000)
		{
			leds.clear_all();
			start = current;
		}
		else
		{
			leds[(current - start) / 1000] = 0x0000FF;
			leds.update();
		}
	}
}

void bouncy(void *param)
{
	int start = 0;
	bool forwards = true;
	int size = 15;
	int last = -1;

	// initial lit up
	for (int i = 0; i < size; i++)
	{
		leds[i] = 0x0000FF;
		last++;
	}

	while (true)
	{

		if (forwards)
		{
			if (last == LED_LENGTH - 1)
			{
				forwards = false;
				last = LED_LENGTH - size;
			}
			else
			{
				leds[last + 1] = 0x0000FF;
				leds[last - size + 1] = 0x000000;
				leds.update();
				pros::delay(30);
				last++;
			}
		}
		else
		{
			if (last == 0)
			{
				forwards = true;
				last = size - 1;
			}
			else
			{
				leds[last - 1] = 0x0000FF;
				leds[last + size - 1] = 0x000000;
				leds.update();
				pros::delay(30);
				last--;
			}
		}
	}
}


///////////////////////////////////////////////////
// Main Functions
///////////////////////////////////////////////////

void initialize()
{
	pros::delay(500); // Stop the user from doing anything while
					  // legacy ports configure.
	pros::lcd::initialize();
	chassis.calibrate();
}

void competition_initialize()
{
	pros::ADIDigitalIn limit_left(LEFT_BUMP);
	pros::ADIDigitalIn limit_right(RiGHT_BUMP);
	pros::lcd::register_btn0_cb(pgDown);
	pros::lcd::register_btn2_cb(pgUp);
	pros::lcd::print(0, "%s", autos[currentAuto].name);
	leds.set_all(autos[currentAuto].color);

	while (true)
	{
		if (limit_left.get_value())
		{
			pgUp();
			pros::delay(500);
		}
		else if (limit_right.get_value())
		{
			pgDown();
			pros::delay(500);
		}
		pros::delay(20);
	}
}

void autonomous()
{
	leds.clear_all();

	pros::Task flash_task(bouncy);
	autos[currentAuto].function();
}

void opcontrol()
{

	leds.clear_all();

	while (true)
	{
		// drive
		int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		int leftY = logDrive(forward, 3) * FORWARD_AMT;
		int rightX = logDrive(turn, 3) * TURN_AMT;

		// move the chassis with arcade drive
		chassis.arcade(leftY, rightX);

		pros::delay(20);
	}
}