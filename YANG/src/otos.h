#pragma once
#include <cstdint>
#include <cstddef>

const size_t BUFFER_SIZE = 1024;

enum class Codes : uint8_t {
	KEEPALIVE = 0x00,
	RECALIBRATE = 0x01
};

typedef struct {
	double x;
	double y;
	double h;
} OtosData;

class OtosSensor {
    public:
        /**
         * @brief Run initial setup code to confiture PROS to recieve OTOS data. This should be run every time the program starts.
         * 
         */
        static void setup();
        /**
         * @brief Sends a message to the RPI to recalibrate the OTOS sensor. This will make the sensor unusable for a ~1s.
         * 
         */
        static void recalibrate();
        /**
         * @brief Returns the latest position as reported by the OTOS sensor.
         * 
         * @return OtosData A struct containing the x, y, and heading of the OTOS sensor
         */
        static OtosData get_latest_position();
};
