#include <StopWatch.h>

long lastElapsed;

// StopWatch rStopwatch, lStopwatch, dsw;
boolean isRunning = false;
int dirty = 0;

StopWatch dsw;
int btn1 = 2;
int btn2 = 3;
int btn3 = 4;

int btnArray[3] = {0, 3, 4}; //button pins
int bedValve[4] = {0, 9, 10, 11}; //bed valve pins
int mainValve[3] = {0, 7, 8}; //main valve pins
int mainLed[3] = {0, 17, 18}; //main led pins
int bedLed[4] = {0, 14, 15, 16}; //bed led pins
int selectedMainState[3]; //holds states for main valves
int selectedBedState[4]; //holds states for bed valves
int ledState = LOW;
int mainMatrix[3][3] {
  {0, 0, 0},
  {0, 1, 0},
  {0, 0, 1}
};
int bedMatrix[8][4] = {

  {0, 0, 0, 0},
  {0, 0, 0, 1},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 1, 0, 0},
  {0, 1, 0, 1},
  {0, 1, 1, 0},
  {0, 1, 1, 1}
};
unsigned long prevMillis = 0;
const long interval = 0;


int lastPressed = 0; //holds value of last pressed button

int mSelector = 0; //indicates selected main index
int bSelector = 0; //indicates selected bed index
int selectedMain = 0; //resolved pin to selected main
int selectedBed = 0; //resolved pin to selected bed

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

  for (int x = 5; x < 19; x++) {
    pinMode(x, OUTPUT);
    //   digitalWrite(x, LOW);
    Serial.print("Output initialized: Pin "); Serial.println(x);
  }
}
void loop() {
  scanInput(); //update states implicitly called
  ledOutputs();

  //  Serial.print("Main: "); Serial.println(selectedMain);
  //  Serial.print("Bed: "); Serial.println(selectedBed);
  //  Serial.print("State: "); Serial.println(isRunning);
  //  delay(300);

}

void scanInput() {

  //Serial.println("scanInput called");

  lastPressed = 0;   //reset last pressed button

  for (int x = 1; x < 3; x++) {

    if (debounce(btnArray[x]))

    { lastPressed = x;
      //  Serial.print("btn pressed: "); Serial.println(x);
      switch (lastPressed) {
        case 1:
          //    Serial.println("toggling main");
          updateMain();
          break;
        case 2:
          //     Serial.println("toggling bed");
          updateBed();
          break;

        default:
          break;
      }
    }
  }
}


void updateMain() {
  Serial.println("toggleMain called");

  mSelector ++;
  Serial.print("mselector: "); Serial.println(mSelector);

  bSelector = 7; //start at "all beds on"
  if (mSelector > 2) {
    mSelector = 0;
    bSelector = 0;
  }
  for (int x = 0; x < 3; x++) {
    selectedMainState[x] = mainMatrix[mSelector][x];
  }
  selectedMain = mainValve[mSelector];
  selectedBed = bedValve[bSelector];
  int out = (selectedMainState[1] * 10) + (selectedMainState[2] * 1);
  Serial.print("array"); Serial.println(out);
  Serial.print("Main changed: "); Serial.println(selectedMain);
  Serial.print("Bed: "); Serial.println(selectedBed);
}

void updateBed() {
  Serial.println("toggling bed");

  bSelector--;
  dirty = 1; //indicates bed change when system is isRunning
  if (bSelector < 0 ) { //loop through bed states
    bSelector = 7;
    dirty = 3;
  }

  for (int x = 0; x < 4; x++) {
    selectedBedState[x] = bedMatrix[bSelector][x];
  }
  //selectedBed = bedValve[bSelector];
  //  Serial.print("Main: "); Serial.println(selectedMain);
  int out = (selectedBedState[1] * 100) + (selectedBedState[2] * 10) + selectedBedState[3];
  Serial.print("Bed changed: "); Serial.println(out);
}

/*void newUpdatebBed(){
  for (int x = 0; x <4; x++){
    if selected}

}*/



void startStop() {
  // Serial.println("start stop");

  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
    isRunning = !isRunning;
    Serial.println("Start stop called");  Serial.print("State: "); Serial.println(isRunning);

  }
  last_interrupt_time = interrupt_time;

  ;
}

void ledOutputs() {
  for (int x = 1; x < 3; x++)
  {
    digitalWrite(mainLed[x], selectedMainState[x]);
  }

  for (int x = 1; x < 4; x++) {
    digitalWrite(bedLed[x], selectedBedState[x]);
  }
  /*unsigned long currentMillis = millis();

  if (currentMillis - prevMillis >= interval) {
    prevMillis = currentMillis;
    digitalWrite(mainLed[2], !digitalRead(mainLed[mSelector]));

    if (ledState == LOW) {

      for (int x = 1; x < 3; x++) {
        digitalWrite(mainLed[x], selectedMainState[x]);
      }
      for (int x = 1; x < 4; x++) {
        digitalWrite(bedLed[x], selectedBedState[x]);
      }
      ledState = HIGH;
    }
    else if (ledState == HIGH) {
      for (int x = 1; x < 3; x++) {
        digitalWrite(mainLed[x], 0);
      }
      for (int x = 1; x < 4; x++) {
        digitalWrite(bedLed[x], 0);
      }
    }
  }*/
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
