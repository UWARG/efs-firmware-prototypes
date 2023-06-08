#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <stdint.h>

class CircularBuffer {
    public:

    uint8_t peek(uint8_t& res, uint16_t dist);
    uint8_t read(uint8_t* buf, uint16_t size);

    uint8_t write(uint8_t data);

    bool hasSpace();
    uint16_t getDataSize();

    CircularBuffer(uint8_t* buf, uint16_t size);
    CircularBuffer();

    private:

    uint8_t* buf;
    uint16_t size;

    uint16_t writePtr = 0;
    uint16_t readPtr = 0;
};


#endif
