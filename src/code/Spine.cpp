#include "Spine.h"
#include <HID.h>

static const u8 _hidReportDescriptor[] PROGMEM = {
    0x06, 0xC0, 0xFF,  // 30
    0x0A, 0x00, 0x0C,
    
    0xA1, 0x01,         // Collection 0x01
    0x85, 0x03,         // REPORT_ID (3)
    0x75, 0x08,         // report size = 8 bits
    0x15, 0x00,         // logical minimum = 0
    0x26, 0xFF, 0x00,   // logical maximum = 255
    
    0x95, 61,         // report count TX
    0x09, 0x01,       // usage
    0x81, 0x02,       // Input (array)
    
    // 0x95, 5,           // report count RX
    // 0x09, 0x02,        // usage
    // 0x91, 0x02,        // Output (array)
    0xC0              // end collection
};

Spine_ Spine;

union
{
  byte b[4];
  float f;
} data;

Spine_::Spine_()
{
  int i;
  for (i = 0; i < 17; i++) {
    sensorInfo[i].name[0] = sensorInfo[i].types[0] = 0;
  }
}

void Spine_::begin() 
{
  hidMsgCounter = 0;
    
  HID().begin();
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  HID().AppendDescriptor(&node);

  timeInfo = millis();
  sendInfo();
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
  
  if (hidMsgCounter == 10) {
    hidBuffer[0] = SEND_VALUES;
    send();
    hidMsgCounter = 0;
    if (millis() - timeInfo > 500) {
      timeInfo = millis();
      sendInfo();
    }
  }
}

void Spine_::send()
{
  HID().SendReport(3, hidBuffer, 61);
}

int Spine_::configSensor(uint8_t sensorID, const char *name, const char *types) {
  if (sensorID >= 17 || sensorID < 0){
    return -1; // not OK
  }
  if (name == NULL || types == NULL){
    return -1;
  }
  if (strlen(name) > 20 || strlen(types) > 20) {
    return -1;
  }
  
  strcpy(sensorInfo[sensorID].name, name);
  strcpy(sensorInfo[sensorID].types, types);
  return 0; // OK
}

void Spine_::sendInfo() {
  int i;
  for (i = 0; i < 17; i++) {
    if (strlen(sensorInfo[i].name) != 0 && strlen(sensorInfo[i].types) != 0) {
      int lengthName = strlen(sensorInfo[i].name) + 1;
      int lengthTypes = strlen(sensorInfo[i].types) + 1;
      hidBuffer[1] = i;
      memcpy((void *)hidBuffer + 2, sensorInfo[i].name, lengthName);
      memcpy((void *)hidBuffer + 2 + lengthName, sensorInfo[i].types, lengthTypes);
      hidBuffer[0] = SEND_INFO;
      send(); 
    }
  }
}



