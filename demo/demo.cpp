#include <iostream>
#include <unistd.h>
#include <format>

#include "RPLidar.h"

#define ANGLE_RANGE 360

int main() {
    const auto lidar = new RPLidar();

    if (!lidar->connect("/dev/ttyUSB0", 460800)) {
        std::cerr << "Failed to connect to LiDAR" << std::endl;
        delete lidar;
        return -1;
    }
    std::cout << "Connected" << std::endl;

    if (!lidar->isHealthy()) {
        const auto deviceHealthInfo = lidar->getDeviceHealthInfo();
        std::cerr << "Device is unhealthy: status=" << deviceHealthInfo.status << ", errorCode="
                  << deviceHealthInfo.errorCode << std::endl;
        lidar->disconnect();
        delete lidar;
        return -1;
    }

    if (!lidar->setMotorSpeed()) {
        std::cerr << "Failed to connect to LiDAR" << std::endl;
        lidar->disconnect();
        delete lidar;

        return -1;
    }
    std::cout << "Set motor speed" << std::endl;

    if (!lidar->startScan()) {
        std::cerr << "Failed to start scan" << std::endl;
        lidar->disconnect();
        delete lidar;

        return -1;
    }

    const auto scanDataList = new ScanData[ANGLE_RANGE];

    for (size_t i = 0; i < 128; i++) {
        std::cout << std::endl << "Iteration: " << i << std::endl << std::endl;

        if (!lidar->getScanData(scanDataList)) {
            std::cerr << "Failed to get scan data" << std::endl;
            usleep(100 * 1000);

            continue;
        }

        for (size_t angle = 0; angle < ANGLE_RANGE; angle++) {
            const auto scanData = scanDataList[angle];
            std::cout << angle << ": distance=" << scanData.distance << ", quality="
                      << std::format("{:d}", scanData.quality) << std::endl;
        }

        usleep(100 * 1000);
    }

    delete[] scanDataList;

    lidar->stopScan();
    lidar->disconnect();

    delete lidar;


    return 0;
}