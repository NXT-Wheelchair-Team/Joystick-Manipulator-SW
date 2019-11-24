// author: ARTHUR THOMPSON JOHNSON
// nov 9 2019

byte dirPinR = 6;
byte stepPinR = 3;
byte dirPinL = 7;
byte stepPinL = 4;
byte enablePin = 8;
int stepLen = 1200; // 'speed' of steppers
float curX = 1.5; // current x
float curY = 1.5; // current y
float newX = 1.5; // recieved x
float newY = 1.5; // recieved y
float inPerstep = 0.00096; // inches per step, might not be accurate needs testing
long stepsR = 0; // calculated steps to move right stepper
long stepsL = 0; // calculated steps to move left stepper
long remR;
long remL;
int res = 40; // arbitrary num determing resolution of movement
int incomingByte = 0;
int BREAK;
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;
String chrY;
String chrX;
boolean NRC = false;

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
  Serial.println("loop");
  stepCalc();
  moveCntrl();
  NRC = true;
  Serial.read();
  delay(100);
  for(int k = 0; k < 100; k++){
    Serial.read();
  }
  while (NRC) {
    MARKERS();
    NEWDATA();

  }

}
void MARKERS() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  // if (Serial.available() > 0) {
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}



void NEWDATA() {
  if (newData == true) {    
    for(int n = 0; receivedChars[n] != ','; n++){
      chrX += receivedChars[n];
      BREAK = n;

    }
    for(int i = BREAK+2; receivedChars[i] != '\0'; i++){
      chrY += receivedChars[i];
    }
    
    newY = chrX.toFloat();
    chrX = "";
    Serial.println(newX);
    newX = chrY.toFloat();
    chrY = "";
    Serial.println(newY);
    newData = false;
        NRC = false;

    }
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
    delayMicroseconds(200);
    digitalWrite(stepPinL, LOW);
    delayMicroseconds(stepLen);
  }
}

void stepCalc() { // calcultes how much x and y need to change, calculates how many total steps R and L need to step
  Serial.println("stepCalc");

  long newstepsX = newX / inPerstep; // converts x to steps
  long newstepsY = newY / inPerstep; // converts y to steps
  long oldstepsX = curX / inPerstep;
  long oldstepsY = curY / inPerstep;
  long c = 2 / inPerstep;
  Serial.print("newStepsX = ");
  Serial.println(newstepsX);
  Serial.print("newStepsY = ");
  Serial.println(newstepsY);

  Serial.print("oldStepsX = ");
  Serial.println(oldstepsX);
  Serial.print("oldStepsY = ");
  Serial.println(oldstepsY);


  long xsq = sq(newstepsX);
  long xsq2 = sq(c - newstepsX);
  long ysq = sq(newstepsY);

  long oldxsq = sq(oldstepsX);
  long oldxsq2 = sq(c - oldstepsX);
  long oldysq = sq(oldstepsY);


  long oldstepsR = sqrt(oldxsq2 + oldysq);
  long oldstepsL = sqrt(oldxsq + oldysq);

  stepsR = sqrt(xsq2 + ysq); // calc R steps
  stepsL = sqrt(xsq + ysq); // calc L steps

  if (oldstepsR > stepsR) {
    digitalWrite(dirPinR, LOW); // right reverse
    Serial.println("Right Reverse");
  }
  if (oldstepsR < stepsR) {
    digitalWrite(dirPinR, HIGH); // right forward
    Serial.println("Right Forward");
  }
  if (oldstepsL > stepsL) {
    digitalWrite(dirPinL, LOW); // left reverse
    Serial.println("Left Reverse");
  }
  if (oldstepsL < stepsL) {
    digitalWrite(dirPinL, HIGH); //left forward
    Serial.println("Left Forward");
  }
  stepsR = abs(oldstepsR - stepsR);
  stepsL = abs(oldstepsL - stepsL);
}


void moveCntrl() { // divides total movement into small steps for smoother movement, calls the move functions.
 int numR = 0;
 int numL = 0;
  if (stepsR <= res){
    numR = stepsR;
    }
  else{
    numR = stepsR / res; // R step lenghts
    
  }
  if (stepsL <= res){
    numL = stepsL;
    }
  else{
    numL = stepsL / res; // L step lengths
  }
  
  int r = 0;
  int l = 0;
  Serial.print("stepsR = ");
  Serial.println(stepsR);
  Serial.print("stepsL = ");
  Serial.println(stepsL);
  while (r < stepsR || l < stepsL) {
    Serial.print("r = ");
    Serial.println(r);
    Serial.print("l = ");
    Serial.println(l);
    delay(4);
    if (r < stepsR) {
      stepMoveR(numR);
      r += numR;
    }
    delay(4);
    if (l < stepsL) {
      stepMoveL(numL);
      l += numL;
    }
  }
  curX = newX;
  Serial.print("NEW X = ");
  Serial.println(newX);
  curY = newY;
  Serial.print("NEW Y = ");
  Serial.print(newY);

}
