#include <HijelHID_BLEKeyboard.h>

HijelHID_BLEKeyboard bleKeyboard("Robotics Button", "Custom", 67);

const int enablePin = 4;  
const int disablePin = 14; 
bool debounce = false;
unsigned long waitTime = 40; 

void setup() {
  Serial.begin(115200);
  //Pins are opposite of a keyboard switch; normally connected instead of normally disconnected
  pinMode(enablePin, INPUT_PULLUP);
  pinMode(disablePin, INPUT_PULLUP);
  
  bleKeyboard.begin();
  Serial.println("BLE Keyboard Started. Advertising...");
}

void loop() {
  if (!bleKeyboard.isConnected()) {
    delay(100);
    return;
  }

  bool enableSwitchPressed = (digitalRead(enablePin) == HIGH);
  bool disableSwitchPressed = (digitalRead(disablePin) == HIGH);

  if ((enableSwitchPressed || disableSwitchPressed) && !debounce) {
    unsigned long windowStart = millis();
    bool dualPressConfirmed = false;

    while (millis() - windowStart < waitTime) {
      if (enableSwitchPressed && disableSwitchPressed) {
        dualPressConfirmed = true;
        break;
      }
      delay(2); //some error happens apparently with esp32s if u dont have a delay
    }

    if (dualPressConfirmed) {
      bleKeyboard.press(KEY_LEFTBRACE);
      bleKeyboard.press(KEY_RIGHTBRACE);
      bleKeyboard.press(KEY_BACKSLASH);
    } else {
      bleKeyboard.press(KEY_RETURN);
    }

    debounce = true;
    delay(100); 
    bleKeyboard.releaseAll();
  }
  if (!enableSwitchPressed && !disableSwitchPressed) {
    debounce = false;
  }
}
