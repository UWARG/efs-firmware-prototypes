#ifndef LOS_D_UART_HPP
#define LOS_D_UART_HPP

#include <cstdint>
#include "usart.h"
#include "CircularBuffer.hpp"

class UARTDevice {
    public:
        static const size_t BUFFER_SIZE = 280;

        UARTDevice(UART_HandleTypeDef* huart);

        void init();
        void callback(uint16_t size);
        bool matchUART(UART_HandleTypeDef* huart);

        void transmit(uint8_t* buf, uint16_t size);
        uint16_t getCurDataSize();
        bool read(uint8_t* buf, uint16_t size);

    private:
        UART_HandleTypeDef* uart_handle;
        uint8_t buf[BUFFER_SIZE];
        CircularBuffer circular_buf;
        uint8_t it_recv_buf[BUFFER_SIZE];
};

#endif
