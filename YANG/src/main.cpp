#include <ctime>

#include "main.h"
#include "arm.h"
#include "conveyor.h"
#include "drive.h"
#include "goal_grabber.h"
#include "intake.h"
#include "optical.h"
#include "otos.h"
#include "top_arm.h"
// #include "auto_chooser.h"
#include "autos.h"

#include "replay.hpp"

// Enable the following to load skills auto instead of competition auto
#define SKILLS true

#define FILENAME "replay_.txt"
#define RECORD false
// #define RECORD_TIME 15000 // 30 seconds
#define RECORD_TIME 30000 // 1 min
#define AUTO_TIME_LIMIT true // TURN OFF FOR COMPETITION PLEASE PLEASE

bool driver = true;
bool recording = RECORD;
std::vector<ReplayStep> replay(0);

static bool is_blue = false;
pros::Controller controllerMain(pros::E_CONTROLLER_MASTER);

/**
 * A callback function for LLEMU's center_btn button.
 */
void on_center_button() {

}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	// pros::lcd::set_text(1, "Is this working");
	Drive::resetHeading();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	Drive::setDriveVelocity(0);
	Intake::brake();
	Conveyor::brake();
	Optical::setLED(false);
	GoalGrabber::setNotGrabbing();
	TopArm::tarePosition();
	// GoalGrabber::setNotGrabbing();

	if(recording && replay.size() > 0) {
		write_replay(replay, FILENAME);
	}
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	// Calibration
	Drive::resetHeading();
	// OtosSensor::setup();

	// OtosSensor::recalibrate();
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left_btn off.
 */
void autonomous() {
	if (AUTO_TIME_LIMIT && !SKILLS) {
		auto main_task = pros::Task::current();
		uint32_t start = pros::c::millis();
		pros::Task::create(
			[&, start]{
				while (true) {
					if (pros::c::millis() - start > 30 /* sec */ * 1000 /* ms */) {
						main_task.suspend();

						Drive::brake();
						Intake::brake();
						Conveyor::brake();
						Optical::setLED(false);
						GoalGrabber::setNotGrabbing();

						break;
					}

					pros::delay(50);
				}
			},
			TASK_PRIORITY_DEFAULT,
			TASK_STACK_DEPTH_DEFAULT,
			"stop after 30s"
		);
	}

	// testCornerSort(true);
	// testDriveWithSort(true);
	// skillsOneYang();
	// fullAutoTwoYang(true);
	if (SKILLS) {
		worldsSkillsYang();
	} else {
		worldsAutoYang(is_blue);
	}
	// AutoChooser::runSelected();

	Drive::brake();
	Intake::setNotMoving();
	Conveyor::setNotMoving();
	return;

	// bool y = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y);
	// if(!y) {
	// 	pros::lcd::set_text(2, "Waiting for press...");
	// 	pros::lcd::clear_line(3);
	// 	pros::lcd::clear_line(4);
	// 	pros::delay(2);
	// 	return;
	// }
	// pros::lcd::clear_line(1);

	// // std::vector<ReplayStep> replay = read_replay(FILENAME);
	// pros::lcd::set_text(2, "Loaded replay!");
	// pros::delay(1000);
	// pros::lcd::set_text(3, "Running replay!");
	// for(ReplayStep replay_step : replay) {
	// 	Drive::driveDirect(replay_step.leftWheels, replay_step.rightWheels);
	// 	Intake::direct(replay_step.intake);
	// 	Conveyor::direct(replay_step.conveyor);
	// 	Arm::direct(replay_step.arm);
	// 	GoalGrabber::direct(replay_step.grabber);

	// 	pros::delay(2);
	// }
	// Drive::brake();
	// Intake::setNotMoving();
	// Conveyor::setNotMoving();
	// pros::lcd::set_text(4, "Finished replay!");
	// pros::delay(1000);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left_btn off.
 *
 * ########## Control Mappings ##########
 *
 * Left analog Y        - Left motors control
 * Right analog Y       - Right motors control
 *
 * Left top bumper      - Intake
 * Left bottom bumper   - Outtake
 * Right top bumper     - Goal grabbing toggle
 *
 * B (right_btn 3d printed) - Hold precision mode
 */
void opcontrol() {
	// pros::delay(2000);
	// fullAutoOneYang(false);

	// return;
	Drive::setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

	Optical::setTeamColor(is_blue);
	Optical::enable();

	TopArm::tarePosition();
	controllerMain.print(0, 0, "Color: %s", is_blue ? "blue" : "red");
	controllerMain.print(1, 0, "Sort: enabled ");

	while (driver) {
		// double left_x = controllerMain.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
		double left_y = controllerMain.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y); // tank left
		// double right_x = controllerMain.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		double right_y = controllerMain.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y); // tank right
		bool l1 = controllerMain.get_digital(pros::E_CONTROLLER_DIGITAL_L1); // intake with conveyor
		bool l2 = controllerMain.get_digital(pros::E_CONTROLLER_DIGITAL_L2); // intake only
		bool r1 = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1); // goal grabber
		bool r2 = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2); // toggle top arm
		bool x = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X); // return top arm to start
		bool a = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A);
		bool b = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B); // precision button
		bool up_arrow = controllerMain.get_digital(pros::E_CONTROLLER_DIGITAL_UP); // reverse intake
		bool down_arrow = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN); // corner arm
		bool left_arrow = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT); // save replay

		// Drive::controlDirection(a);
		Drive::controlTank(left_y, right_y, false);
		// Drive::controlArcade(right_y, left_x, b);
		Intake::toggleColorSort(x, controllerMain);
		Intake::control(l2, l1, up_arrow, Optical::stoppedDetectingOpposite());
		// Intake::control(l2, l1, up_arrow, false);

		GoalGrabber::control(r1);
		// Conveyor::control(l1, up_arrow);
		Arm::control(down_arrow);
		TopArm::control(b, r2);

		int replay_step = 0;
		if(recording) {
			ReplayStep current_step;
			current_step.leftWheels[0] = Drive::left.get_actual_velocity(0);
			current_step.leftWheels[1] = Drive::left.get_actual_velocity(1);
			current_step.leftWheels[2] = Drive::left.get_actual_velocity(2);
			current_step.leftWheels[3] = Drive::left.get_actual_velocity(3);
			current_step.rightWheels[0] = Drive::right.get_actual_velocity(0);
			current_step.rightWheels[1] = Drive::right.get_actual_velocity(1);
			current_step.rightWheels[2] = Drive::right.get_actual_velocity(2);
			current_step.rightWheels[3] = Drive::right.get_actual_velocity(3);
			current_step.intake = Intake::intakeMotor.get_actual_velocity();
			current_step.conveyor = Conveyor::conveyorMotor.get_actual_velocity();
			current_step.arm = Arm::armValue;
			current_step.grabber = GoalGrabber::grabValue;
			replay.push_back(current_step);

			if(replay_step >= RECORD_TIME || left_arrow) {
				// driver = false;
				recording = false;
				write_replay(replay, FILENAME);
				pros::lcd::set_text(1, "Saved replay!");
			}
			replay_step++;
		}
		pros::delay(2);
	}
}
