#include "Spine.h"
#include <HID.h>

static const u8 _hidReportDescriptor[] PROGMEM = {
    0x06, 0xC0, 0xFF,  // 30
    0x0A, 0x00, 0x0C,
    
    0xA1, 0x01,       // Collection 0x01
    0x85, 0x03,         // REPORT_ID (3)
    0x75, 0x08,       // report size = 8 bits
    0x15, 0x00,       // logical minimum = 0
    0x26, 0xFF, 0x00,   // logical maximum = 255
    
    0x95, 61,       // report count TX
    0x09, 0x01,       // usage
    0x81, 0x02,       // Input (array)
    
    // 0x95, 5,       // report count RX
    // 0x09, 0x02,        // usage
    // 0x91, 0x02,        // Output (array)
    0xC0            // end collection
};

Spine_ Spine;

static u8 hidBuffer[61];
static u8 hidMsgCounter;
static u8 spineID;

union
{
	byte b[4];
	float f;
} data;

Spine_::Spine_(void)
{
}

void Spine_::begin(u8 ID) 
{
	spineID = ID;
	hidBuffer[0] = spineID;
	hidMsgCounter = 0;
    
    HID().begin();
    static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
    HID().AppendDescriptor(&node);
}

void Spine_::write(uint8_t sensorID, uint8_t valueID, float value)
{
	u8 offset = hidMsgCounter * 6 + 1;
    
    hidBuffer[offset] = sensorID;
    hidBuffer[++offset] = valueID;
	data.f = value;
	
	hidBuffer[++offset] = data.b[0];
	hidBuffer[++offset] = data.b[1];
	hidBuffer[++offset] = data.b[2];
	hidBuffer[++offset] = data.b[3];
	hidMsgCounter++;
	
	hidBuffer[0] = (hidMsgCounter << 4) | (spineID & 0x0F);
	if (hidMsgCounter == 10)
		flush();
}

void Spine_::flush(void)
{
    HID().SendReport(3, hidBuffer, 61);
	hidBuffer[0] = spineID;
	hidMsgCounter = 0;	
}
