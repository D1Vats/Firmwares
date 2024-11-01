#include <Arduino.h>
#include <Wire.h>

// keyboard Constants
#define KEYBOARD_ADDRESS 0x34
#define KEYBOARD_INT_PIN 2
#define KEYBOARD_EVENT_REGISTER 0x04
#define KEYBOARD_FLAG_REGISTER 0x02
#define KEYBOARD_KEY_DOWN 0x80
#define KEYBOARD_KEY_MASK 0x7F

const uint8_t KEYBOARD_CONFIG_COMMANDS[] = {0x1D, 0xFF, 0x1E, 0xFF, 0x1F, 0x03, 0x01, 0xB9, 0x02, 0x0F};

void keyboard_configure(void);
void keyboard_clearFlag(void);
uint8_t keyboard_getState(void);
void keyboard_ISR(void);

bool keyInt = true;
int buzzer = 13;
bool trigger;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  keyboard_configure();
  pinMode(buzzer, OUTPUT);
  pinMode(KEYBOARD_INT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(KEYBOARD_INT_PIN), keyboard_ISR, FALLING);
}

void loop()
{
  if (keyInt)
  {
    uint8_t key_code = keyboard_getState();
    //Serial.println(key_code);
    if (key_code & KEYBOARD_KEY_DOWN)
    {
      switch (key_code & KEYBOARD_KEY_MASK)
      {
      case 58:
        while (key_code != 58)
        {
          Serial.println("58");
          TCCR0B = TCCR0B & (B11111000 | B00000010);
          analogWrite(buzzer, 180);
          key_code = keyboard_getState();
        }
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 0);
        // Serial.println("58");
        break;
      case 59:
        Serial.println("59");
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 180);
        delay(100);
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 0);
        break;
      case 60:
        Serial.println("60");
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 180);
        delay(100);
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 0);
        break;
      case 68:
        Serial.println("68");
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 180);
        delay(100);
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 0);
        break;
      case 69:
        Serial.println("69");
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 180);
        delay(100);
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 0);
        break;
      case 70:
        Serial.println("70");
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 180);
        delay(100);
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 0);
        break;
      case 78:
        Serial.println("Vashishtha Research");
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 180);
        delay(100);
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 0);
        break;
      case 79:
        Serial.println("TCA8418");
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 180);
        delay(100);
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 0);
        break;
      case 80:
        Serial.println("This is It.");
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 180);
        delay(100);
        TCCR0B = TCCR0B & (B11111000 | B00000010);
        analogWrite(buzzer, 0);
        break;
      default:
        break;
      }
      keyboard_clearFlag();
    }
    delay(100);
  }
}

void keyboard_configure(void)
{
  for (uint8_t i = 0; i < 9; i += 2)
  {
    Wire.beginTransmission(KEYBOARD_ADDRESS);
    Wire.write(KEYBOARD_CONFIG_COMMANDS[i]);
    Wire.write(KEYBOARD_CONFIG_COMMANDS[i + 1]);
    Wire.endTransmission();
  }
}
uint8_t keyboard_getState(void)
{
  uint8_t key;
  Wire.beginTransmission(KEYBOARD_ADDRESS);
  Wire.write(KEYBOARD_EVENT_REGISTER);
  Wire.endTransmission();
  Wire.requestFrom(KEYBOARD_ADDRESS, 1);
  while (Wire.available())
  {
    key = Wire.read();
  }
  return key;
}
void keyboard_clearFlag(void)
{
  Wire.beginTransmission(KEYBOARD_ADDRESS);
  Wire.write(KEYBOARD_FLAG_REGISTER);
  Wire.write(0x0F);
  Wire.endTransmission();
}
void keyboard_ISR(void) {
 keyInt = true;
}
