/*
 * libscale.cpp
 *
 * Created: 13.04.2020 15:48:35
 *  Author: GuenzOne
 */ 

#include "libscale.h"
#include "HX711.h"
#include <Preferences.h>
#define BUTTON 12              // Pin Define for a Button to set the scale to Tare

HX711 scale;                  // setting HX711 as scale
Preferences preferences;     // Methode for saving Tare Results

const int LOADCELL_DOUT_PIN = 35; // Define for HX711 Scale Pins
const int LOADCELL_SCK_PIN = 32; // Define for HX711 Scale Pins
int buttonState = 0;            // Define for tara button
bool buttonPressed = false;    // Define for state of tara button
unsigned long delayTime;    // delay time for HX711



RTC_DATA_ATTR float Tare;               // defines variable for tare value
RTC_DATA_ATTR float rawweight;            // defines variable for weight

void taring ()
{
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);  // beginig scale hx711 connection with pins from define
  //scale.tare();  //Reset the scale to 0 (hx711 function but not helping here)
  /* Scalefactor of the hx711+scale(5kg) 
  (green, no name, no datasheet, china board)
   -> Formula : scalefactor = (output*gain*ADCRange*VoltATScale)/Scalerange*VCC)
  858993.408(calculated for normal sparkfun hx711 with 5kg loadcell)  
  841753.44(adjusted value for green china board) */
  
  pinMode(BUTTON, INPUT_PULLUP);       // define mode for tara button
  scale.set_scale(841753.44);                        
  delayTime = 1000;
  scale.power_up(); //HX711 ADC booting
  Serial.println("Zum Tarieren druecke den Tara Knopf fuer 5 Sekunden");
  delay(2000);  
  buttonState = digitalRead(BUTTON); //reads state of button
  delay(100);
  Serial.println(buttonState);  // prints zero if button is pressed, otherwise 1
      if (buttonState == LOW)
        {
          Serial.println("Tara Start");
          Tare = (scale.get_units(10)); // saves average from 10 measurements
          
          Serial.println(Tare);         // tells the value auf tare
          preferences.begin("Tara", false);   // open folder on esp to save tare value to be reset safe
          preferences.putFloat("TaraGewicht", Tare);    // saving tare value
          preferences.end();   // closing folder
          Serial.println("Tara Ende");
          scale.power_down();             // put the ADC in sleep mode
        }
        
}                            //end void button

void weight()
{
	
	scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);  // beginig scale hx711 connection with pins from define
	Serial.println("Wiegen Start");
	scale.set_scale(841753.44);                        // Scalefactor of the scale
	delayTime = 1000;
	scale.power_up(); //HX711 ADC booting
	preferences.begin("Tara", false);   // opening a folder for saving and writing tara
	Tare = preferences.getFloat("TaraGewicht", 0);  // den Geschützten Wert aufrufen und in der Variablen Tare speichern
	rawweight = scale.get_units(10);                //saves measured rawweight (avg. of 10)
	Serial.print("Gemessenes Gewicht: ");
	Serial.print((rawweight *2 - Tare *2)*1000, 2);  // conversion of raw weight in kg to real weight in gram with 2 decimals
	Serial.println("g");
	preferences.end();                // close folder
	Serial.println("Wiegen Ende");
	scale.power_down();             // put the ADC in sleep mode
	
}                  //end void wiegen