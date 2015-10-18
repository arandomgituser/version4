#include <StopWatch.h>

/* long lastElapsed;

// StopWatch rStopwatch, lStopwatch, dsw;
//bool isRunning = false;
int dirty = 0;
*/
StopWatch dsw;
int btn1 = 2;
int btn2 = 3;
int btn3 = 4;

int btnArray[] = {0, 3, 4}; //button pins
int bed[] = {0, 9, 10, 11 }; //bed valve pins
int main[] = {0, 7, 8 }; //main valve pins
int mainLed[] = {0, 5, 6}; //main led pins
int bedLed[] = {0, 14, 15, 16 }; //bed led pins

int lastPressed = 0; //holds value of last pressed button

int mSelector = 0; //indicates selected main index
int bSelector = 0; //indicates selected bed index

void setup() {
  Serial.begin(9600);

  /* SETUP INPUTS
  btn1 = pin2 => start/stop
  btn2 = pin3 => toggle main
  btn3 = pin4 => toggle bed
  */

  pinMode(btn1, INPUT);
  Serial.println("Input initialized: Pin 2");
  attachInterrupt(0, startStop, RISING);

  pinMode(btn2, INPUT);
  Serial.println("Input initialized: Pin 3");

  pinMode(btn3, INPUT);
  Serial.println("Input initialized: Pin 4");


  /* SETUP OUTPUTS
  pin5 = mainLed1
  pin6 = mainLed2
  pin7 = main1
  pin8 = main2
  pin9 = bed1
  pin10 = bed2
  pin11 = bed3
  ...
  pin14 = bedLed1
  pin15 = bedLed2
  pin16 = bedLed3
  */

  for (int x = 7; x < 17; x++) {
    pinMode(x, OUTPUT);
    //   digitalWrite(x, LOW);
    Serial.print("Output initialized: Pin "); Serial.println(x);
  }
}
void loop() {

  scanInput();



}

void scanInput() {

  //Serial.println("scanInput called");

  lastPressed = 0;   //reset last pressed button

  for (int x = 1; x < 3; x++) {
    Serial.println(x);

    if (debounce(btnArray[x]))

    { lastPressed = x;
      //  Serial.print("btn pressed: "); Serial.println(x);
      switch (lastPressed) {
        case 1:
          //    Serial.println("toggling main");
          toggleMain();
          break;
        case 2:
          //     Serial.println("toggling bed");
          toggleBed();
          break;

        default:
          break;
      }
    }
  }
}


void toggleMain() {
  Serial.println("toggleMain called");

  mSelector ++;
  bSelector = 1;
  if (mSelector > 2) {
    mSelector = 0;
    bSelector = 0;
  }
  selectedMain = main[mSelector];
  selectedBed = bed[bSelector];
  Serial.print("Main changed: ); Serial.println(selectedMain);
  Serial.print("Bed : ); Serial.println(selectedBed);
}

void toggleBed() {
  Serial.println("toggling bed");

  bSelector ++;
  dirty = 1; //ndicates bed change when system is running
  if (bSelector > 3) {
    bSelector = 0;
    dirty = 3;
  }
  selectedBed = bed[bSelector];
}

void toggleBed() {
  Serial.println("toggleBed called");
}


void startStop() {
  Serial.println("start stop");

  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
    //	isRunning != isRunning;
    Serial.println("Start stop called");
  }
  last_interrupt_time = interrupt_time;
  ;
}

boolean debounce(int _pin) {
  // Serial.println("debouncing");

  boolean state = false;
  long lastChecked = 0;
  state = digitalRead(_pin);
  if ((dsw.elapsed() - lastChecked < 200) && (state)) {
    //Serial.println("waiting...");
    delay(100);
  }
  if (state) {
    //if initially pressed
    //wait 100millis then check again
    //if true again, btn pressed
    dsw.reset();
    dsw.start();
    while (dsw.elapsed() < 100) {
      //mark time
    }
    state = digitalRead(_pin);
    lastChecked = dsw.elapsed();
    //if (state) Serial.println("btn debounced");
  }
  return state;
}
