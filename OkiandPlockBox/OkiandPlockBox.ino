#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

#define SERVOMIN  150
#define SERVOMAX  600

int strobe = 4; // strobe pins on digital 4
int res = 5; // reset pins on digital 5
int left[7]; // store band values in these arrays
int right[7];

// Increasing the number of readings to take an average from
const int numReadings = 12;
int leftReadings[numReadings];
int rightReadings[numReadings];
int readIndex = 0;
int l_total = 0;
int r_total = 0;
int l_average = 0;
int r_average = 0;


int band;
int val;

int eyemin = map(120, 0, 180, SERVOMIN, SERVOMAX);
int eyemax = map(60, 0, 180, SERVOMIN, SERVOMAX);
int eyenatural = map(90, 0, 180, SERVOMIN, SERVOMAX);

int mouthmin = map(90, 0, 180, SERVOMIN, SERVOMAX);
int mouthmax = map(20, 0, 180, SERVOMIN, SERVOMAX);
int mouthnatural = map(60, 0, 180, SERVOMIN, SERVOMAX);

int tachemin = map(50, 0, 180, SERVOMIN, SERVOMAX);
int tachemax = map(25, 0, 180, SERVOMIN, SERVOMAX);
int tachenatural = map(37, 0, 180, SERVOMIN, SERVOMAX);

int boxopen = map(90, 0, 180, SERVOMIN, SERVOMAX);
int boxclosed = map(20, 0, 180, SERVOMIN, SERVOMAX);

uint8_t leftEye = 0;
uint8_t rightEye = 1;
uint8_t okiMouth = 2;
uint8_t plockLeftTache = 3;
uint8_t plockRightTache = 4;
uint8_t plockBox = 5;

unsigned long lastPos = eyemax;
String state;
int pos = eyemax; 

unsigned long lastUpdate;
unsigned long moveUpdate;
unsigned long updateInterval = 2000;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void readMSGEQ7() {
 digitalWrite(res, HIGH);
 digitalWrite(res, LOW);
 for(band=0; band <7; band++) {
   digitalWrite(strobe,LOW);
   // Increasing the delay in the MSGEQ7 readings
   delay(100);
   left[band] = analogRead(0);
   right[band] = analogRead(1); // ... and the right
   digitalWrite(strobe,HIGH); 
 }
}

class Mouth
{
  int mouth_interval;  

 public:
  Mouth(int interval)
  {
    mouth_interval = 1000;    
  }

  void Update() {
    unsigned long lUpdate;
    unsigned long uInterval = 5000;
    
    if((millis() - lUpdate) > uInterval)  // time to update
    {
      lUpdate = millis();
      readMSGEQ7();
      String l;
      String r;
      l_total = l_total-leftReadings[readIndex];
      r_total = r_total-rightReadings[readIndex];

      int le = 0;
      int i = 0;
      for (i = 0; i < readIndex; i++) {
         le += leftReadings[i];
        }

      int ri = 0;
      int x = 0;
      for (x = 0; x < readIndex; x++) {
         ri += rightReadings[x];
        }

     bool left_state = false;
     bool right_state = true;

      for (band = 0; band < 7; band++) {
         // Decreasing the sensitivity
         if (left[band] > right[band] && left[band] > 180) {
            left_state = true;
            right_state = false;
            l = left[band];            
            int val = map(l.toInt(), 150, 1200, 0, 180);
            int lservopos = map(val, 0, 180, SERVOMIN, SERVOMAX);
            leftReadings[readIndex] = lservopos;           
            
            readIndex = readIndex+1;
    
            if (readIndex >= numReadings) {
              readIndex = 0;
              }

            // Decreasing the sensitivity
          } else if (right[band] > left[band] && right[band] > 150) {
            left_state = false;
            right_state = true;
            r = right[band];
            int rval = map(r.toInt(), 150, 800, 0, 180);
            int rservopos = map(rval, 0, 60, SERVOMIN, SERVOMAX);
            rightReadings[readIndex] = rservopos;           
            
            readIndex = readIndex+1;
    
            if (readIndex >= numReadings) {
              readIndex = 0;
              }
          }
      }

      readIndex = readIndex+1;
    
      if (readIndex >= numReadings) {
        readIndex = 0;
        }

      l_average = le/numReadings;
      r_average = ri/numReadings;


      // Decreasing the sensitivity
      if (left_state && l_average > 190) {
        int new_val = map(l_average, 100, 500, mouthmin - 10, mouthmax);        
        pwm.setPWM(okiMouth, 0, l_average);
        pwm.setPWM(plockLeftTache, 0, tachenatural); 
        pwm.setPWM(plockRightTache, 0, tachemin);
        // Decreasing the sensitivity
      } else if (right_state && r_average > 160) {
        int smaller = map(r_average, 0, 700, tachemin, tachemax);
        int smaller2 = map(700-r_average, 0, 700, tachemin, tachemax);
        pwm.setPWM(plockLeftTache, 0, smaller2); 
        pwm.setPWM(plockRightTache, 0, smaller);
        pwm.setPWM(okiMouth, 0, eyemin);    
      } else {
        pwm.setPWM(okiMouth, 0, eyemin);
      }
    }
  }
};

