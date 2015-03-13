#define uchar unsigned char
#define uint  unsigned int

//breadboard
const uchar segmentA = 12;
const uchar segmentB = 10;
const uchar segmentC = 13;
const uchar segmentD = 16; // a2
const uchar segmentE = 17; // a3
const uchar segmentF = 11;
const uchar segmentG = 14; // a0
const uchar segmentDP = 15; // a1

// not a rank (не разряд)
const uchar digit1   = 7;
const uchar digit2   = 8;
const uchar digit3   = 9;
const uchar digit4   = 6;

//pcb
//const uchar segmentA = 17; // a3
//const uchar segmentB = 16; // a2
//const uchar segmentC = 15; // a1
//const uchar segmentD = 14; // a0
//const uchar segmentE = 13;
//const uchar segmentF = 12;
//const uchar segmentG = 10;
//const uchar segmentDP = 21; // a7

//// it is a number count, not a rank
//const uchar digit1   = 20;
//const uchar digit2   = 19;
//const uchar digit3   = 18;
//const uchar digit4   = 9;

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

void Timer1Init() {
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


void sensorInInterrupt() {
  if(sensorIn == 0) {
    Serial.println("sensorIn");
    TCCR1B = (1<<CS10); // Timer/Counter 1 running (no prescaling)
    sensorIn = 1;
  }
}

void sensorOutInterrupt() {
  if(sensorOut == 0) {
    Serial.println("sensorOut");
    TCCR1B = 0; // Timer/Counter 1 stopped (no clock source)
    sensorOut = 1;
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("setup");

  // Global disable interrupts
  cli();

  // Timer/Counter 1 initialization
  Timer1Init();

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

  outreset();
  
  Serial.println("setup complete");
}

void loop() {
  //testLoop();
  realLoop();
  //sensorLoop();
}

double outData = 0;
uchar outDataType = 0;
uchar sensorDurty = 0;
uchar sensorDurtyCounter = 0;
const uchar sensorDurtyBreakCount = 4;
void realLoop() {
  outreset();
  
  if(sensorDurty) {
    Serial.println("Sensors is durty. Clean sensors");
    sensorDurty = 0;
    sensorDurtyCounter = 0;
    sensorIn = 0;
    sensorOut = 0;
    TCCR1B = 0;
    TCNT1 = 0;
  }
  
  if(sensorIn ^ sensorOut) {
    ++sensorDurtyCounter;
    sensorDurty = sensorDurtyCounter / sensorDurtyBreakCount;
  }

  if(sensorIn && sensorOut) {
    lastData = length * 16000000.0/ TCNT1;
    averageData = ((averageData * averageCount) + lastData) / (averageCount + 1);
    ++averageCount;

    sensorIn = 0;
    sensorOut = 0;

    TCCR1B = 0;
    TCNT1 = 0;
  }

  if(digitalRead(button)) {
    if(outData != averageData || outDataType != 0) {
      Serial.print("avg: ");
      Serial.println(averageData);
      outData = averageData;
      outDataType = 0;
    }
    outvalue(averageData);
  } else {
    if(outData != lastData || outDataType != 1) {
      Serial.print("lst: ");
      Serial.println(lastData);
      outData = lastData;
      outDataType = 1;
    }
    outvalue(lastData);
  }

  delay(3);
}

void sensorLoop() {
  outreset();
  if(sensorIn) {
    outdigit(1, 0, 1);
  }

  if(sensorOut) {
    outdigit(1, 0, 2);
  }

  if(digitalRead(button)) {
    outdigit(1, 0, 3);
  }

  delay(200);
}

void testLoop() {
  outreset();

  /*for(uchar i = 0; i < segmentsCount; ++i) {*/
    /*digitalWrite(segments[i], LOW);*/
  /*}*/

  for(uchar i = 0; i < digitsCount; ++i) {
    digitalWrite(digits[i], HIGH);
  }

  /*outdigit(8, 1, 4);*/

  /*delay(1000);*/

  for(uchar i = 0; i < segmentsCount; ++i) {
    digitalWrite(segments[i], LOW);
    delay(500);
    digitalWrite(segments[i], HIGH);
  }
}

void outreset() {
  for(uchar i = 0; i < segmentsCount; ++i) {
    digitalWrite(segments[i], HIGH);
  }

  for(uchar i = 0; i < digitsCount; ++i) {
    digitalWrite(digits[i], LOW);
  }
}

void outdigit(uchar value, uchar dot, uchar digit) {
  digitalWrite(digits[digit-1], HIGH);

  if(dot)
    digitalWrite(segmentDP, LOW);

  switch(value) {
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
void outvalue(double value) {
  uchar dot = (counter == 3) ? 1 : 0;

  uint val = (uint)(value*10);
  uchar figure = 0;
  for(uchar i = 1; i <= (5 - counter); ++i) {
    figure = val % 10;
    val = val / 10;
  }

  outdigit(figure, dot, counter);
  counter = 1 + counter % 4;
}
