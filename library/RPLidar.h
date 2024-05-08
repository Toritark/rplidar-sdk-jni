#ifndef LIDAR_RPLIDAR_H
#define LIDAR_RPLIDAR_H

#include "sl_lidar_driver.h"

using namespace sl;

#define DEFAULT_STOP_SCAN_TIMEOUT_MS 2000
#define DEFAULT_GET_SCAN_DATA_TIMEOUT_MS 2000

typedef struct DeviceHealth {
    uint8_t status;
    uint16_t errorCode;
} DeviceHealth;

typedef struct ScanData {
    float distance = 0.0f;
    uint8_t quality = 0;
} ScanData;

class RPLidar {
public:
    RPLidar();

    ~RPLidar();

    bool connect(const std::string &serialPort, int baudRate);

    bool disconnect();

    bool isConnected();

    DeviceHealth getDeviceHealthInfo();

    bool isHealthy();

    bool setMotorSpeed(uint16_t speed = DEFAULT_MOTOR_SPEED);

    [[nodiscard]] bool isScanning() const;

    bool startScan();

    bool stopScan(uint32_t timeoutMs = DEFAULT_STOP_SCAN_TIMEOUT_MS);

    bool getScanData(ScanData *scanDataList, uint32_t timeoutMs = DEFAULT_GET_SCAN_DATA_TIMEOUT_MS);

private:
    ILidarDriver *lidarDriver;
    bool isScanStarted = false;
};


#endif //LIDAR_RPLIDAR_H
