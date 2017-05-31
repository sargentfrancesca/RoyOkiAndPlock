/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  mySwitch.enableReceive(0);
  myservo.write(0);
}

void openEyes(int interval) {
  unsigned long myUpdate;
  unsigned long moveUpdate = interval;
  if((millis() - myUpdate) > moveUpdate) {
    myUpdate = millis();
      for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'                       // waits 15ms for the servo to reach the position
        delay(10);
      }      
  }
}

void closeEyes(int interval) {
  unsigned long myUpdate;
  unsigned long moveUpdate = interval;
  if((millis() - myUpdate) > moveUpdate) {
    myUpdate = millis();
      for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'                      // waits 15ms for the servo to reach the position
        delay(10);
      }
  }
}

void loop() {
  
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();

    if (value == -28533) {
      Serial.println(value);
      openEyes(9000);
    } 

    if (value == -28534) {
      Serial.println(value);
      closeEyes(9000);
    }
//    else {
//      Serial.print("Received ");
//      Serial.print( mySwitch.getReceivedValue() );
//      Serial.print(" / ");
//      Serial.print( mySwitch.getReceivedBitlength() );
//      Serial.print("bit ");
//      Serial.print("Protocol: ");
//      Serial.println( mySwitch.getReceivedProtocol() );
//    }

    mySwitch.resetAvailable();
  }

}

