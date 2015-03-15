#define uchar unsigned char
#define uint  unsigned int

const uchar segmentA = A3; //17;
const uchar segmentB = A2; //16;
const uchar segmentC = A1; //15;
const uchar segmentD = A0; //14;
const uchar segmentE = 13;
const uchar segmentF = 12;
const uchar segmentG = 10;
const uchar segmentDP = 7;

// it is a number count, not a rank
const uchar digit1   = 8;
const uchar digit2   = A5; //19;
const uchar digit3   = A4; //18;
const uchar digit4   = 9;

const uchar segmentsCount = 8;
const uchar digitsCount = 4;
const uchar segments[] = { segmentA, segmentB, segmentC, segmentD, segmentE, segmentF, segmentG, segmentDP };
const uchar digits[]   = { digit1, digit2, digit3, digit4 };

const uchar button = 4;

volatile uchar sensorIn = 0;
volatile uchar sensorOut = 0;

double length = 0.084;
uint averageCount = 0;
double averageData = 0;
double lastData = 0;

void Counter1Init() {
  TCNT1 = 0;

  // Bits: COM1A1 COM1A0 COM1B1 COM1B0 - - WGM11 WGM10
  TCCR1A = 0;

  // Bits: ICNC1 ICES1 - WGM13 WGM12 CS12 CS11 CS10
  TCCR1B = 0;

  // Bits: FOC1A FOC1B - - - - - -
  TCCR1C = 0;

  // Bits: - - ICIE1 - - OCIE1B OCIE1A TOIE1
  TIMSK1 = 0;

  // Bits: – – ICF1 – - OCF1B OCF1A TOV1
  TIFR1 = 0;
}

void startCounter1() {
  TCCR1B = (1<<CS10); // Counter 1 running (no prescaling)
}

void stopCounter1() {
  TCCR1B = 0; // Counter 1 stopped (no clock source)
}

void clearCounter1() {
  stopCounter1();
  TCNT1 = 0;
}

void sensorInInterrupt() {
  if(sensorIn == 0) {
    //Serial.println("sensorIn");
    startCounter1();
    sensorIn = 1;
  }
}

void sensorOutInterrupt() {
  if(sensorOut == 0) {
    //Serial.println("sensorOut");
    stopCounter1();
    sensorOut = 1;
  }
}

void clearSensors() {
  sensorIn = 0;
  sensorOut = 0;

  clearCounter1();
}

void setup() {
  Serial.begin(9600);

  // Global disable interrupts
  cli();

  // Timer/Counter 1 initialization
  Counter1Init();

  // Global enable interrupts
  sei();

  for(uchar i = 0; i < segmentsCount; ++i) {
    pinMode(segments[i], OUTPUT);
  }

  for(uchar i = 0; i < digitsCount; ++i) {
    pinMode(digits[i], OUTPUT);
  }

  // pin 2
  attachInterrupt(0, sensorInInterrupt, FALLING);
  // pin 3
  attachInterrupt(1, sensorOutInterrupt, FALLING);

  pinMode(button, INPUT);

  showReset();

  Serial.println("Ready!");
}

void loop() {
  realLoop();
  //testShowLoop();
  //testSensorsLoop();
}

// count = loop counts = count * loopDelay (ms)
const uchar loopDelay = 3;
uint loopsAfterMeasuringCounter = 0;
const uint errorCanBeAfterCount = 300;
uchar sensorError = 0;
uchar sensorErrorConfirmCounter = 0;
const uchar sensorErrorConfirmCount = 2;
uint showErrorCounter = 0;
const uint showErrorCount = 600;

void realLoop() {
  showReset();

  if(sensorError) {
    clearSensors();
    if(showErrorCounter >= showErrorCount) {
      showErrorCounter = 0;
      sensorError = 0;
    }
  }

  if(sensorIn ^ sensorOut) {
    ++sensorErrorConfirmCounter;
    uchar error = sensorErrorConfirmCounter / sensorErrorConfirmCount;
    if((loopsAfterMeasuringCounter >= errorCanBeAfterCount) && error) {
      Serial.println("Error");
      sensorError = 1;
      sensorErrorConfirmCounter = 0;
    }
  }

  if(sensorIn && sensorOut) {
    lastData = length * 16000000.0/ TCNT1;
    averageData = ((averageData * averageCount) + lastData) / (averageCount + 1);
    ++averageCount;

    clearSensors();

    Serial.print("lst: ");
    Serial.println(lastData);
    Serial.print("avg: ");
    Serial.println(averageData);
  }

  if(sensorError) {
    if(showErrorCounter < showErrorCount) {
      ++showErrorCounter;
    }
    showValue(-1);
  } else {
    if(loopsAfterMeasuringCounter < errorCanBeAfterCount) {
      ++loopsAfterMeasuringCounter;
    }
    if(digitalRead(button)) {
      showValue(averageData);
    } else {
      showValue(lastData);
    }
  }

  delay(loopDelay);
}

