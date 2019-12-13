// author: ARTHUR THOMPSON JOHNSON
// nov 24 2019

byte dirPinR = 6;
byte stepPinR = 3;
byte dirPinL = 7;
byte stepPinL = 4;
byte enablePin = 8;
int stepLen = 1200; // 'speed' of steppers
float inPerstep = 0.00096; // inches per step, might not be accurate needs testing

float curX = 1.5; // current x
float curY = 1.5; // current y
float newX = 1.5; // recieved x
float newY = 1.5; // recieved y

long newstepsX;
long newstepsY;
long oldstepsX;
long oldstepsY;

long xsq;
long xsq2;
long ysq;
long c = 2 / inPerstep;

long stepsR; // calculated steps to move right stepper
long stepsL; // calculated steps to move left stepper
long oldstepsR;
long oldstepsL;
int res = 40; // arbitrary num determing resolution of movement

int incomingByte = 0;
int BREAK;
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;
String chrY;
String chrX;
boolean NRC = false;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

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


///////////////////////////////////////////////////////////////////////////////
//// SERIAL STUFF
///////////////////////////////////////////////////////////////////////////////


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

///////////////////////////////////////////////////////////////////////////////
//// STEP MOVEMENT
///////////////////////////////////////////////////////////////////////////////

void stepMoveR(int steps) { // moves right motor 'steps' num of steps
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPinR, HIGH);
    digitalWrite(stepPinR, LOW);
    delayMicroseconds(stepLen);
    oldstepsR ++;

  }
}

void stepMoveL(int steps) { // moves left motor 'steps' num of steps
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPinL, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPinL, LOW);
    delayMicroseconds(stepLen);
    oldstepsL ++;
  }
}


///////////////////////////////////////////////////////////////////////////////
//// STEP CALCULATIONS
///////////////////////////////////////////////////////////////////////////////



void stepCalc() { // calcultes how much x and y need to change, calculates how many total steps R and L need to step
  oldCalc();
  newCalc();
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


void newCalc(){
  newstepsX = newX / inPerstep;
  newstepsY = newY /inPerstep;
  
  xsq = sq(newstepsX);
  xsq2 = sq(c - newstepsX);
  ysq = sq(newstepsY);
  
  stepsR = sqrt(xsq2 + ysq);
  stepsL = sqrt(xsq + ysq);
  
}

void oldCalc(){
  oldstepsX = curX / inPerstep;
  oldstepsY = curY / inPerstep;

  xsq = sq(oldstepsX);
  xsq2 = sq(c - oldstepsX);
  ysq = sq(newstepsY);
  
  oldstepsR = sqrt(xsq2 + ysq);
  oldstepsL = sqrt(xsq + ysq);
}


///////////////////////////////////////////////////////////////////////////////
//// MOVE CONTROLS 
///////////////////////////////////////////////////////////////////////////////


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
