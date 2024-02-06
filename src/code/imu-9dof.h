/// MPU6050 instance variables
uint8_t devAddr;
uint8_t buffer[14];
//////// end

void getCompassDate_calibrated();
void getCompass_Data(void);
void getAccelGyro_Data(void);
void get_one_sample_date_mxyz();
void get_calibration_Data();
void getTiltHeading(void);
void getHeading(void);
void MPU6050setClockSource(uint8_t);
void MPU6050setFullScaleGyroRange(uint8_t);
void MPU6050setFullScaleAccelRange(uint8_t);
void MPU6050setSleepEnabled(bool);
void MPU6050();
void MPU6050initialize();
void MPU6050getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);
bool I2CdevwriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
bool I2CdevwriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data);
bool I2CdevwriteBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
bool I2CdevwriteBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);


const uint16_t I2CdevreadTimeout = 1000;
int8_t I2CdevreadByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout = I2CdevreadTimeout);
int8_t I2CdevreadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout = I2CdevreadTimeout);


const uint8_t MPU6050_RA_PWR_MGMT_1 = 0x6B;
const uint8_t MPU6050_PWR1_SLEEP_BIT = 6;
const uint8_t MPU6050_RA_ACCEL_CONFIG = 0x1C;
const uint8_t MPU6050_ACONFIG_AFS_SEL_BIT = 4;
const uint8_t MPU6050_ACONFIG_AFS_SEL_LENGTH = 2;
const uint8_t MPU6050_RA_GYRO_CONFIG = 0x1B;
const uint8_t MPU6050_GCONFIG_FS_SEL_BIT = 4;
const uint8_t MPU6050_GCONFIG_FS_SEL_LENGTH = 2;
const uint8_t MPU6050_PWR1_CLKSEL_BIT = 2;
const uint8_t MPU6050_PWR1_CLKSEL_LENGTH = 3;
const uint8_t MPU6050_RA_INT_PIN_CFG = 0x37;
const uint8_t MPU9150_RA_MAG_ADDRESS = 0x0C;
const uint8_t MPU9150_RA_MAG_XOUT_L = 0x03;
const uint8_t MPU6050_DEFAULT_ADDRESS = 0x68;
const uint8_t MPU6050_RA_ACCEL_XOUT_H = 0x3B;
const uint8_t MPU6050_CLOCK_PLL_XGYRO = 0x01;
const uint8_t MPU6050_GYRO_FS_2000 = 0x03;
const uint8_t MPU6050_ACCEL_FS_16 = 0x03;

uint8_t buffer_m[6];
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;
float Axyz[3];
float Gxyz[3];
float Mxyz[3];

void setup() {
  Wire.begin();
  MPU6050();
  MPU6050initialize();
}

void loop() 
{
  long now = millis();
  if (now < 100) {
    MPU6050initialize(); // initialization sometimes fails in the very beginning
  }
  
  getAccelGyro_Data();
  getCompass_Data();  

/*
  Serial.print("Accel.: ");
  Serial.print(Axyz[0], 2); 
  Serial.print(", ");
  Serial.print(Axyz[1], 2); 
  Serial.print(", ");
  Serial.print(Axyz[2], 2); 
  Serial.print("; Gyro:");
  Serial.print(Gxyz[0], 2); 
  Serial.print(", ");
  Serial.print(Gxyz[1], 2); 
  Serial.print(", ");
  Serial.print(Gxyz[2], 2); 
  Serial.print("; Compass:");
  Serial.print(Mxyz[0], 2); 
  Serial.print(", ");
  Serial.print(Mxyz[1], 2); 
  Serial.print(", ");
  Serial.print(Mxyz[2], 2);
*/
  Spine.write(connector, 0, Axyz[0]);
  Spine.write(connector, 1, Axyz[1]);
  Spine.write(connector, 2, Axyz[2]);
  Spine.write(connector, 3, Gxyz[0]);
  Spine.write(connector, 4, Gxyz[1]);
  Spine.write(connector, 5, Gxyz[2]);
  Spine.write(connector, 6, Mxyz[0]);
  Spine.write(connector, 7, Mxyz[1]);
  Spine.write(connector, 8, Mxyz[2]);

}	


