#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <stdint.h>

class CircularBuffer {
    public:
        // Constructor. `buf` is the buffer to be used as a circular buffer
        CircularBuffer(uint8_t* buf, uint16_t size);

        // Peek at a byte in the buffer
        uint8_t peek(uint8_t& res, uint16_t dist);

        // Read bytes from the buffer
        uint8_t read(uint8_t* buf, uint16_t dist);

        // Write byte to the buffer
        uint8_t write(uint8_t byte);

        // Returns true if the buffer has space
        bool hasSpace();

        // Returns the size of the current data available
        uint16_t getDataSize();

    private:
        uint8_t* buf;
        uint16_t size;
        uint16_t writePtr = 0;
        uint16_t readPtr = 0;
};


#endif
