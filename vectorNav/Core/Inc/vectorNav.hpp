/*
 * vectorNav.hpp
 *
 *  Created on: Jun 23, 2022
 *      Author: Christopher Chung
 */

#ifndef INC_VECTORNAV_HPP_
#define INC_VECTORNAV_HPP_

#include "stm32f0xx_hal.h"
#include <stdint.h>

// Header strings that are sent to the VN300 to request data from it
#define GPS_HEADER "$VNGPS"
#define IMU_HEADER "$VNIMU"

// struct based on section 8.8.1 here: https://geo-matching.com/uploads/default/m/i/migrationca8kud.pdf
typedef struct{
    double time;        // GPS time of week in seconds
    uint16_t week;      // GPS week
    uint8_t gpsFix;     // GPS fix type
    uint8_t numSats;    // Number of GPS satellites used in solution
    double latitude;    // Latitude in degrees
    double longitude;   // Longitude in degrees
    double altitude;    // Altitude above ellipsoid
    float nedVelX;      // Velocity measurement in north direction in m/s
    float nedVelY;      // Velocity measurement in east direction in m/s
    float nedVelZ;      // Velocity measurement in down direction in m/s
    float northAcc;     // North position accuracy estimate in m
    float eastAcc;      // East position accuracy estimate in m
    float vertAcc;      // Vertical position accuracy estimate in m
    float speedAcc;     // Speed accuracy estimate in m/s
    float timeAcc;      // Time accuracy estimate in seconds
}GPSData_t; 

// struct based on section 1.7.7 here: https://geo-matching.com/uploads/default/m/i/migrationca8kud.pdf
typedef struct{
    float magX;         // Uncompensated Magnetic X-axis in Gauss
    float magY;         // Uncompensated Magnetic Y-axis in Gauss
    float magZ;         // Uncompensated Magnetic Z-axis in Gauss
    float AccelX;       // Uncompensated Acceleration X-axis in m/s^2
    float AccelY;       // Uncompensated Acceleration Y-axis in m/s^2
    float AccelZ;       // Uncompensated Acceleration Z-axis in m/s^2
    float GyroX;        // Uncompensated Angular rate X-axis in rad/s
    float GyroY;        // Uncompensated Angular rate Y-axis in rad/s
    float GyroZ;        // Uncompensated Angular rate Z-axis in rad/s
    float temp;         // IMU Temperature in degrees C
    float pressure;     // Barometric pressure in kPa
}IMUData_t;

// This is the enum that will signify which data to send
typedef enum {
    gpsImu = 0,
    gps,
    imu
}typeData_e;

/*
    -This is the struct that will be passed in and out of GetResult
    -Based on type_data, we will know which data to send with all the rest being 0
*/
typedef struct{
    GPSData_t gps_data;
    IMUData_t imu_data;
    typeData_e type_data;
}PositionData_t

class Position{
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

        /**
         * @brief Gets the IMU data from the VN300
         * 
         * @param imuData The struct holding the IMU data
         */
        void getIMUData(IMUData_t& imuData);

};

#endif /* INC_VECTORNAV_HPP_ */