void getAccelGyro_Data(void)
{
  MPU6050getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Axyz[0] = (float) ax * 16 / 32768;
  Axyz[1] = (float) ay * 16 / 32768;
  Axyz[2] = (float) az * 16 / 32768; 
  Gxyz[0] = (float) gx * 2000 / 32768;
  Gxyz[1] = (float) gy * 2000 / 32768;
  Gxyz[2] = (float) gz * 2000 / 32768;
}




/** Get raw 6-axis motion sensor readings (accel/gyro).
 * Retrieves all currently available motion sensor values.
 * @param ax 16-bit signed integer container for accelerometer X-axis value
 * @param ay 16-bit signed integer container for accelerometer Y-axis value
 * @param az 16-bit signed integer container for accelerometer Z-axis value
 * @param gx 16-bit signed integer container for gyroscope X-axis value
 * @param gy 16-bit signed integer container for gyroscope Y-axis value
 * @param gz 16-bit signed integer container for gyroscope Z-axis value
 * @see getAcceleration()
 * @see getRotation()
 * @see MPU6050_RA_ACCEL_XOUT_H
 */
void MPU6050getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
    I2CdevreadBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 14, buffer);
    *ax = (((int16_t)buffer[0]) << 8) | buffer[1];
    *ay = (((int16_t)buffer[2]) << 8) | buffer[3];
    *az = (((int16_t)buffer[4]) << 8) | buffer[5];
    *gx = (((int16_t)buffer[8]) << 8) | buffer[9];
    *gy = (((int16_t)buffer[10]) << 8) | buffer[11];
    *gz = (((int16_t)buffer[12]) << 8) | buffer[13];
}



void getCompass_Data(void)
{

    static long lastMillis = -1;
    if (lastMillis == -1) {
        I2CdevwriteByte(devAddr, MPU6050_RA_INT_PIN_CFG, 0x02); //set i2c bypass enable pin to true to access magnetometer
        I2CdevwriteByte(MPU9150_RA_MAG_ADDRESS, 0x0A, 0x01); //enable the magnetometer
        lastMillis = millis();
    }
    long now = millis();
    
    if (now - lastMillis > 10) {
        I2CdevreadBytes(MPU9150_RA_MAG_ADDRESS, MPU9150_RA_MAG_XOUT_L, 6, buffer);
        mx = (((int16_t)buffer[1]) << 8) | buffer[0];
        my = (((int16_t)buffer[3]) << 8) | buffer[2];
        mz = (((int16_t)buffer[5]) << 8) | buffer[4];
    
        Mxyz[0] = (float) mx * 1200 / 4096;
        Mxyz[1] = (float) my * 1200 / 4096;
        Mxyz[2] = (float) mz * 1200 / 4096;
        lastMillis = -1;
    }
}


/** Default constructor, uses default I2C address.
 * @see MPU6050_DEFAULT_ADDRESS
 */
void MPU6050() {
    devAddr = MPU6050_DEFAULT_ADDRESS;
}




/** Power on and prepare for general usage.
 * This will activate the device and take it out of sleep mode (which must be done
 * after start-up). This function also sets both the accelerometer and the gyroscope
 * to their most sensitive settings, namely +/- 2g and +/- 250 degrees/sec, and sets
 * the clock source to use the X Gyro for reference, which is slightly better than
 * the default internal clock source.
 */
void MPU6050initialize() {
    MPU6050setClockSource(MPU6050_CLOCK_PLL_XGYRO);
    MPU6050setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
    MPU6050setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
    MPU6050setSleepEnabled(false); // thanks to Jack Elston for pointing this one out!
}

