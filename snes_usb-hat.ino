#define center -1
#define up 0
#define down 180
#define left 270
#define right 90
#define upRight 45
#define upLeft 315
#define downLeft 225
#define downRight 135

// change pins to match your implementation, colours can vary with extensions
// https://www.repairfaq.org/REPAIR/F_SNES.html
// yellow
const byte clockPin = 23;
// red
const byte dataPin = 21;
//orange
const byte latchPin = 22;

const byte buttonCount = 12;

// set in the order of SNES controller spec
const byte* joystickButtons[] = {1, 2, 3, 4,
                                 5, 6, 7, 8,
                                 9, 10, 11, 12
                                };

int hatAngle = center;

bool* currentPress = 0;

bool* buttonStatus[] = {0,0,0,0,
                        0,0,0,0,
                        0,0,0,0
                       };

bool usbState = 0;

// override yield() for speed
void yield(){
  digitalToggleFast(0);
}

void setup() {
  // define pin modes
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, INPUT);

  // set USB manual send mode
  Joystick.useManualSend(true);
}

void loop()
{
  usbState = 0;
  
  // replicate SNES controller communications protocol
  digitalWriteFast(latchPin, HIGH);
  delayMicroseconds(5);
  //digitalToggleFast(0);
  digitalWriteFast(latchPin, LOW);
  // testing showed this wasn't needed, left as comment for controller protocol reference
  //delayMicroseconds(6);

  for (byte i = 0; i < buttonCount; i++){
    // more controller protocol
    digitalWriteFast(clockPin, LOW);

    // any lower than 1µs and errors occur from the controller IC, protocol is 6µs
    delayMicroseconds(1);

    currentPress = digitalReadFast(dataPin);

    // check data pin to see if the current button has changed state
    if (currentPress != buttonStatus[i]) {
      // check d-pad for hat translation
      if (i > 3 && i < 8){
        switch (i){
          // up
          case 4:
            if (currentPress == false){
              if (hatAngle == left)
                hatAngle = upLeft;
              else if (hatAngle == right)
                hatAngle = upRight;
              else
                hatAngle = up;
            }
            else{
              if (hatAngle == upLeft)
                hatAngle = left;
              else if (hatAngle == upRight)
                hatAngle = right;
              else if (hatAngle == down)
                hatAngle = down;
              else
                hatAngle = center;
            }
            break;
          // down
          case 5:
            if (currentPress == false){
              if (hatAngle == left)
                hatAngle = downLeft;
              else if (hatAngle == right)
                hatAngle = downRight;
              else
                hatAngle = down;
            }
            else{
              if (hatAngle == downLeft)
                hatAngle = left;
              else if (hatAngle == downRight)
                hatAngle = right;
              else if (hatAngle == up)
                hatAngle = up;
              else
                hatAngle = center;
            }
            break;
          // left
          case 6:
            if (currentPress == false){
              if (hatAngle == up)
                hatAngle = upLeft;
              else if (hatAngle == down)
                hatAngle = downLeft;
              else
                hatAngle = left;
            }
            else{
              if (hatAngle == upLeft)
                hatAngle = up;
              else if (hatAngle == downLeft)
                hatAngle = down;
              else if (hatAngle == right)
                hatAngle = right;
              else
                hatAngle = center;
            }
            break;
          // right
          case 7:
            if (currentPress == false){
              if (hatAngle == up)
                hatAngle = upRight;
              else if (hatAngle == down)
                hatAngle = downRight;
              else
                hatAngle = right;
            }
            else{
              if (hatAngle == upRight)
                hatAngle = up;
              else if (hatAngle == downRight)
                hatAngle = down;
              else if (hatAngle == left)
                hatAngle = left;
              else
                hatAngle = center;  
            }
            break;
        }
        // save current button state for later check
        buttonStatus[i] = currentPress;
      }
      // fill gap left by button to hat conversion
      else if (i >7){
        Joystick.button(joystickButtons[i-4], !currentPress);
        buttonStatus[i] = currentPress;
      }
      else {
        Joystick.button(joystickButtons[i], !currentPress);
        buttonStatus[i] = currentPress;
      }

      usbState = 1;
    }

    // final part of controller protocol
    digitalWrite(clockPin, HIGH);
  }
  
  // only send USB updates if there are changes
  if (usbState){
    // send collected button states all at once over USB
    Joystick.hat(hatAngle);
    Joystick.send_now();
  }
}
