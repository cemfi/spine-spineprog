 
inline void writeI2C(uint8_t _register, uint8_t _data)
{
  ::Wire.begin();
  ::Wire.beginTransmission(0x4C);
  ::Wire.write(_register);
  ::Wire.write(_data);
  ::Wire.endTransmission();
}


inline uint8_t read(uint8_t _register)
{
  uint8_t data_read;
  ::Wire.begin();
  ::Wire.beginTransmission(0x4C);
  ::Wire.write(_register);
  ::Wire.endTransmission();
  ::Wire.beginTransmission(0x4C);
  ::Wire.requestFrom(0x4C, 1);
  while (::Wire.available())
  {
    data_read = ::Wire.read();
  }
  ::Wire.endTransmission();
  return data_read;
}


inline void getXYZ(int8_t *x, int8_t *y, int8_t *z)
{
  while (::Wire.available() > 0)
    ::Wire.read();
  ::Wire.requestFrom(0x4C, 3);
  if (::Wire.available())
  {
  *x = ((char)(::Wire.read() << 2)) / 4;
  *y = ((char)(::Wire.read() << 2)) / 4;
  *z = ((char)(::Wire.read() << 2)) / 4;
  }
}


inline void getAcceleration(float *ax, float *ay, float *az)
{
  int8_t x, y, z;
  getXYZ(&x, &y, &z);
  *ax = x / 21.00;
  *ay = y / 21.00;
  *az = z / 21.00;
}


void setup()
{
  writeI2C(0x07, 0x00); // STAND BY
  writeI2C(0x08, 0x00); // 120 SPS
  writeI2C(0x07, 0x01); // ACTIVE
  
  Serial.begin(9600);
}


void loop()
{
  float ax,ay,az;
  getAcceleration(&ax,&ay,&az);
  
  write(0, ax);
  //Serial.println(ax);
  
  write(1, ay);
  //Serial.println(ay);
  
  write(2, az);
  Serial.println(az);
}