/** Set clock source setting.
 * An internal 8MHz oscillator, gyroscope based clock, or external sources can
 * be selected as the MPU-60X0 clock source. When the internal 8 MHz oscillator
 * or an external source is chosen as the clock source, the MPU-60X0 can operate
 * in low power modes with the gyroscopes disabled.
 *
 * Upon power up, the MPU-60X0 clock source defaults to the internal oscillator.
 * However, it is highly recommended that the device be configured to use one of
 * the gyroscopes (or an external clock source) as the clock reference for
 * improved stability. The clock source can be selected according to the following table:
 *
 * <pre>
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
 * </pre>
 *
 * @param source New clock source setting
 * @see getClockSource()
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_CLKSEL_BIT
 * @see MPU6050_PWR1_CLKSEL_LENGTH
 */
void MPU6050setClockSource(uint8_t source) {
    I2CdevwriteBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see getFullScaleRange()
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_RA_GYRO_CONFIG
 * @see MPU6050_GCONFIG_FS_SEL_BIT
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
void MPU6050setFullScaleGyroRange(uint8_t range) {
    I2CdevwriteBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

/** Set full-scale accelerometer range.
 * @param range New full-scale accelerometer range setting
 * @see getFullScaleAccelRange()
 */
void MPU6050setFullScaleAccelRange(uint8_t range) {
    I2CdevwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

/** Set sleep mode status.
 * @param enabled New sleep mode enabled status
 * @see getSleepEnabled()
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_SLEEP_BIT
 */
void MPU6050setSleepEnabled(bool enabled) {
    I2CdevwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}





//////////////////////
/** Read multiple bytes from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register regAddr to read from
 * @param length Number of bytes to read
 * @param data Buffer to store read data in
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2CdevreadTimeout)
 * @return Number of bytes read (-1 indicates failure)
 */
int8_t I2CdevreadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout) {
    int8_t count = 0;
    uint32_t t1 = millis();
    
    // Arduino v1.0.1+, Wire library
    // Adds official support for repeated start condition, yay!
    
    // I2C/TWI subsystem uses internal buffer that breaks with large data requests
    // so if user requests more than BUFFER_LENGTH bytes, we have to do it in
    // smaller chunks instead of all at once
    for (uint8_t k = 0; k < length; k += min(length, BUFFER_LENGTH)) {
        Wire.beginTransmission(devAddr);
        Wire.write(regAddr);
        Wire.endTransmission();
        Wire.beginTransmission(devAddr);
        Wire.requestFrom(devAddr, (uint8_t)min(length - k, BUFFER_LENGTH));
        
        for (; Wire.available() && (timeout == 0 || millis() - t1 < timeout); count++) {
            data[count] = Wire.read();

        }
        
        Wire.endTransmission();
    }

    // check for timeout
    if (timeout > 0 && millis() - t1 >= timeout && count < length) count = -1; // timeout

    return count;
}

/** Read single byte from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param data Container for byte value read from device
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2CdevreadTimeout)
 * @return Status of read operation (true = success)
 */
int8_t I2CdevreadByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout) {
    return I2CdevreadBytes(devAddr, regAddr, 1, data, timeout);
}


/** Write multiple bits in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 * @return Status of operation (true = success)
 */
bool I2CdevwriteBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t b;
    if (I2CdevreadByte(devAddr, regAddr, &b) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
        return I2CdevwriteByte(devAddr, regAddr, b);
    } else {
        return false;
    }
}

/** Write multiple bytes to an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register address to write to
 * @param length Number of bytes to write
 * @param data Buffer to copy new data from
 * @return Status of operation (true = success)
 */
bool I2CdevwriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data) {
    uint8_t status = 0;
    Wire.beginTransmission(devAddr);
    Wire.write((uint8_t) regAddr); // send address
    for (uint8_t i = 0; i < length; i++) {
        Wire.write((uint8_t) data[i]);
    }
    status = Wire.endTransmission();
    return status == 0;
}


/** Write single byte to an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param data New byte value to write
 * @return Status of operation (true = success)
 */
bool I2CdevwriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
    return I2CdevwriteBytes(devAddr, regAddr, 1, &data);
}

bool I2CdevwriteBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data) {
    uint8_t b;
    I2CdevreadByte(devAddr, regAddr, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return I2CdevwriteByte(devAddr, regAddr, b);
}
