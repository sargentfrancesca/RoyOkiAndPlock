/* PlockBox
 *  Uses RC Switch to trigger the opening mechanism for Plock's box
 *  Uses digitalWrite to control the Solenoid state
 *  Button 9 on the Remote = OPENCLOSE (-2621)
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

int solenoidPin = 9; // Declate solenoid pin

void setup() {
  Serial.begin(9600);
  pinMode(solenoidPin, OUTPUT); // Set pin as output
  mySwitch.enableReceive(0);
  digitalWrite(solenoidPin, LOW); // Runs once - ensures closed on startup
}

void loop() {
    
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    // You can open Serial (top right, magnifying glass) to detect which button code is being sent
    Serial.println(value);
    
    // This will cause the solenoid to rotate to be open, then closed straight after 
    if (value == -2621) {
      digitalWrite(solenoidPin, HIGH);
      delay(1000); // Wait for a second before closing
      digitalWrite(solenoidPin, LOW);
      delay(1000); // Wait for a second before anything else can happen
    } 

    mySwitch.resetAvailable();
  }

}

