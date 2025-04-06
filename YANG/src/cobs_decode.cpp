#include "cobs_decode.h"

#include <cstddef>
#include <cstdint>

size_t cobs_decode(uint8_t* buffer, size_t bufferSize) {
    // First byte of COBS-encoding is always a pointer to the next null
    uint8_t nextNull = buffer[0];
    bool nextIsOverhead = nextNull == 0xFF; // If the first byte is max, then next "null location" shouldn't be written as a null byte
    // Keep track of how much we have written to the buffer
    size_t nextWriteIndex = 0;
    // Loop over each char
    for (size_t charIndex = 1; charIndex < bufferSize; charIndex++) {
        nextNull--; // We just read a byte
        if (nextNull == 0 && nextIsOverhead) {
            // We reached the next pointer, but it shouldn't be encoded as a null byte
            nextNull = buffer[charIndex];
            nextIsOverhead = nextNull == 0xFF;
            continue;
        } else if (nextNull == 0) {
            // We reached the next pointer, and it isn't overhead so we must encode a null byte
            buffer[nextWriteIndex] = '\0';
            nextWriteIndex++;
            // Handle reseting the next null pointer
            nextNull = buffer[charIndex];
            nextIsOverhead = nextNull == 0xFF;
            continue;
        } else {
            // This is not a null byte or overhead, so we should encode as usual
            buffer[nextWriteIndex] = buffer[charIndex];
            nextWriteIndex++;
        }
    }
    // Return the size we have written
    return nextWriteIndex;
}
