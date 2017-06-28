/* Roy
 *  Uses RCSwitch to Open or Close Roy's eyes.
 *  Button 11 on the Remote = OPEN (-2609)
 *  Button 12 on the Remote = CLOSE (-2576)
*/

#include <Servo.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

Servo myservo; 
String state = "closed";
int pos = 0;

void closeEyes(int interval) {
  unsigned long myUpdate;
  unsigned long moveUpdate = interval;
  if((millis() - myUpdate) > moveUpdate) {
    myUpdate = millis();
      for (pos = 0; pos <= 30; pos += 1) { // goes from 0 degrees to 180 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'                       // waits 15ms for the servo to reach the position
        delay(15);
      }      
  }
}

void openEyes(int interval) {
  unsigned long myUpdate;
  unsigned long moveUpdate = interval;
  if((millis() - myUpdate) > moveUpdate) {
    myUpdate = millis();
      for (pos = 30; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);
        delay(15);
      }
  }
}

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  mySwitch.enableReceive(0);
  myservo.write(30);
}

void loop() {
  Serial.println(myservo.read());
//  Serial.println(state);
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    
    if (value == -2609) {
//      if (state == "open") {
        openEyes(9000);
//        state = "closed";       
        delay(500);
//      }      
    } 

    if (value == -2576) {
//      if (state == "closed") {
        closeEyes(9000);
//        state = "open";
        delay(500);
//      }
      
    }
    mySwitch.resetAvailable();
  }

}

