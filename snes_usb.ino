// change pins to match your implementation, colours can vary with extensions
const byte clockPin = 23;
const byte dataPin = 21;
const byte latchPin = 22;

long buttonData, lastButtonData = 0;

const byte j_buttons = 13;
const byte n_buttons = 12;

unsigned long start = 0;

// set in the order of SNES controller spec - not required, just because
const int* joystickButtons[] = {1, 2, 3, 4,
                                5, 6, 7, 8,
                                9, 10, 11, 12
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
  buttonData = 0;

  // replicate SNES controller communications protocol
  digitalWriteFast(latchPin, HIGH);
  delayMicroseconds(12);
  digitalWriteFast(latchPin, LOW);
  // testing showed this wasn't needed, left as comment for controller protocol reference
  //delayMicroseconds(6);

  for (byte i = 0; i < n_buttons; i++)
  {
    // digitalWriteFast on clock pin was too fast for the controller IC to report correctly
    // more controller protocol
    digitalWrite(clockPin, LOW);

    // testing showed any lower than 1µs and errors occur from the controller IC, protocol is 6µs
    delayMicroseconds(1);

    // check data pin to see if there are pressed buttons
    if (!digitalReadFast(dataPin)) {
      buttonData = buttonData | ((long)1 << i);
    }

    // final part of controller protocol
    digitalWrite(clockPin, HIGH);
  }

  // if buttons are being held down, don't reset them
  if (buttonData != lastButtonData){ 
    for (byte i = 1; i < j_buttons; i++)
    {
      Joystick.button(i, 0);
    }
    Joystick.send_now();
  }
  
  lastButtonData = buttonData;

  // collect pressed button states
  for (byte i = 0; i < n_buttons; i++)
  { 
    // iterate over the binary string   
    if (buttonData & (1 << i) ) {
      Joystick.button(joystickButtons[i], 1);
    }
  }

  // send collected button states all at once over USB
  Joystick.send_now();
}
