#pragma once
#include <cstddef>
#include <cstdint>

/**
 * @brief Decodes a buffer with cobs-encoded bytes in-place
 *
 * @note This has not been tested on strings longer than a few bytes, so try testing it on longer strings if it fails! This also does not check for errors, so the only sign of an encoding error is unexpected decoded size.
 * 
 * @param buffer The buffer to decode
 * @param bufferSize The length of data in the buffer to decode
 * @return size_t The length of the decoded data
 */
size_t cobs_decode(uint8_t* buffer, size_t bufferSize);
