/* PlockBox
 *  Uses RC Switch to trigger the opening mechanism for Plock's box
 *  Uses PWM to control the Servo position
 *  Button 9 on the Remote = OPENCLOSE (-2621)
*/

#include <Servo.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

Servo myservo;

int pos = 0;

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  mySwitch.enableReceive(0);
  myservo.write(0);
}

void loop() {
    
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    if (value == -2621) {
      myservo.writeMicroseconds(1000);
      delay(1000);
      myservo.writeMicroseconds(2200);
      delay(1000); 
    } 

    mySwitch.resetAvailable();
  }

}

