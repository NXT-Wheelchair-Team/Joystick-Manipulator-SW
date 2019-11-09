byte dirPinR = 6;
byte stepPinR = 3;
byte dirPinL = 7;
byte stepPinL = 4;
byte enablePin = 8;
int stepLen = 1000;
int curX = 1;
int curY = 1;
int newX = 2;
int newY = 2;
int inPerstep = 0.00096; // inches per step, might not be accurate needs testing
int stepsR = 1000;
int stepsL = 2000;
int res = 200; // arbitrary num determing resolution of movement
void setup() {
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  pinMode(dirPinR, OUTPUT);
  pinMode(stepPinR, OUTPUT);
  pinMode(dirPinL, OUTPUT);
  pinMode(stepPinL, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(dirPinR, HIGH);
  digitalWrite(dirPinL, HIGH);
  // stepCalc();
  moveCntrl();
  delay(1000000);


}

void stepMoveR(int steps) { // moves right motor 'steps' num of steps
  for (int i = 0; i < steps; i++) {
      digitalWrite(stepPinR, HIGH);
      digitalWrite(stepPinR, LOW);
      delayMicroseconds(stepLen);
   
  }
}

void stepMoveL(int steps) { // moves left motor 'steps' num of steps
  for (int i = 0; i < steps; i++) {
      digitalWrite(stepPinL, HIGH);
      digitalWrite(stepPinL, LOW);
      delayMicroseconds(stepLen);
  }
}

void stepCalc() { // calcultes how much x and y need to change, calculates how many total steps R and L need to step
  Serial.println("stepCalc");
  int dx = newX - curX; // new X and new Y should be even numbers to avoid floats
  int dy = newY - curY;
  Serial.println("dx dy");
  int stepsX = dx / inPerstep; // converts x to steps
  int stepsY = dy / inPerstep; // converts y to steps
  Serial.println("stepsX stepsY");
  int stepsR = sqrt((2-stepsX)^2 + stepsY^2); // calc R steps
  int stepsL = sqrt(stepsX^2 + stepsY^2); // calc L steps
  Serial.println("stepsR stepsL");
}


void moveCntrl(){ // divides total movement into small steps for smoother movement, calls the move functions.
  int numR = stepsR/res;
  int numL = stepsL/res;
  int r = 0;
  int l = 0;
  Serial.print("stepsR = ");
  Serial.println(stepsR);
  Serial.print("stepsL = ");
  Serial.println(stepsL);
  while (r < stepsR || l < stepsL){
    Serial.print("r = ");
    Serial.println(r);
    Serial.print("l = ");
    Serial.println(l);
    if (r < stepsR){
      stepMoveR(numR);
      r += numR;
    }
    if (l < stepsL){
      stepMoveL(numL);
      l += numL;
    }
  }
  curX = newX;
  curY = newY;
  
}
