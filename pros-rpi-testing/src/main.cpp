#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <sstream>

#include "main.h"

#include "pros/llemu.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "pros/apix.h"

#include "cobs_decode.h"

typedef struct {
	double x;
	double y;
	double h;
} Data;

enum class Codes : uint8_t {
	KEEPALIVE = 0x00,
	RECALIBRATE = 0x01
};

const size_t BUFFER_SIZE = 1024;

/**
 * This variable is owned by the [read_serial_task], DO NOT MODIFY IT OUTSIDE OF THIS TASK
 */
Data latest_serial_communication({0., 0., 0.});

void read_serial_task() {
	uint8_t byteRead;
	uint8_t dataBuffer[BUFFER_SIZE];
	size_t dataBufferIndex = 0;

	while (true) {
		size_t out = fread(&byteRead, 1, 1, stdin);
		if (byteRead != '\0') {
			dataBuffer[dataBufferIndex] = byteRead;
			dataBufferIndex++;
		} else {
			size_t decodedLen = cobs_decode(dataBuffer, dataBufferIndex);
			if (decodedLen != sizeof(Data)) {
				std::cout
					<< "[WARNING] Incorrect size recieved after decoding COBS (expected "
					<< sizeof(Data)
					<< ", got "
					<< decodedLen
					<< ")"
					<< std::endl;
				std::stringstream ss;
				for (size_t i = 0; i < decodedLen; i++) {
					ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(dataBuffer[i]);
				}
				std::cout << ss.str() << std::endl;
			} else {
				std::memcpy(&latest_serial_communication, dataBuffer, decodedLen);
			}
			dataBufferIndex = 0; // Reset data buffer
		}
	}
}

void display_serial_task() {
	while (true) {
		pros::lcd::print(2, "X: %.6f", latest_serial_communication.x);
		pros::lcd::print(3, "Y: %.6f", latest_serial_communication.y);
		pros::lcd::print(4, "H: %.6f", latest_serial_communication.h);
		pros::delay(50);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
	pros::lcd::set_text(7, "              Recalibrate");

	pros::c::serctl(SERCTL_BLKWRITE, NULL);
	pros::c::serctl(SERCTL_ENABLE_COBS, NULL);

	pros::lcd::register_btn1_cb([] {
		Codes byte = Codes::RECALIBRATE;
		fwrite(&byte, sizeof(Codes), 1, stderr);
	});

	pros::Task::create(
		[] {
			Codes byte = Codes::KEEPALIVE;
			while (true) {
				fwrite(&byte, sizeof(Codes), 1, stderr);
				pros::delay(500);
			}
		},
		TASK_PRIORITY_DEFAULT,
		TASK_STACK_DEPTH_MIN,
		"Send keepalive task"
	);
	pros::Task::create(
		read_serial_task,
		TASK_PRIORITY_DEFAULT,
		TASK_STACK_DEPTH_DEFAULT,
		"Read serial task"
	);
	pros::Task::create(
		display_serial_task,
		TASK_PRIORITY_DEFAULT,
		TASK_STACK_DEPTH_DEFAULT,
		"Display serial task"
	);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	
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
 * task, not resume it from where it left off.
 */
void opcontrol() {
	
}