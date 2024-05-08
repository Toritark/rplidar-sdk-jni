#include "RPLidar.h"

#include <iostream>
#include <unistd.h>

#define SL_IS_OK(x)    ( ((x) & SL_RESULT_FAIL_BIT) == 0 )
#define SL_IS_FAIL(x)  ( ((x) & SL_RESULT_FAIL_BIT) )

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

#define MIN_ANGLE 0
#define MAX_ANGLE 359

RPLidar::RPLidar() {
    lidarDriver = *createLidarDriver();
}

RPLidar::~RPLidar() {
    if (lidarDriver != nullptr) {
        disconnect();

        delete lidarDriver;
        lidarDriver = nullptr;
    }
}


bool RPLidar::connect(const std::string &serialPort, int baudRate) {
    const auto channel = *createSerialPortChannel(serialPort, baudRate);
    const auto connectionResult = lidarDriver->connect(channel);

    if (!SL_IS_OK(connectionResult)) {
        std::cerr << "Failed to connect: " << connectionResult << std::endl;
        return false;
    }

    return true;
}

bool RPLidar::disconnect() {
    if (isConnected()) {
        if (isScanning()) {
            stopScan();
        }

        usleep(100 * 1000);

        setMotorSpeed(0);

        usleep(100 * 1000);

        lidarDriver->disconnect();

        return true;
    }

    return false;
}


bool RPLidar::isConnected() {
    return lidarDriver != nullptr && lidarDriver->isConnected();
}

DeviceHealth RPLidar::getDeviceHealthInfo() {
    DeviceHealth deviceHealth;
    sl_lidar_response_device_health_t healthInfo;

    const auto result = lidarDriver->getHealth(healthInfo);
    if (SL_IS_OK(result)) {
        deviceHealth.status = healthInfo.status;
        deviceHealth.errorCode = healthInfo.error_code;
    } else {
        deviceHealth.status = 255;
        deviceHealth.errorCode = 255;
    }

    return deviceHealth;
}

bool RPLidar::isHealthy() {
    const auto deviceHealth = getDeviceHealthInfo();
    return deviceHealth.status == SL_LIDAR_STATUS_OK;
}

bool RPLidar::setMotorSpeed(uint16_t speed) {
    const auto result = lidarDriver->setMotorSpeed(speed);

    if (SL_IS_FAIL(result)) {
        std::cerr << "Failed to set motor speed: " << result << std::endl;
        return false;
    }

    return true;
}

bool RPLidar::isScanning() const {
    return isScanStarted;
}

bool RPLidar::startScan() {
    if (isScanStarted) {
        std::cerr << "Failed to start scan: already started" << std::endl;
    }

    const auto result = lidarDriver->startScan(false, true);

    if (SL_IS_FAIL(result)) {
        std::cerr << "Failed to start scan: " << result << std::endl;
        return false;
    }

    isScanStarted = true;

    return true;
}

bool RPLidar::stopScan(uint32_t timeoutMs) {
    const auto result = lidarDriver->stop(timeoutMs);

    if (SL_IS_FAIL(result)) {
        std::cerr << "Failed to stop scan: " << result << std::endl;
        return false;
    }

    isScanStarted = false;

    return true;
}

bool RPLidar::getScanData(ScanData *scanDataList, uint32_t timeoutMs) {
    sl_lidar_response_measurement_node_hq_t scanNodes[8192];
    size_t scanNodesCount = _countof(scanNodes);

    auto result = lidarDriver->grabScanDataHq(scanNodes, scanNodesCount);
    if (SL_IS_FAIL(result)) {
        std::cerr << "Failed get scan data (grabScanDataHq): " << result << std::endl;
        return false;
    }

    result = lidarDriver->ascendScanData(scanNodes, scanNodesCount);
    if (SL_IS_FAIL(result)) {
        std::cerr << "Failed get scan data (ascendScanData): " << result << std::endl;
        return false;
    }

    for (size_t i = 0; i < scanNodesCount; i++) {
        const auto scanNode = scanNodes[i];

        auto angle = (int) ((scanNode.angle_z_q14 * 90.f) / (1 << 14)); // NOLINT(*-narrowing-conversions)
        if (angle < MIN_ANGLE) {
            std::cout << "Got angle < " << MIN_ANGLE << " = " << angle << std::endl;
            angle = MIN_ANGLE;
        }
        if (angle > MAX_ANGLE) {
            std::cout << "Got angle > " << MAX_ANGLE << " = " << angle << std::endl;
            angle = MAX_ANGLE;
        }

        const auto distance = scanNode.dist_mm_q2 / (1 << 2); // NOLINT(*-narrowing-conversions)
        const auto quality =
                scanNode.quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT; // NOLINT(*-narrowing-conversions)

        scanDataList[angle].distance = distance;
        scanDataList[angle].quality = quality;
    }

    return true;
}






