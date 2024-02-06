#include <stdint.h>

typedef struct {
  char name[21];
  char types[21]; // i - 32bit int, f - 32bit float, b - boolean (32 bit) 
} SensorInfo;

class Spine_
{
public:
  Spine_();
  void begin();
  void write(uint8_t sensorID, uint8_t valueID, float value);
  void send();
  int configSensor(uint8_t sensorID, const char *name, const char *types);

protected:
  uint8_t hidBuffer[61];
  uint8_t hidMsgCounter;
  
  const uint8_t SEND_VALUES = 0;
  const uint8_t SEND_INFO = 1;
  SensorInfo sensorInfo[17];
  void sendInfo();
  unsigned long timeInfo; 
};

extern Spine_ Spine;

