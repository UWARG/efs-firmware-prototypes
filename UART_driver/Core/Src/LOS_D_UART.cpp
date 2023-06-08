#include "LOS_D_UART.hpp"
#include "main.h"

UARTDevice::UARTDevice(UART_HandleTypeDef* huart)
    :
    uart_handle(huart), circular_buf(buf, BUFFER_SIZE) {}

void UARTDevice::startInterrupt() {
    HAL_UARTEx_ReceiveToIdle_DMA(uart_handle, buf, BUFFER_SIZE);
	__HAL_DMA_DISABLE_IT(uart_handle->hdmarx, DMA_IT_HT);
}

void UARTDevice::callback(uint16_t data_size) {
    HAL_UARTEx_ReceiveToIdle_DMA(uart_handle, it_recv_buf, BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT(uart_handle->hdmarx, DMA_IT_HT);

    for (int i = 0; i < data_size; ++i) {
        circular_buf.write(it_recv_buf[i]);
    }
}

bool UARTDevice::matchUART(UART_HandleTypeDef* huart) {
    if(uart_handle != NULL && huart != NULL) {
        return uart_handle == huart;
    }

    return false;
}

void UARTDevice::transmit(uint8_t* buf, uint16_t size) {
    HAL_UART_Transmit_DMA(uart_handle, buf, size);
}

uint16_t UARTDevice::getCurDataSize() {
    return circular_buf.getDataSize();
}

bool UARTDevice::read(uint8_t* buf, uint16_t size) {
    return circular_buf.read(buf, size);
}
