#include <stdint.h>

class Spine_
{
public:
	Spine_(void);
	void begin(uint8_t ID);
	void write(uint8_t sensorID, uint8_t valueID, float value);
	void flush(void);
};
extern Spine_ Spine;