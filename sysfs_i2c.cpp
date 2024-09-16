/**
 * @file    sysfs_i2c.cpp
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    17-09-24
 *
 * @brief   SYSFS I2C wrapper
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "sysfs_i2c.h"

namespace sysfs {

I2C_Wrapper::I2C_Wrapper() {
}

I2C_Wrapper::~I2C_Wrapper() {
}

bool I2C_Wrapper::lockBus(uint8_t addr) {
    m_mutex.lock();
    if (!m_deviceAddress && addr) {
        if (ioctl(m_fd, I2C_SLAVE, addr) < 0) {
            m_mutex.unlock();
            return false;
        }

        m_deviceAddress = addr;
        m_mutex.unlock();
        return true;
    } else if (m_deviceAddress == addr) {
        m_mutex.unlock();
        return true;
    }
    m_mutex.unlock();
    return false;
}

bool I2C_Wrapper::unlockBus(uint8_t addr) {
    m_mutex.lock();
    if (!m_deviceAddress) {
        m_mutex.unlock();
        return true;
    } else if (m_deviceAddress == addr) {
        m_deviceAddress = 0;
        m_mutex.unlock();
        return true;
    }
    m_mutex.unlock();
    return false;
}

bool I2C_Wrapper::isOpen() {
    return (m_fd >= 0);
}

bool I2C_Wrapper::isLocked(uint8_t addr) {
    return (m_deviceAddress != addr);
}

int I2C_Wrapper::openPort(const char* path) {

    int ret = open(path, O_RDWR);
    if (ret >= 0) {
        m_portName = path;
    }

    m_fd = ret;

    return ret;
}

void I2C_Wrapper::closePort() {
    if (m_fd >= 0) {
        close(m_fd);
        m_fd = -1;
    }
}

int I2C_Wrapper::writeData(const uint8_t *data, size_t size) {
    if (!data || !size) {
        return 0;
    }

    int ret = 0;
    m_mutex.lock();
    ret = write(m_fd, data, size);
    m_mutex.unlock();
    return ret;
}

int I2C_Wrapper::readData(uint8_t *data, size_t size) {
    if (!data || !size) {
        return 0;
    }

    int ret = 0;
    m_mutex.lock();
    ret = read(m_fd, data, size);
    m_mutex.unlock();

    return ret;
}

}
