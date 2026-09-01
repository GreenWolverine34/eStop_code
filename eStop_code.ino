#include <HijelHID_BLEKeyboard.h>
#include <WiFi.h>

HijelHID_BLEKeyboard bleKeyboard("Robotics Button", "Custom", 67);

const int estopPin = 4;
const int switchPin = 14;

bool debounce = false;
unsigned long waitTime = 40;

void setup() {
  Serial.begin(115200);
  
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
  pinMode(estopPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);
  
  bleKeyboard.begin();
}

void loop() {
  bool switchPressed = (digitalRead(switchPin) == LOW);
  bool estopPressed = (digitalRead(estopPin) == HIGH);

  if ((switchPressed || estopPressed) && !debounce) {
    unsigned long windowStart = millis();
    bool dualPressConfirmed = false;

    while (millis() - windowStart < waitTime) {
      if (digitalRead(switchPin) == LOW && digitalRead(estopPin) == HIGH) {
        dualPressConfirmed = true;
        break;
      }
    }

    if (dualPressConfirmed) {
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

  if (digitalRead(switchPin) == HIGH && digitalRead(estopPin) == LOW) {
    debounce = false;
  }
}
