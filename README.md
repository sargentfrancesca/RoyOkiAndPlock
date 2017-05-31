# RoyOkiAndPlock

This is the repository for the Roy, Oki and Plock puppet project.  Here, each respective puppet is controlled using servos.  These servos react either to audio input or received RF signals.

##Requirements
###Software
* Arduino IDE
* RCSwitch library installed to Arduino IDE
* Adafruit PWM Servo Driver Library
* Adafruit Motor Shield Library

###Hardware
* 2x Arduino Uno
* 2x 5v Power Supply (hooked up to mains, wires available for Arduino powering)
* Adafruit Motor Shield
* 7x Servos - we use Futaba S3003's
* 2x RF receiver
* Universal RF remote
* Sparkfun Spectrum Shield

###Skills/etc
* Soldering. Will require a decent soldering iron, and lead free solder.

##Roy
###Needed from Requirements
* 1x Arduino Uno
* 1x Adafruit Motor Shield
* 1x Servo
* 1x RF receiver
* 1x Power Supply
* Adafruit Motor Shield Library
* RCSwitch library

###RF Calibration
Roy requires two buttons of the remote - one to open, one to close. This functionality will change. Either one button will act as a toggle, or Roy will appear to 'blink' with one button press.

The correct button codes will need to be calibrated. To do this, open the ReceiveDemo from the rc-switch examples folder and upload to your Arduino (the transmitter must be connected). Open the serial monitor, and press the remote button to receive the codes. Note these down. For each button press, the code will be the first integer after "Received: " and before "/". 

In Roy's Arduino sketch, inside the `loop()` function, you will see where to replace these integers. One to `openEyes(9000)`, and one to `closeEyes(9000)`.

##Oki and Plock
###Needed from Requirements
* 1x Arduino Uno
* 6x Servos
* Sparkfun Spectrum Shield
* 5v Power Supply
* RF receiver
* RCSwitch library installed to Arduino IDE
* Adafruit PWM Servo Driver Library

###Box calibration
The box received RF signals to 'reveal' Plock. Again, one button to open, and one button to close, though in the future this will probably become toggled.

RF calibration is similar to Roy, and appears in the `loop()` function.
