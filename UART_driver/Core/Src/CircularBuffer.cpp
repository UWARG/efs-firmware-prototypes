#include "CircularBuffer.hpp"
#include <cstring>

CircularBuffer::CircularBuffer(uint8_t* buf, uint16_t size) {
    this->buf = buf;
    this->size = size;
}

bool CircularBuffer::peek(uint8_t& byte, uint16_t index) {
    bool success = true;

    if (readPtr + index >= writePtr) {
        success = false;
    } else {
        byte = buf[(readPtr + index) % size];
    }

    return success;
}

bool CircularBuffer::read(uint8_t* res, uint16_t dist) {
    bool success = true;

    if (dist + readPtr > writePtr) {
        success = false;
    } else {
        if ((readPtr % size) + dist >= size) {
            // Two memcpys needed because we need to loop back to the
            // beginning of the buffer.
            uint16_t dist_to_end = size - (readPtr % size);
            std::memcpy(res, &buf[readPtr % size], dist_to_end);
            readPtr += dist_to_end;
            dist -= dist_to_end;

            std::memcpy(&res[dist_to_end], &buf[readPtr % size], dist);
            readPtr += dist;
        } else {
            // One memcpy needed.
            std::memcpy(res, &buf[readPtr % size], dist);
            readPtr += dist;
        }
    }

    return success;
}

bool CircularBuffer::isFull() {
    // The buffer is full when the writePtr wraps around to the
    // readPtr, so it's ahead of the readPtr by value, but overlaps
    // the readPtr by index aftering modding by the buffer size.
    return (writePtr != readPtr) &&
           (writePtr % size == readPtr % size);
}

uint16_t CircularBuffer::getNumAvailBytes() {
    return writePtr - readPtr;
}

bool CircularBuffer::write(uint8_t byte) {
    bool success = true;

    if (isFull()) {
        success = false;
    } else {
        buf[writePtr % size] = byte;
        writePtr++;
    }

    return success;
}
