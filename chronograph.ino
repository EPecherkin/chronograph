const int segmentA = 12;
const int segmentB = 10;
const int segmentC = 13; // a0
const int segmentD = 16; // a2
const int segmentE = 17; // a3
const int segmentF = 11;
const int segmentG = 14;
const int segmentH = 15; // a1

const int digit1   = 7;
const int digit2   = 8;
const int digit3   = 9;
const int digit4   = 6;

const int segmentsCount = 8;
const int digitsCount = 4;
const int segments[] = { segmentA, segmentB, segmentC, segmentD, segmentE, segmentF, segmentG, segmentH };
const int digits[]   = { digit1, digit2, digit3, digit4 };

void outreset() {
  for(int i = 0; i < segmentsCount; ++i) {
    digitalWrite(segments[i], HIGH);  
  }
  
  for(int i = 0; i < digitsCount; ++i) {
    digitalWrite(digits[i], LOW);  
  }
}

void outdigit(int number, int point, int digit) {
  digitalWrite(digits[digit-1], HIGH);

  if(point)
    digitalWrite(segmentH, LOW);

  switch(number) {
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

int counter = 1;
void outvalue(double value) {
  int point = 0;
  if(counter == 3)
    point = 1;
    
  int val = (int)(value*10);
  int figure = 0;
  for(int i = 1; i <= (5 - counter); ++i) {
    figure = val % 10;
    val = val / 10;
  }

  outdigit(figure, point, counter);
  counter = 1 + counter % 4;
}

void setup() {                
  for(int i = 0; i < segmentsCount; ++i) {
    pinMode(segments[i], OUTPUT);  
  }

  for(int i = 0; i < digitsCount; ++i) {
    pinMode(digits[i], OUTPUT);  
  }

  outreset();
}

void loop() {
  outreset();
  outvalue(567.3);
  
  //for(int i = 0; i < segmentsCount; ++i) {
  //  digitalWrite(segments[i], LOW);  
  //}
  
  //for(int i = 0; i < digitsCount; ++i) {
  //  digitalWrite(digits[i], HIGH); 
  //}
  
//  delay(3);
  delay(1000);
}
