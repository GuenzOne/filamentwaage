#include "libscale.h"


void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  taring ();
  weight();
  delay(5000);
}
