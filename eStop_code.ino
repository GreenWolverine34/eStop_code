#include <HijelHID_BLEKeyboard.h>

// 1. Fixed missing quotes for the device name and manufacturer strings
HijelHID_BLEKeyboard bleKeyboard("Robotics Button", "Custom", 67); 

const int estopPin = 4;
const int switchPin = 14;
bool debounce = false;
unsigned long waitTime = 40;

void setup() {
  Serial.begin(115200);
  
  // REMOVED WiFi.mode(WIFI_OFF) to prevent radio chip shutdown
  
  pinMode(estopPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);
  
  bleKeyboard.begin(); 
  Serial.println("BLE Keyboard Started. Advertising...");
}

void loop() {
  // SAFETY: Only monitor buttons if a device is actually paired and connected
  if (!bleKeyboard.isConnected()) {
    delay(100); 
    return;
  }

  bool switchPressed = (digitalRead(switchPin) == HIGH);
  bool estopPressed = (digitalRead(estopPin) == HIGH);

  if ((switchPressed || estopPressed) && !debounce) {
    unsigned long windowStart = millis();
    bool dualPressConfirmed = false;

    while (millis() - windowStart < waitTime) {
      if (digitalRead(switchPin) == HIGH && digitalRead(estopPin) == HIGH) {
        dualPressConfirmed = true;
        break;
      }
    }

    if (dualPressConfirmed) {
      // 2. Fixed character notation typos using single quotes
      bleKeyboard.press((uint8_t)'[');
      bleKeyboard.press((uint8_t)']');
      bleKeyboard.press((uint8_t)'\\'); 
    } else {
      bleKeyboard.press(KEY_RETURN);
    }

    debounce = true;
    delay(100);
    bleKeyboard.releaseAll();
  }

  if (digitalRead(switchPin) == LOW && digitalRead(estopPin) == LOW) {
    debounce = false;
  }
}
