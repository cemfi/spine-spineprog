/*
    A library for Grove - 6-Axis Accelerometer&Gyroscope（BMI088）

    Copyright (c) 2018 seeed technology co., ltd.
    Author      : Wayen Weng
    Create Time : June 2018
    Change Log  :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef __BOSCH_BMI088_H__
#define __BOSCH_BMI088_H__

#include <Arduino.h>
#include <Wire.h>

#define BMI088_ACC_ADDRESS          0x19
#define BMI088_ACC_ALT_ADDRESS      0x18

#define BMI088_ACC_CHIP_ID          0x00 // Default value 0x1E
#define BMI088_ACC_ERR_REG          0x02
#define BMI088_ACC_STATUS           0x03

#define BMI088_ACC_X_LSB            0x12
#define BMI088_ACC_X_MSB            0x13
#define BMI088_ACC_Y_LSB            0x14
#define BMI088_ACC_Y_MSB            0x15
#define BMI088_ACC_Z_LSB            0x16
#define BMI088_ACC_Z_MSB            0x17

#define BMI088_ACC_SENSOR_TIME_0    0x18
#define BMI088_ACC_SENSOR_TIME_1    0x19
#define BMI088_ACC_SENSOR_TIME_2    0x1A

#define BMI088_ACC_INT_STAT_1       0x1D

#define BMI088_ACC_TEMP_MSB         0x22
#define BMI088_ACC_TEMP_LSB         0x23

#define BMI088_ACC_CONF             0x40
#define BMI088_ACC_RANGE            0x41

#define BMI088_ACC_INT1_IO_CTRL     0x53
#define BMI088_ACC_INT2_IO_CTRL     0x54
#define BMI088_ACC_INT_MAP_DATA     0x58

#define BMI088_ACC_SELF_TEST        0x6D

#define BMI088_ACC_PWR_CONF         0x7C
#define BMI088_ACC_PWR_CTRl         0x7D

#define BMI088_ACC_SOFT_RESET       0x7E

#define BMI088_GYRO_ADDRESS         0x69
#define BMI088_GYRO_ALT_ADDRESS     0x68


#define BMI088_GYRO_CHIP_ID             0x00 // Default value 0x0F

#define BMI088_GYRO_RATE_X_LSB          0x02
#define BMI088_GYRO_RATE_X_MSB          0x03
#define BMI088_GYRO_RATE_Y_LSB          0x04
#define BMI088_GYRO_RATE_Y_MSB          0x05
#define BMI088_GYRO_RATE_Z_LSB          0x06
#define BMI088_GYRO_RATE_Z_MSB          0x07

#define BMI088_GYRO_INT_STAT_1          0x0A

#define BMI088_GYRO_RANGE               0x0F
#define BMI088_GYRO_BAND_WIDTH          0x10

#define BMI088_GYRO_LPM_1               0x11

#define BMI088_GYRO_SOFT_RESET          0x14

#define BMI088_GYRO_INT_CTRL            0x15
#define BMI088_GYRO_INT3_INT4_IO_CONF   0x16
#define BMI088_GYRO_INT3_INT4_IO_MAP    0x18

#define BMI088_GYRO_SELF_TEST           0x3C

enum device_type_t { // device type
    ACC = 0x00, //
    GYRO = 0x01, //
};

enum acc_scale_type_t { // measurement rage
    RANGE_3G = 0x00, //
    RANGE_6G = 0x01, //
    RANGE_12G = 0x02, //
    RANGE_24G = 0x03, //
};

enum acc_odr_type_t { // output data rate
    ODR_12 = 0x05, //
    ODR_25 = 0x06, //
    ODR_50 = 0x07, //
    ODR_100 = 0x08, //
    ODR_200 = 0x09, //
    ODR_400 = 0x0A, //
    ODR_800 = 0x0B, //
    ODR_1600 = 0x0C, //
};

enum acc_power_type_t { // power mode
    ACC_ACTIVE = 0x00, //
    ACC_SUSPEND = 0x03, //
};

enum gyro_scale_type_t { // measurement rage
    RANGE_2000 = 0x00, //
    RANGE_1000 = 0x01, //
    RANGE_500 = 0x02, //
    RANGE_250 = 0x03, //
    RANGE_125 = 0x04, //
};

enum gyro_odr_type_t { // output data rate
    ODR_2000_BW_532 = 0x00, //
    ODR_2000_BW_230 = 0x01, //
    ODR_1000_BW_116 = 0x02, //
    ODR_400_BW_47 = 0x03, //
    ODR_200_BW_23 = 0x04, //
    ODR_100_BW_12 = 0x05, //
    ODR_200_BW_64 = 0x06, //
    ODR_100_BW_32 = 0x07, //
};

enum gyro_power_type_t { // power mode
    GYRO_NORMAL = 0x00, //
    GYRO_SUSPEND = 0x80, //
    GYRO_DEEP_SUSPEND = 0x20, //
};

class BMI088{
public:

    BMI088( unsigned short, unsigned short );

    bool isConnection(void);

    void initialize(void);

    void setAccPoweMode(acc_power_type_t mode);
    void setGyroPoweMode(gyro_power_type_t mode);

    void setAccScaleRange(acc_scale_type_t range);
    void setAccOutputDataRate(acc_odr_type_t odr);

    void setGyroScaleRange(gyro_scale_type_t range);
    void setGyroOutputDataRate(gyro_odr_type_t odr);

    void getAcceleration(float* x, float* y, float* z);
    float getAccelerationX(void);
    float getAccelerationY(void);
    float getAccelerationZ(void);

    void getGyroscope(float* x, float* y, float* z);
    float getGyroscopeX(void);
    float getGyroscopeY(void);
    float getGyroscopeZ(void);

    int16_t getTemperature(void);

    uint8_t getAccID(void);
    uint8_t getGyroID(void);

    void resetAcc(void);
    void resetGyro(void);

  private:

    void write8(device_type_t dev, uint8_t reg, uint8_t val);
    uint8_t read8(device_type_t dev, uint8_t reg);
    uint16_t read16(device_type_t dev, uint8_t reg);
    uint16_t read16Be(device_type_t dev, uint8_t reg);
    uint32_t read24(device_type_t dev, uint8_t reg);
    void read(device_type_t dev, uint8_t reg, uint8_t* buf, uint16_t len);

    float accRange;
    float gyroRange;
    uint8_t devAddrAcc;
    uint8_t devAddrGyro;
};

#endif


/*
    A library for Grove - 6-Axis Accelerometer&Gyroscope（BMI088）

    Copyright (c) 2018 seeed technology co., ltd.
    Author      : Wayen Weng
    Create Time : June 2018
    Change Log  :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/


BMI088::BMI088( unsigned short acc_addr = BMI088_ACC_ADDRESS, unsigned short gyro_addr = BMI088_GYRO_ADDRESS ){
    devAddrAcc = acc_addr;
    devAddrGyro = gyro_addr;
}

void BMI088::initialize(void) {
    setAccScaleRange(RANGE_6G);
    setAccOutputDataRate(ODR_100);
    setAccPoweMode(ACC_ACTIVE);

    setGyroScaleRange(RANGE_2000);
    setGyroOutputDataRate(ODR_2000_BW_532);
    setGyroPoweMode(GYRO_NORMAL);
}

bool BMI088::isConnection(void) {
    return ((getAccID() == 0x1E) && (getGyroID() == 0x0F));
}

void BMI088::resetAcc(void) {
    write8(ACC, BMI088_ACC_SOFT_RESET, 0xB6);
}

void BMI088::resetGyro(void) {
    write8(GYRO, BMI088_GYRO_SOFT_RESET, 0xB6);
}

uint8_t BMI088::getAccID(void) {
    return read8(ACC, BMI088_GYRO_CHIP_ID);
}

uint8_t BMI088::getGyroID(void) {
    return read8(GYRO, BMI088_GYRO_CHIP_ID);
}

void BMI088::setAccPoweMode(acc_power_type_t mode) {
    if (mode == ACC_ACTIVE) {
        write8(ACC, BMI088_ACC_PWR_CTRl, 0x04);
        write8(ACC, BMI088_ACC_PWR_CONF, 0x00);
    } else if (mode == ACC_SUSPEND) {
        write8(ACC, BMI088_ACC_PWR_CONF, 0x03);
        write8(ACC, BMI088_ACC_PWR_CTRl, 0x00);
    }
}

void BMI088::setGyroPoweMode(gyro_power_type_t mode) {
    if (mode == GYRO_NORMAL) {
        write8(GYRO, BMI088_GYRO_LPM_1, (uint8_t)GYRO_NORMAL);
    } else if (mode == GYRO_SUSPEND) {
        write8(GYRO, BMI088_GYRO_LPM_1, (uint8_t)GYRO_SUSPEND);
    } else if (mode == GYRO_DEEP_SUSPEND) {
        write8(GYRO, BMI088_GYRO_LPM_1, (uint8_t)GYRO_DEEP_SUSPEND);
    }
}

void BMI088::setAccScaleRange(acc_scale_type_t range) {
    if (range == RANGE_3G) {
        accRange = 3000;
    } else if (range == RANGE_6G) {
        accRange = 6000;
    } else if (range == RANGE_12G) {
        accRange = 12000;
    } else if (range == RANGE_24G) {
        accRange = 24000;
    }

    write8(ACC, BMI088_ACC_RANGE, (uint8_t)range);
}

void BMI088::setAccOutputDataRate(acc_odr_type_t odr) {
    uint8_t data = 0;

    data = read8(ACC, BMI088_ACC_CONF);
    data = data & 0xf0;
    data = data | (uint8_t)odr;

    write8(ACC, BMI088_ACC_CONF, data);
}

void BMI088::setGyroScaleRange(gyro_scale_type_t range) {
    if (range == RANGE_2000) {
        gyroRange = 2000;
    } else if (range == RANGE_1000) {
        gyroRange = 1000;
    } else if (range == RANGE_500) {
        gyroRange = 500;
    } else if (range == RANGE_250) {
        gyroRange = 250;
    } else if (range == RANGE_125) {
        gyroRange = 125;
    }

    write8(GYRO, BMI088_GYRO_RANGE, (uint8_t)range);
}

void BMI088::setGyroOutputDataRate(gyro_odr_type_t odr) {
    write8(GYRO, BMI088_GYRO_BAND_WIDTH, (uint8_t)odr);
}

void BMI088::getAcceleration(float* x, float* y, float* z) {
    uint8_t buf[6] = {0};
    uint16_t ax = 0, ay = 0, az = 0;
    float value = 0;

    read(ACC, BMI088_ACC_X_LSB, buf, 6);

    ax = buf[0] | (buf[1] << 8);
    ay = buf[2] | (buf[3] << 8);
    az = buf[4] | (buf[5] << 8);

    value = (int16_t)ax;
    *x = accRange * value / 32768;

    value = (int16_t)ay;
    *y = accRange * value / 32768;

    value = (int16_t)az;
    *z = accRange * value / 32768;
}

float BMI088::getAccelerationX(void) {
    uint16_t ax = 0;
    float value = 0;

    ax = read16(ACC, BMI088_ACC_X_LSB);

    value = (int16_t)ax;
    value = accRange * value / 32768;

    return value;
}

float BMI088::getAccelerationY(void) {
    uint16_t ay = 0;
    float value = 0;

    ay = read16(ACC, BMI088_ACC_Y_LSB);

    value = (int16_t)ay;
    value = accRange * value / 32768;

    return value;
}

float BMI088::getAccelerationZ(void) {
    uint16_t az = 0;
    float value = 0;

    az = read16(ACC, BMI088_ACC_Z_LSB);

    value = (int16_t)az;
    value = accRange * value / 32768;

    return value;
}

void BMI088::getGyroscope(float* x, float* y, float* z) {
    uint8_t buf[6] = {0};
    uint16_t gx = 0, gy = 0, gz = 0;
    float value = 0;

    read(GYRO, BMI088_GYRO_RATE_X_LSB, buf, 6);

    gx = buf[0] | (buf[1] << 8);
    gy = buf[2] | (buf[3] << 8);
    gz = buf[4] | (buf[5] << 8);

    value = (int16_t)gx;
    *x = gyroRange * value / 32768;

    value = (int16_t)gy;
    *y = gyroRange * value / 32768;

    value = (int16_t)gz;
    *z = gyroRange * value / 32768;
}

float BMI088::getGyroscopeX(void) {
    uint16_t gx = 0;
    float value = 0;

    gx = read16(GYRO, BMI088_GYRO_RATE_X_LSB);

    value = (int16_t)gx;
    value = gyroRange * value / 32768;

    return value;
}

float BMI088::getGyroscopeY(void) {
    uint16_t gy = 0;
    float value = 0;

    gy = read16(GYRO, BMI088_GYRO_RATE_Y_LSB);

    value = (int16_t)gy;
    value = gyroRange * value / 32768;

    return value;
}

float BMI088::getGyroscopeZ(void) {
    uint16_t gz = 0;
    float value = 0;

    gz = read16(GYRO, BMI088_GYRO_RATE_Z_LSB);

    value = (int16_t)gz;
    value = gyroRange * value / 32768;

    return value;
}

int16_t BMI088::getTemperature(void) {
    uint16_t data = 0;

    data = read16Be(ACC, BMI088_ACC_TEMP_MSB);
    data = data >> 5;

    if (data > 1023) {
        data = data - 2048;
    }

    return (int16_t)(data / 8 + 23);
}

void BMI088::write8(device_type_t dev, uint8_t reg, uint8_t val) {
    uint8_t addr = 0;

    if (dev) {
        addr = devAddrGyro;
    } else {
        addr = devAddrAcc;
    }

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

uint8_t BMI088::read8(device_type_t dev, uint8_t reg) {
    uint8_t addr = 0, data = 0;

    if (dev) {
        addr = devAddrGyro;
    } else {
        addr = devAddrAcc;
    }

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(addr, (uint8_t)1);
    while (Wire.available()) {
        data = Wire.read();
    }

    return data;
}

uint16_t BMI088::read16(device_type_t dev, uint8_t reg) {
    uint8_t addr = 0;
    uint16_t msb = 0, lsb = 0;

    if (dev) {
        addr = devAddrGyro;
    } else {
        addr = devAddrAcc;
    }

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(addr, (uint8_t)2);
    while (Wire.available()) {
        lsb = Wire.read();
        msb = Wire.read();
    }

    return (lsb | (msb << 8));
}

uint16_t BMI088::read16Be(device_type_t dev, uint8_t reg) {
    uint8_t addr = 0;
    uint16_t msb = 0, lsb = 0;

    if (dev) {
        addr = devAddrGyro;
    } else {
        addr = devAddrAcc;
    }

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(addr, (uint8_t)2);
    while (Wire.available()) {
        msb = Wire.read();
        lsb = Wire.read();
    }

    return (lsb | (msb << 8));
}

uint32_t BMI088::read24(device_type_t dev, uint8_t reg) {
    uint8_t addr = 0;
    uint32_t hsb = 0, msb = 0, lsb = 0;

    if (dev) {
        addr = devAddrGyro;
    } else {
        addr = devAddrAcc;
    }

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(addr, (uint8_t)3);
    while (Wire.available()) {
        lsb = Wire.read();
        msb = Wire.read();
        hsb = Wire.read();
    }

    return (lsb | (msb << 8) | (hsb << 16));
}

void BMI088::read(device_type_t dev, uint8_t reg, uint8_t* buf, uint16_t len) {
    uint8_t addr = 0;

    if (dev) {
        addr = devAddrGyro;
    } else {
        addr = devAddrAcc;
    }

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(addr, len);
    while (Wire.available()) {
        for (uint16_t i = 0; i < len; i ++) {
            buf[i] = Wire.read();
        }
    }
}


float ax = 0, ay = 0, az = 0;
float gx = 0, gy = 0, gz = 0;
int16_t temp = 0;
BMI088 bmi088( BMI088_ACC_ADDRESS, BMI088_GYRO_ADDRESS );


void setup(void) {
    Wire.begin();
    while (1) {
        if (bmi088.isConnection()) {
            bmi088.initialize();
            break;
        } else {
            // Serial.println("BMI088 is not connected");
        }
    }
}

void loop(void) {
    bmi088.getAcceleration(&ax, &ay, &az);
    bmi088.getGyroscope(&gx, &gy, &gz);
    temp = bmi088.getTemperature();

    Spine.write(connector, 0, ax);
    Spine.write(connector, 1, ay);
    Spine.write(connector, 2, az);
    
    Spine.write(connector, 3, gx);
    Spine.write(connector, 4, gy);
    Spine.write(connector, 5, gz);
    
    Spine.write(connector, 6, temp);
}
