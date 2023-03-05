/*
 * vectorNav.cpp
 *
 *  Created on: Jun 23, 2022
 *      Author: Christopher Chung
 */

#include <vectorNav.hpp>

const uint8_t BITS_PER_BYTE = 8;

const char* GPS_HEADER = "$VNGPS\r\n";
const char* IMU_HEADER = "$VNIMU\r\n";

extern UART_HandleTypeDef huart2;

const char ANT_A_OFFSET_COMMAND[] = "$VNWRG,57,0.1,0,0*XX\r\n";                     //ASCII command to set the Antenna A Offset {0.1,0,0}
const char ANT_B_OFFSET_COMMAND[] = "$VNWRG,93,1.5,0,0,0.038,0.038,0.038*XX\r\n";   //ACSII command to set the Antenna A Offset {0.2,0,0} and uncertainly {0.0254, 0.0254, 0.0254}

const char MSG_CONFIG_COMMAND[] = "$VNWRG,06,0*XX\r\n$VNWRG,75,0,0,01,1C9*XX\r\n"; //ASCII command to configure custom binary data group 1
const char POLL_DATA_COMMAND[] = "$VNBOM,1*XX\r\n";              //ASCII command to poll VN300 and request custom binary data group 1

const uint16_t PAYLOAD_SIZE = 74;                            //Size of custom message payload coming from VN300
uint8_t payload_data[PAYLOAD_SIZE] = {0};                    //RX buffer to hold incoming data

/* Public Methods  ---------------------------------------------------------*/

Position& VN300::getInstance(){
    static VN300 singleton;
    return singleton;
}

void VN300::GetResult(PositionData_t& data){
    //set all values in the struct to 0?

    sendCommand(POLL_DATA_COMMAND, sizeof(POLL_DATA_COMMAND));
    recieveResponse();

    if (data.req_data.gps)
        parse_gps_data(data.gps_data);

    if (data.req_data.imu)
        parse_imu_data(data.imu_data);
}

/* Private methods  ---------------------------------------------------------*/
VN300::VN300(){
    VN300Init();
}

void VN300::VN300Init(void){
    //GNSS Antenna A Offset
    sendCommand(ANT_A_OFFSET_COMMAND, sizeof(ANT_A_OFFSET_COMMAND));

    //GNSS Antenna B Offset
    sendCommand(ANT_B_OFFSET_COMMAND, sizeof(ANT_B_OFFSET_COMMAND));

    /*
        "To ensure that the output is consistent with the attitude of the vehicle the sensor is attached to you will
        need to align the sensor on the vehicle such that the X-axis points out the front of the vehicle (bow), the
        Y-axis points out the right (starboard), and the Z axis points down."

        If the above is not true, you must set the Reference Frame Rotation Register
    */

    //set configuration register with custom binary output
    sendCommand(MSG_CONFIG_COMMAND, sizeof(MSG_CONFIG_COMMAND));
}


void VN300::parse_gps_data(GPSData_t& gpsData){
    //Define offsets
    const uint8_t TIME_OFFSET = 4;
    const uint8_t LATITUDE_OFFSET = 24;
    const uint8_t LONGITUDE_OFFSET = 32;
    const uint8_t ALTITUDE_OFFSET = 40;
    const uint8_t VELNORTH_OFFSET = 48;
    const uint8_t VELEAST_OFFSET = 52;
    const uint8_t VELDOWN_OFFSET = 56;

    gpsData.time = 0;
    for (int8_t i = 7; i >= 0; i--)
        gpsData.time |= ((uint64_t)payload_data[TIME_OFFSET + i] << (BITS_PER_BYTE * i));

    uint64_t temp = 0;
    for (int8_t i = 7; i >= 0; i--)
        temp |= ((uint64_t)payload_data[LATITUDE_OFFSET + i] << (BITS_PER_BYTE * i));
    gpsData.latitude = binary_to_double(temp);

    temp = 0;
    for (int8_t i = 7; i >= 0; i--)
        temp |= ((uint64_t)payload_data[LONGITUDE_OFFSET + i] << (BITS_PER_BYTE * i));
    gpsData.longitude = binary_to_double(temp);

    temp = 0;
    for (int8_t i = 7; i >= 0; i--)
        temp |= ((uint64_t)payload_data[ALTITUDE_OFFSET + i] << (BITS_PER_BYTE * i));
    gpsData.altitude = binary_to_double(temp);

    temp = 0;
    for (int8_t i = 3; i >= 0; i--)
        temp |= ((uint64_t)payload_data[VELNORTH_OFFSET + i] << (BITS_PER_BYTE * i));
    gpsData.velNorth = binary_to_float(temp);

    temp = 0;
    for (int8_t i = 3; i >= 0; i--)
        temp |= ((uint64_t)payload_data[VELEAST_OFFSET + i] << (BITS_PER_BYTE * i));
    gpsData.velEast = binary_to_float(temp);

    temp = 0;
    for (int8_t i = 3; i >= 0; i--)
        temp |= ((uint64_t)payload_data[VELDOWN_OFFSET + i] << (BITS_PER_BYTE * i));
    gpsData.velDown = binary_to_float(temp);

}

float VN300::binary_to_float(uint64_t val){
    float f = (float &)val;
    return f;
}

double VN300::binary_to_double(uint64_t val){
    double d = (double &)val;
    return d;
}

void VN300::parse_imu_data(IMUData_t& imuData){
    const uint8_t YAW_OFFSET = 12;
    const uint8_t PITCH_OFFSET = 16;
    const uint8_t ROLL_OFFSET = 20;
    const uint8_t ACCEL0_OFFSET = 60;
    const uint8_t ACCEL1_OFFSET = 64;
    const uint8_t ACCEL2_OFFSET = 68;

    uint64_t temp = 0;
    for (int8_t i = 3; i >= 0; i--)
        temp |= (payload_data[PITCH_OFFSET + i] << (BITS_PER_BYTE * i));
    imuData.pitch = binary_to_float(temp);

    temp = 0;
    for (int8_t i = 3; i >= 0; i--)
        temp |= (payload_data[ROLL_OFFSET + i] << (BITS_PER_BYTE * i));
    imuData.roll = binary_to_float(temp);

    temp = 0;
    for (int8_t i = 3; i >= 0; i--)
        temp |= (payload_data[YAW_OFFSET + i] << (BITS_PER_BYTE * i));
    imuData.yaw = binary_to_float(temp);

    temp = 0;
    for (int8_t i = 3; i >= 0; i--)
        temp |= (payload_data[ACCEL0_OFFSET + i] << (BITS_PER_BYTE * i));
    imuData.accel0 = binary_to_float(temp);

    temp = 0;
    for (int8_t i = 3; i >= 0; i--)
        temp |= (payload_data[ACCEL1_OFFSET + i] << (BITS_PER_BYTE * i));
    imuData.accel1 = binary_to_float(temp);

    temp = 0;
    for (int8_t i = 3; i >= 0; i--)
        temp |= (payload_data[ACCEL2_OFFSET + i] << (BITS_PER_BYTE * i));
    imuData.accel2 = binary_to_float(temp);
}

void VN300::sendCommand(const char* command, uint16_t size){
    HAL_UART_Transmit(&huart2, (uint8_t*)command, size, 100);
}

void VN300::recieveResponse(void){
    //__HAL_UART_CLEAR_OREFLAG(&huart2);
	HAL_UART_AbortReceive(&huart2);
    HAL_UART_Receive(&huart2, payload_data, PAYLOAD_SIZE, 1000);
}

