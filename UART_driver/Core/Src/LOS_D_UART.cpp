#include "LOS_D_UART.hpp"
#include "main.h"

UARTDevice::UARTDevice(UART_HandleTypeDef* huart)
    :
    uart_handle(huart), circular_buf(buf, BUFFER_SIZE) {}

void UARTDevice::init() {
    HAL_UARTEx_ReceiveToIdle_DMA(uart_handle, it_recv_buf, BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT(uart_handle->hdmarx, DMA_IT_HT);
}

void UARTDevice::callback(uint16_t data_size) {
    HAL_UARTEx_ReceiveToIdle_DMA(uart_handle, it_recv_buf, BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT(uart_handle->hdmarx, DMA_IT_HT);

    for (uint16_t i = 0; i < data_size; ++i) {
        circular_buf.write(it_recv_buf[i]);
    }
}

bool UARTDevice::matchUART(UART_HandleTypeDef* huart) {
    bool match = false;

    if(uart_handle != NULL && huart != NULL) {
        match = (uart_handle == huart);
    }

    return match;
}

void UARTDevice::transmit(uint8_t* buf, uint16_t size) {
    HAL_UART_Transmit(uart_handle, buf, size, TRANSMIT_TIMEOUT);
}

uint16_t UARTDevice::getAvailDataSize() {
    return circular_buf.getNumAvailBytes();
}

bool UARTDevice::read(uint8_t* buf, uint16_t size) {
    return circular_buf.read(buf, size);
}
