#define uchar unsigned char
#define uint  unsigned int

const uchar segmentA = 17; // a3
const uchar segmentB = 16; // a2
const uchar segmentC = 15; // a1
const uchar segmentD = 14; // a0
const uchar segmentE = 13;
const uchar segmentF = 12;
const uchar segmentG = 10;
const uchar segmentDP = 21; // a7

// it is a number count, not a rank
const uchar digit1   = 20;
const uchar digit2   = 19;
const uchar digit3   = 18;
const uchar digit4   = 9;

const uchar segmentsCount = 8;
const uchar digitsCount = 4;
const uchar segments[] = { segmentA, segmentB, segmentC, segmentD, segmentE, segmentF, segmentG, segmentDP };
const uchar digits[]   = { digit1, digit2, digit3, digit4 };

const uchar button = 4;

volatile uchar s1 = 0;
volatile uchar s2 = 0;

volatile double length = 0.084;
volatile double averageData = 0;
volatile double lastData = 0;

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


void sensor1() {
  Serial.println("sensor1");
  if(s1 == 0) {
    TCCR1B = (1<<CS10); // Timer/Counter 1 running (no prescaling)
    s1 = 1;
  }
}

void sensor2() {
  Serial.println("sensor2");
  if(s2 == 0) {
    TCCR1B = 0; // Timer/Counter 1 stopped (no clock source)
    s2 = 1;
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
  attachInterrupt(0, sensor1, FALLING);
  // pin 3
  attachInterrupt(1, sensor2, FALLING);

  pinMode(button, INPUT);

  outreset();
  
  Serial.println("setup complete");
}

void loop() {
  testLoop();
  //realLoop();
  /*sensorLoop();*/
}

void realLoop() {
  outreset();

  if(s1 && s2) {
    averageData = ((averageData * 2) + lastData) / 3;
    /*double time = TCNT1 * (1.0 / 16000000.0;*/
    /*lastData = length / time;*/
    lastData = length * 16000000.0/ TCNT1;

    s1 = 0;
    s2 = 0;

    TCCR1B = 0;
    TCNT1 = 0;
  }

  if(digitalRead(button)) {
    //Serial.print("avg: ");
    //Serial.println(averageData);
    outvalue(averageData);
  } else {
    //Serial.print("lst: ");
    //Serial.println(lastData);
    outvalue(lastData);
  }

  delay(3);
}

void sensorLoop() {
  outreset();
  if(s1) {
    outdigit(1, 0, 1);
  }

  if(s2) {
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

void outdigit(uchar value, uchar point, uchar digit) {
  digitalWrite(digits[digit-1], HIGH);

  if(point)
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
  uchar point = (counter == 3) ? 1 : 0;

  uint val = (uint)(value*10);
  uchar figure = 0;
  for(uchar i = 1; i <= (5 - counter); ++i) {
    figure = val % 10;
    val = val / 10;
  }

  outdigit(figure, point, counter);
  counter = 1 + counter % 4;
}
