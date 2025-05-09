#include "otos.h"
#include "cobs_decode.h"
#include "pros/apix.h"
#include <cstring>

/**
 * @brief This variable is owned by the read serial task, DO NOT MODIFY IT OUTSIDE OF THIS TASK
 */
OtosData latest_otos_data({0., 0., 0.});

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
			if (decodedLen != sizeof(OtosData)) {
				std::cout
					<< "[WARNING] Incorrect size recieved after decoding COBS (expected "
					<< sizeof(OtosData)
					<< ", got "
					<< decodedLen
					<< ")"
					<< std::endl;
			} else {
				std::memcpy(&latest_otos_data, dataBuffer, decodedLen);
			}
			dataBufferIndex = 0; // Reset data buffer
		}

		pros::delay(2);
	}
}

Codes recalibrate_byte = Codes::RECALIBRATE;

void OtosSensor::setup() {
    // Enable blocking writes (to avoid the taks unnecessarily hogging the CPU)
    pros::c::serctl(SERCTL_BLKWRITE, NULL);
    // Enable COBS to ensure packet framing between PROS and the RPI
	pros::c::serctl(SERCTL_ENABLE_COBS, NULL);

    // Start a new task for reading serial
    pros::Task::create(
		read_serial_task,
		TASK_PRIORITY_DEFAULT,
		TASK_STACK_DEPTH_DEFAULT,
		"Read serial task"
	);

	// Start a new task for printing serial
 //    pros::Task::create(
	// 	[&]{
	// 		while (true) {
	// 			auto pos = get_latest_position();
	// 			pros::lcd::print(1, "%f.2 %f.2 %f.2", pos.x, pos.y, pos.h);

	// 			pros::delay(100);
	// 		}
	// 	},
	// 	TASK_PRIORITY_DEFAULT,
	// 	TASK_STACK_DEPTH_DEFAULT,
	// 	"print serial task"
	// );
}

void OtosSensor::recalibrate() {
    fwrite(&recalibrate_byte, sizeof(Codes), 1, stderr);
}

OtosData OtosSensor::get_latest_position() {
    return latest_otos_data;
}
