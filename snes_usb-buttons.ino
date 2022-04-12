// change pins to match your implementation, colours can vary with extensions
const byte dataPin = 21;
const byte latchPin = 22;
const byte clockPin = 23;

const byte buttonCount = 12;

// set in the order of SNES controller spec
const int* joystickButtons[] = {1, 2, 3, 4,
                                5, 6, 7, 8,
                                9, 10, 11, 12
                                };                  

bool* currentPress = 0;
bool* buttonStatus[] = {0,0,0,0,
                        0,0,0,0,
                        0,0,0,0
                       };

// override yield()
void yield(){
  digitalToggleFast(0);
}

void setup() {

  // set pin modes to SNES controller spec
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, INPUT);

  // set USB to manual send mode
  Joystick.useManualSend(true);
}

void loop()
{
  // replicate SNES controller communications protocol
  digitalWriteFast(latchPin, HIGH);
  // testing showed this wasn't needed, left as comment for controller protocol reference
  //delayMicroseconds(12);
  delayMicroseconds(5);
  digitalWriteFast(latchPin, LOW);
  // testing showed this wasn't needed, left as comment for controller protocol reference
  //delayMicroseconds(6);

  for (byte i = 0; i < buttonCount; i++)
  {
    // digitalWriteFast on clock pin was too fast for the controller IC to report correctly
    // more controller protocol
    digitalWrite(clockPin, LOW);

    // testing showed any lower than 1µs and errors occur from the controller IC, protocol is 6µs
    delayMicroseconds(1);

    // save current pin state
    currentPress = digitalReadFast(dataPin);
    
    // only update the button if state has changed
    if (currentPress != buttonStatus[i]) {
      Joystick.button(joystickButtons[i], !currentPress);
      buttonStatus[i] = currentPress;
    }

    // final part of controller protocol
    digitalWrite(clockPin, HIGH);
  }

  // send collected button states all at once over USB
  Joystick.send_now();
}
