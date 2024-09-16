/**
 * @file    sysfs_i2c.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    17-09-24
 *
 * @brief   SYSFS I2C wrapper
 */
#ifndef SYSFS_I2C_H
#define SYSFS_I2C_H

#include <string>
#include <array>
#include <mutex>

namespace sysfs {

class I2C_Wrapper {
public:
    I2C_Wrapper();
    ~I2C_Wrapper();

    bool lockBus(uint8_t addr);
    bool unlockBus(uint8_t addr);
    bool isLocked(uint8_t addr);

    bool isOpen();
    int openPort(const char* path);
    void closePort();

    int writeData(const uint8_t* data, size_t size);
    int readData(uint8_t* data, size_t size);

private:
    int m_fd = -1;
    uint8_t m_deviceAddress = 0;
    const char* m_portName = nullptr;
    std::mutex m_mutex;
};
}

#endif /* SYSFS_I2C_H */
/* EOF */
