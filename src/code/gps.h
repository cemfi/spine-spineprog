uint8_t buffer[82]; // max length of text message = 80 + cr lf
int count = 0;
void clearBufferArray();


void setup()
{
    Serial.begin(115200);
    Serial1.begin(9600);
}

bool isGPRMC() {
  const char *GPRMC = "$GPRMC";
  int i;
  for (i = 0; i < 6; i++) {
    if (buffer[i] != GPRMC[i]) {
      return false;
    }
  }
  return true;
}

float bufferToFloat(uint8_t start, uint8_t count) {
  char tmp[80]; // max length of entire NMEA sentence
  int i;
  for (i = start; i < start + count; i++) {
    tmp[i - start] = buffer[i];
  }
  tmp[i] = 0; // null-terminate string
  return atof(tmp);  
}

void loop()
{
    if (Serial1.available())
    {
        while(Serial1.available())
        {
            buffer[count] = Serial1.read();
            Serial.write(buffer[count]);
            if (buffer[count] == '\n') {
              
              //if (isGPGLL()) {
              if (isGPRMC()) {
                // GSV GSA GGA VTG RMC messages are also received
                uint8_t commaIndices[7];
                int i;
                int current = 0;
                for (i = 0; i < count && current < 7; i++) {
                  if (buffer[i] == ',') {
                    commaIndices[current] = i;
                    current++;
                  }
                }
                
                /*
                Serial.print(bufferToFloat(commaIndices[0] + 1, commaIndices[1] - commaIndices[0]), 5);
                Serial.print(" N, ");
                Serial.print(bufferToFloat(commaIndices[2] + 1, commaIndices[3] - commaIndices[2]), 5);
                Serial.print(" E");
                Serial.print(" @ ");               
                Serial.print(bufferToFloat(commaIndices[4] + 1, commaIndices[5] - commaIndices[4]), 2);
                Serial.println();
                */
                
                Spine.write(connector, 0, bufferToFloat(commaIndices[0] + 1, commaIndices[1] - commaIndices[0]));
                Spine.write(connector, 1, bufferToFloat(commaIndices[2] + 1, commaIndices[3] - commaIndices[2]));
                Spine.write(connector, 2, bufferToFloat(commaIndices[4] + 1, commaIndices[5] - commaIndices[4]));
              }             
              count = 0;
            }
            else {
              count++;
            }
        }
    }
}