void testSensorsLoop() {
  showReset();
  if(sensorIn) {
    Serial.println("sensorIn");
    showDigit(1, 0, 1);
  }

  if(sensorOut) {
    Serial.println("sensorOut");
    showDigit(1, 0, 2);
  }

  if(digitalRead(button)) {
    Serial.println("button");
    showDigit(1, 0, 3);
  }

  clearSensors();

  delay(200);
}

void testShowLoop() {
  showReset();

  /*for(uchar i = 0; i < segmentsCount; ++i) {*/
    /*digitalWrite(segments[i], LOW);*/
  /*}*/

  for(uchar i = 0; i < digitsCount; ++i) {
    digitalWrite(digits[i], HIGH);
  }

  /*showDigit(8, 1, 4);*/

  /*delay(1000);*/

  for(uchar i = 0; i < segmentsCount; ++i) {
    digitalWrite(segments[i], LOW);
    delay(500);
    digitalWrite(segments[i], HIGH);
  }
}

void showReset() {
  for(uchar i = 0; i < segmentsCount; ++i) {
    digitalWrite(segments[i], HIGH);
  }

  for(uchar i = 0; i < digitsCount; ++i) {
    digitalWrite(digits[i], LOW);
  }
}

void showDigit(char value, uchar dot, uchar digit) {
  digitalWrite(digits[digit-1], HIGH);

  if(dot)
    digitalWrite(segmentDP, LOW);

  switch(value) {
    case -1:
      digitalWrite(segmentG, LOW);
      break;
    case 0:
      digitalWrite(segmentA, LOW);
      digitalWrite(segmentB, LOW);
      digitalWrite(segmentC, LOW);
      digitalWrite(segmentD, LOW);
      digitalWrite(segmentE, LOW);
      digitalWrite(segmentF, LOW);
      break;
    case 1:
      digitalWrite(segmentB, LOW);
      digitalWrite(segmentC, LOW);
      break;
    case 2:
      digitalWrite(segmentA, LOW);
      digitalWrite(segmentB, LOW);
      digitalWrite(segmentG, LOW);
      digitalWrite(segmentE, LOW);
      digitalWrite(segmentD, LOW);
      break;
    case 3:
      digitalWrite(segmentA, LOW);
      digitalWrite(segmentB, LOW);
      digitalWrite(segmentG, LOW);
      digitalWrite(segmentC, LOW);
      digitalWrite(segmentD, LOW);
      break;
    case 4:
      digitalWrite(segmentF, LOW);
      digitalWrite(segmentG, LOW);
      digitalWrite(segmentB, LOW);
      digitalWrite(segmentC, LOW);
      break;
    case 5:
      digitalWrite(segmentA, LOW);
      digitalWrite(segmentF, LOW);
      digitalWrite(segmentG, LOW);
      digitalWrite(segmentC, LOW);
      digitalWrite(segmentD, LOW);
      break;
    case 6:
      digitalWrite(segmentA, LOW);
      digitalWrite(segmentF, LOW);
      digitalWrite(segmentE, LOW);
      digitalWrite(segmentD, LOW);
      digitalWrite(segmentC, LOW);
      digitalWrite(segmentG, LOW);
      break;
    case 7:
      digitalWrite(segmentA, LOW);
      digitalWrite(segmentB, LOW);
      digitalWrite(segmentC, LOW);
      break;
    case 8:
      digitalWrite(segmentA, LOW);
      digitalWrite(segmentB, LOW);
      digitalWrite(segmentC, LOW);
      digitalWrite(segmentD, LOW);
      digitalWrite(segmentE, LOW);
      digitalWrite(segmentF, LOW);
      digitalWrite(segmentG, LOW);
      break;
    case 9:
      digitalWrite(segmentA, LOW);
      digitalWrite(segmentB, LOW);
      digitalWrite(segmentC, LOW);
      digitalWrite(segmentD, LOW);
      digitalWrite(segmentF, LOW);
      digitalWrite(segmentG, LOW);
      break;
  }
}

uchar counter = 1;
void showValue(double value) {
  if(value < 0) {
    showDigit(-1, 0, counter);
  } else {
    uchar dot = (counter == 3) ? 1 : 0;

    uint val = (uint)(value*10);
    uchar figure = 0;
    for(uchar i = 1; i <= (5 - counter); ++i) {
      figure = val % 10;
      val = val / 10;
    }
    showDigit(figure, dot, counter);
  }

  counter = 1 + counter % 4;
}