class Sweeper
{
  int increment;       
  int updateInterval2;     
  
public: 
  Sweeper(int interval)
  {
    updateInterval2 = interval;
    increment = 1;
  }

  void Up() {
    unsigned long myUpdate;
    unsigned long moveUpdate = 3000;
    if((millis() - myUpdate) > moveUpdate) {
      myUpdate = millis();
      // eyemax + 10 widens the range on the eye
      for (uint16_t pulselen = eyenatural; pulselen > eyemax + 10; pulselen--) {
        pwm.setPWM(leftEye, 0, pulselen);
        pwm.setPWM(rightEye, 0, pulselen);
      }
    }  
  }
  
  void Down() {
    unsigned long myUpdate;
    unsigned long moveUpdate = 3000;
    if((millis() - myUpdate) > moveUpdate) {
      myUpdate = millis();
      // Eyemin - 10 fixes the shake
      for (uint16_t pulselen = eyemax; pulselen < eyemin - 10; pulselen++) {
        pwm.setPWM(leftEye, 0, pulselen);
        pwm.setPWM(rightEye, 0, pulselen);
      }
    } 
  }

  void Return() {
    unsigned long myUpdate;
    unsigned long moveUpdate = 3000;
    if((millis() - myUpdate) > moveUpdate) {
      myUpdate = millis();
      for (uint16_t pulselen = eyemin; pulselen < eyenatural; pulselen++) {
        pwm.setPWM(leftEye, 0, pulselen);
        pwm.setPWM(rightEye, 0, pulselen);
      } 
    }
  }
  
  void Update()
  {
    if((millis() - lastUpdate) > updateInterval2)  // time to update
    {
      lastUpdate = millis();
      Up();
      Down();
      Return();
    }
  }
};
 
Sweeper oki_eyes(random(2000, 8000));
Mouth oki_mouth(1000);
 
void setup() 
{ 
  Serial.begin(9600);
  mySwitch.enableReceive(0);
  pwm.begin();
  pwm.setPWMFreq(60);
  pinMode(res, OUTPUT); // reset
  pinMode(strobe, OUTPUT); // strobe
  digitalWrite(res,LOW); // reset low
  digitalWrite(strobe,HIGH); //pin 5 is RESET on the shield
  yield();

  pwm.setPWM(leftEye, 0, eyenatural);
  pwm.setPWM(rightEye, 0, eyenatural);
  pwm.setPWM(okiMouth, 0, eyemin);
  pwm.setPWM(plockLeftTache, 0, tachenatural); 
  pwm.setPWM(plockRightTache, 0, tachenatural);
  pwm.setPWM(plockBox, 0, boxclosed);

  for (int thisReading = 0; thisReading < numReadings; thisReading++){
    leftReadings[thisReading] = 0;
    rightReadings[thisReading] = 0;
  }
} 
 
 
void loop() 
{      
     oki_eyes.Update();
     oki_mouth.Update();
   
}
