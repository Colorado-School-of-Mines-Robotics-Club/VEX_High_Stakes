#include "main.h"
#include "constants.h"
#include "drive.h"
#include "intake.h"
#include "conveyor.h"
#include "arm.h"
#include "goal_grabber.h"
// #include "auto_chooser.h"
#include "autos.h"

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
	// pros::lcd::initialize();
	// pros::lcd::set_text(1, "Is this working");
	Drive::resetHeading();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
// void disabled() {
// 	Drive::setDriveVelocity(0);
// 	Intake::brake();
// 	Conveyor::brake();
// 	// GoalGrabber::setNotGrabbing();

// 	// Competition initialize
// 	auto left_btn = 0;
// 	auto center_btn = 0;
// 	auto right_btn = 0;
	
// 	pros::lcd::print(0, "Choose the auto!");
// 	pros::lcd::print(1, "Current Auto:");
// 	pros::lcd::print(3, "Current Color:");
// 	while (true) {
// 		auto lcd_buttons = pros::lcd::read_buttons();

// 		auto old_left_btn = left_btn;
// 		auto old_center_btn = center_btn;
// 		auto old_right_btn = right_btn;

// 		left_btn = lcd_buttons & LCD_BTN_LEFT;
// 		center_btn = lcd_buttons & LCD_BTN_CENTER;
// 		right_btn = lcd_buttons & LCD_BTN_RIGHT;

// 		if (left_btn != old_left_btn && left_btn) {
// 			pros::lcd::print(5, "left_btn pressed");
// 			AutoChooser::selectPrev();
// 		} else if (center_btn != old_center_btn && center_btn) {
// 			pros::lcd::print(5, "center_btn pressed");
// 			AutoChooser::toggleColor();
// 		} else if (right_btn != old_right_btn && right_btn) {
// 			pros::lcd::print(5, "right_btn pressed");
// 			AutoChooser::selectNext();
// 		}
// 		pros::lcd::print(2, "%s", AutoChooser::getName());
// 		pros::lcd::print(4, "%s", AutoChooser::isBlue() ? "blue" : "red");

// 		pros::delay(25);
// 	}
// }

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
	// AutoChooser::runSelected();
	// testBasicFeedbackDrive();
	// fullAutoOneYin(false);
	fullAutoOneYang(true);
	// rushWithArm();
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
	Drive::setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

	while (true) {
		double left_x = controllerMain.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
		double left_y = controllerMain.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		double right_x = controllerMain.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		double right_y = controllerMain.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
		bool l1 = controllerMain.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
		bool l2 = controllerMain.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
		bool r1 = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1);
		bool r2 = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2);
		bool a = controllerMain.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A);
		bool b = controllerMain.get_digital(pros::E_CONTROLLER_DIGITAL_B);

		// Drive::controlDirection(a);
		// Drive::controlTank(left_y, right_y, b);
		Drive::controlArcade(right_y, left_x, b);
		Intake::control(l1, l2);
		GoalGrabber::control(r1);
		// Conveyor::control();
		Arm::control(r2);
	}
}
