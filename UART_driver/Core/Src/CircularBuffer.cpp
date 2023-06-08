#include "CircularBuffer.hpp"
#include <cstring>

uint8_t CircularBuffer::peek(uint8_t& res, uint16_t dist) {
    if(dist + readPtr >= writePtr){
        return 0;
    }
    res = buf[(readPtr + dist) % size];
    return 1;
}

uint8_t CircularBuffer::read(uint8_t* res, uint16_t dist) {
    if(dist + readPtr > writePtr) return 0;

    if( (readPtr % size) + dist >= size ) {
        // two memcpys needed
        int dist_to_end = size - (readPtr % size);
        std::memcpy(res, &buf[readPtr % size], dist_to_end);
        readPtr += dist_to_end;
        dist -= dist_to_end;
        std::memcpy(&res[dist_to_end], &buf[readPtr % size], dist);
        readPtr += dist;
    } else {
        // one memcpy needed
        std::memcpy(res, &buf[readPtr % size], dist);
        readPtr += dist;
    }

    return 1;

}


CircularBuffer::CircularBuffer(uint8_t* buf, uint16_t size) {
    this->buf = buf;
    this->size = size;
}

CircularBuffer::CircularBuffer() {
    this->buf = NULL;
    this->size = 0;
}

bool CircularBuffer::hasSpace() {
    return !((writePtr % size == readPtr % size) && writePtr != readPtr);
}

uint16_t CircularBuffer::getDataSize() {
    return writePtr - readPtr;
}

uint8_t CircularBuffer::write(uint8_t data) {
    buf[writePtr % size] = data;
    this->writePtr++;
    return 1;
}
