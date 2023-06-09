#ifndef LOS_D_UART_HPP
#define LOS_D_UART_HPP

#include <cstdint>
#include "usart.h"
#include "CircularBuffer.hpp"

class UARTDevice {
    public:
        static const size_t BUFFER_SIZE = 280;

        // Constructor. Assigns a uart handle to the driver.
        UARTDevice(UART_HandleTypeDef* huart);

        // Initialization: enable DMA interrupt.
        void init();

        // Callback to write DMA data to internal circular buffer.
        void callback(uint16_t size);

        // Check if interrupt uart handle matches with the driver's uart handle.
        bool matchUART(UART_HandleTypeDef* huart);

        // Transmit data via DMA.
        void transmit(uint8_t* buf, uint16_t size);

        // Get the size of the current available data in the internal circular buffer.
        uint16_t getCurDataSize();

        // Read data from the internal circular buffer.
        bool read(uint8_t* buf, uint16_t size);

    private:
        UART_HandleTypeDef* uart_handle;
        uint8_t buf[BUFFER_SIZE];
        CircularBuffer circular_buf;
        uint8_t it_recv_buf[BUFFER_SIZE];
};

#endif
