/*
 * vectorNav.hpp
 *
 *  Created on: Jun 23, 2022
 *      Author: Christopher Chung
 */

#ifndef INC_VECTORNAV_HPP_
#define INC_VECTORNAV_HPP_

#include <main.hpp>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// Header strings that are sent to the VN300 to request data from it

const char* GPS_HEADER = "$VNGPS";
const char* IMU_HEADER = "$VNIMU";

typedef struct{
    uint64_t time;        // Time since startup in nanoseconds
    double latitude;      // Latitude in degrees
    double longitude;     // Longitude in degrees
    double altitude;      // Altitude in m
    float velNorth;       // Velocity in North frame in m/s
    float velEast;        // Velocity in East frame in m/s
    float velDown;        // Velocity in Down frame in m/s
}GPSData_t;

typedef struct{
    float pitch;        // Pitch in degrees
    float roll;         // Roll in degrees
    float yaw;          // Yaw in degrees
    float accel0;       // Acceleration in m/s^2 in body frame 0
    float accel1;       // Acceleration in m/s^2 in body frame 1
    float accel2;       // Acceleration in m/s^2 in body frame 2
}IMUData_t;

// This is the enum that will signify which data to send
typedef struct {
    bool gps;
    bool imu;
}requestData_t;

/*
    -This is the struct that will be passed in and out of GetResult
    -Based on type_data, we will know which data to send with all the rest being 0
*/
typedef struct{
    GPSData_t gps_data;
    IMUData_t imu_data;
    requestData_t req_data;
}PositionData_t;

class Position {
    public:
        virtual void GetResult(PositionData_t& data) = 0;
};

/*
    This class inherits the Position class and is used for the VN300 operation
*/
class VN300: public Position{
    public:
        
        /**
         * @brief Gets the only instance of this class, following the singleton model
         * 
         * @return The singleton instance
         */
        static Position& getInstance();

        /**
         * @brief Constructor that will instantly delete the object that was instantiated to protect the singleton model
         */
        VN300(const VN300*)=delete;

        /**
         * @brief Get the IMU and GPS data
         * 
         * @param data Struct holding the IMU and GPS data
         */
        void GetResult(PositionData_t& data);

    private:

        //I think this calls the constructor and makes our single instance
        VN300();

        /**
         * @brief Initialization of the VN300
         */
        void VN300Init(void); //we may not need this

        /**
         * @brief Gets the GPS data from the VN300
         * 
         * @param gpsData The struct holding the GPS data
         */
        void getGPSData(GPSData_t& gpsData);

        void parse_gps_data(GPSData_t& gpsData);

        float binary_to_float(uint64_t val);

        double binary_to_double(uint64_t val);
        
        /**
         * @brief Sends an ASCII command through UART
         * 
         * @param command ASCII command string 
         */

        void parse_imu_data(IMUData_t& imuData);
        void sendCommand(const char* command, uint16_t size);

        void recieveResponse(void);

};

#endif /* INC_VECTORNAV_HPP_ */
