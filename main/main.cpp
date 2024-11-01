//#include <Arduino.h>
#include "UI.h"

UI menu;

void key_Event();

void setup()
{
  // put your setup code here, to run once:
  // Serial.begin(115200);
  /* Begining of UI library this Function initializes the MPU9250, BMM150, SSD1306, CH376s and get
  the data saved in the EEPROM. */
  menu.begin();
  // This  Function Print the Logo of the company
  menu.logo();
  // Setting up the pins in microcontroller which are connected to keys
  menu.keysetUp();
  Timer1.initialize(10000);
  Timer1.attachInterrupt(key_Event);
}

void loop()
{ 
  // put your main code here, to run repeatedly:
  uint8_t read = digitalRead(A1);
  if (read == LOW)
  {
    menu.resolveInputFlags();
    menu.display();
    menu.lidarData_display();
    menu.imuData_display();
    menu.coordinate_display();
    menu.lengthDisplay();
    menu.flashDataToDrive();
  }
  if (read == HIGH)
  {
    menu.charge_Indicator();
  }
}

void key_Event()
{
  menu.setInputFlags();
}
