// #include <chrono>
#include <ctime>
#include <vector>
#include <string>
#include <format>

#include "main.h"
#include "constants.h"

#include "arm.h"
#include "conveyor.h"
#include "drive.h"
#include "intake.h"
#include "goal_grabber.h"
#include "top_arm.h"
// #include "auto_chooser.h"
#include "autos.h"
#include "optical.h"

#include "replay.hpp"

#define RECORD_NAME "replay_skills_7.txt"
#define PLAY_NAME "replay_skills_7.txt"

#define RECORD false
// #define RECORD_TIME 15000 // 30 sec
#define RECORD_TIME 30000 // 1 min

bool is_blue = true;
bool recording = RECORD;
std::vector<ReplayStep> replay(0);

pros::Controller controllerMain(pros::E_CONTROLLER_MASTER);

// void setColor(bool blue) {
// 	if(blue) {

// 	} else {

// 	}
// }

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
	// setColor(isBlue);
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
	TopArm::tarePosition();

	if(recording && replay.size() > 0) {
		write_replay(replay, RECORD_NAME);
	}

	// Competition initialize
	// auto left_btn = 0;
	// auto center_btn = 0;
	// auto right_btn = 0;

	// pros::lcd::print(0, "Choose the auto!");
	// pros::lcd::print(1, "Current Auto:");
	// pros::lcd::print(3, "Current Color:");
	// while (true) {
	// 	auto lcd_buttons = pros::lcd::read_buttons();

	// 	auto old_left_btn = left_btn;
	// 	auto old_center_btn = center_btn;
	// 	auto old_right_btn = right_btn;

	// 	left_btn = lcd_buttons & LCD_BTN_LEFT;
	// 	center_btn = lcd_buttons & LCD_BTN_CENTER;
	// 	right_btn = lcd_buttons & LCD_BTN_RIGHT;

	// 	if (left_btn != old_left_btn && left_btn) {
	// 		pros::lcd::print(5, "left_btn pressed");
	// 		AutoChooser::selectPrev();
	// 	} else if (center_btn != old_center_btn && center_btn) {
	// 		pros::lcd::print(5, "center_btn pressed");
	// 		AutoChooser::toggleColor();
	// 	} else if (right_btn != old_right_btn && right_btn) {
	// 		pros::lcd::print(5, "right_btn pressed");
	// 		AutoChooser::selectNext();
	// 	}
	// 	pros::lcd::print(2, "%s", AutoChooser::getName());
	// 	pros::lcd::print(4, "%s", AutoChooser::isBlue() ? "blue" : "red");

	// 	pros::delay(25);
	// }
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
	testDriveWithSort(true);
	// fullAutoOneYin(is_blue);
	// Intake::setIntaking();
	// pros::delay(1000000);
	yinRush(is_blue); // REAL CODE
	// pros::delay(10000);
	// AutoChooser::runSelected();
	// driveForward(48);
	// driveForward(-48);
	// rotate(720);
	// driveCircle(12);
	// driveDistanceGyro(24);
	// rotateTest();
	// testBasicFeedbackDrive();
	// rushWithArm();
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

	std::vector<ReplayStep> replay = read_replay(PLAY_NAME);
	pros::lcd::set_text(2, "Loaded replay!");
	pros::delay(1000);
	pros::lcd::set_text(3, "Running replay!");
	for(ReplayStep replay_step : replay) {
		Drive::driveDirect(replay_step.leftWheels, replay_step.rightWheels);
		Intake::direct(replay_step.intake);
		Conveyor::direct(replay_step.conveyor);
		Arm::direct(replay_step.arm);
		GoalGrabber::direct(replay_step.grabber);

		pros::delay(2);
	}
	Drive::brake();
	Intake::setNotMoving();
	Conveyor::setNotMoving();
	pros::lcd::set_text(4, "Finished replay!");
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
	// autonomous();
	// pros::delay(2000);
	Drive::setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
	Optical::setTeamColor(is_blue);
	Optical::setLED(true);
	TopArm::approachMogo();

	bool last_detected = false;
	int replay_step = 0;
	while (true) {
		double left_x = controllerMain.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X); // turn for arcade
		// double left_y = controllerMain.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y); // left tank
		// double right_x = controllerMain.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // not used
		double right_y = controllerMain.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y); // arcade forward and right tank
		bool l1 = controllerMain.get_digital(pros::E_CONTROLLER_DIGITAL_L1); // control conveyor and intake
		bool l2 = controllerMain.get_digital(pros::E_CONTROLLER_DIGITAL_L2); // reverse conveyor and intake
		bool r1 = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1); // goal grabber
		bool r2 = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2); // toggle between load and high stake position
		bool a = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A); // toggle color sort
		bool b = controllerMain.get_digital(pros::E_CONTROLLER_DIGITAL_B); // precision mode
		bool x = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X); // go to mogo position
		bool y = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y); // spin throw button
		bool up_arrow = controllerMain.get_digital(pros::E_CONTROLLER_DIGITAL_UP); // intake only button
		bool down_arrow = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN); // corner arm
		bool left_arrow = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT); // save replay
		// Drive::controlDirection(a);
		// Drive::controlTank(left_y, right_y, b);
		Drive::controlArcade(right_y, left_x, b);

		Intake::toggleColorSort(a);
		Intake::control(up_arrow, l1, l2, a, Optical::oppositeColorDetected());
		// Intake::control(l2, l1, up_arrow, Optical::getColor() == Color::BLUE);

		GoalGrabber::control(r1);
		Arm::control(down_arrow);
		TopArm::control(x, r2);

		if(y) {
			Drive::driveArc(0, 1.5, 127);
			GoalGrabber::setNotGrabbing();
		}

		if(recording) {
			ReplayStep current_step;
			current_step.leftWheels[0] = Drive::left.get_actual_velocity(0);
			current_step.leftWheels[1] = Drive::left.get_actual_velocity(1);
			current_step.leftWheels[2] = Drive::left.get_actual_velocity(2);
			current_step.leftWheels[3] = Drive::left.get_actual_velocity(3);
			current_step.leftWheels[4] = Drive::left.get_actual_velocity(4);
			current_step.rightWheels[0] = Drive::right.get_actual_velocity(0);
			current_step.rightWheels[1] = Drive::right.get_actual_velocity(1);
			current_step.rightWheels[2] = Drive::right.get_actual_velocity(2);
			current_step.rightWheels[3] = Drive::right.get_actual_velocity(3);
			current_step.rightWheels[4] = Drive::right.get_actual_velocity(4);
			current_step.intake = Intake::intakeMotor.get_actual_velocity();
			current_step.conveyor = Conveyor::conveyorMotors.get_actual_velocity();
			current_step.arm = Arm::armValue;
			current_step.grabber = GoalGrabber::grabValue;
			replay.push_back(current_step);

			// if(replay_step >= RECORD_TIME || left_arrow) {
			if(left_arrow) {
				recording = false;
				write_replay(replay, RECORD_NAME);
				pros::lcd::set_text(1, "Saved replay!");
				// autonomous();
			}
			replay_step++;
		}
		pros::delay(2);
	}
}
