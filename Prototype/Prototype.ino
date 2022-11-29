#include <Wire.h>
#include <virtuabotixRTC.h>
#include <IRremote.hpp>
#include <Stepper.h>

#define IN1 13
#define IN2 11
#define IN3 12
#define IN4 10

#define STEPS 2038

//Remote button values
//CH - b946ff00
//Play - bc43ff00
//EQ - f609ff00
//CH- - ba45ff00
//CH+ - b847ff00
//Back - bb44ff00
//Forward - bf40ff00
//Minus - f807ff00
//Plus - ea15ff00
//0 - e916ff00
//100+ - e619ff00
//200+ - f20dff00
//1 - f30cff00
//2 - e718ff00
//3 - a15eff00
//4 - f708ff00
//5 - e31cff00
//6 - a55aff00
//7 - bd42ff00
//8 - ad52ff00
//9 - b54aff00

//Seven segment display
// A0 = C
// A1 = DP
// A2 = D
// A3 = E
// A4 = G
// A5 = F
// 5 = B
// 6 = A

// define the values for the remote functionality
#define CH_Button 0xB946FF00 // CH == *
#define Play_Button 0xBC43FF00 // >|
#define EQ_Button 0xF609FF00 // EQ
#define PlusHundred_Button 0xE619FF00 // 100+
#define Button_0 0xE916FF00 // 0
#define Button_1 0xF30CFF00 // 1
#define Button_2 0xE718FF00 // 2
#define Button_3 0xA15EFF00 // 3
#define Button_4 0xF708FF00 // 4
#define Button_5 0xE31CFF00 // 5
#define Button_6 0xA55AFF00 // 6
#define Button_7 0xBD42FF00 // 7
#define Button_8 0xAD52FF00 // 8
#define Button_9 0xB54AFF00 // 9
#define NO_KEY 0x0

#define REC 8 // pin 2 is used for recording
#define PLAY_E 9 // pin 3 is used for playback-edge trigger
#define FT 5 // pin 5 is used for feed through
#define playTime 5000 // playback time 5 seconds
#define recordTime 3000 // recording time 3 seconds you can extend time upto 10 seconds

//seven segment display values
#define a 5
#define b A0
#define c A1
#define d A2
#define e A3
#define g A4
#define f A5

virtuabotixRTC myRTC(2, 3, 4); //Wiring of the RTC (CLK,DAT,RST)
//If you change the wiring change the pins here also


int i1, i2, i3, i4;
char c1, c2, c3, c4;
long keypressed, keypressedx; // long datatype to store the received data correctly

int A_hour = NULL;
int A_minute = NULL;
int AlarmIsActive = NULL;

const int PIN_Buzzer = 5;

char inChar;
bool configuration = false;

const int IR_RECEIVE_PIN = 7;
unsigned long key_value = 0;

Stepper myStepper = Stepper(STEPS, IN1, IN2, IN3, IN4);


void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // start the receiver

  myStepper.setSpeed(10); // Rotate step motor quickly at 10 RPM

  pinMode(REC, OUTPUT); // set the REC pin as output

  pinMode(PLAY_E, OUTPUT); // set the PLAY_E pin as output
  pinMode(FT, OUTPUT); // set the FT pin as output
  Serial.begin(9600);// set up Serial monitor

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  setAlarmTone();
}


void setAlarmTone() {
  while (configuration == false) {
    displayWaiting();
    Serial.println("Waiting to record a sound");
    Serial.println("### Press play button to record");
    long val = waitForData();
    if (val == Play_Button) {
      digitalWrite(REC, HIGH);
      Serial.println("Recording started");
      delay(recordTime);
      digitalWrite(REC, LOW);
      Serial.println("Recording Stopped ");
      configuration = true;
    }
  }
}


void clearDisplay(void)
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(g, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
}


void display1(void) {
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
}


void display2(void) {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(d, HIGH);
}


void display3(void)
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);

  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(g, HIGH);
}


void display4(void)
{
  digitalWrite(f, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(c, HIGH);
}


void display5(void)
{
  digitalWrite(a, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
}


void display6(void)
{
  digitalWrite(a, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
}


void display7(void)
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
}


void display8(void)
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
}


void display9(void)
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(f, HIGH);
}


void display0(void)
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
}


void displayWaiting() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH); 
  delay(200);
  digitalWrite(a, LOW); 
  digitalWrite(c, HIGH); 
  delay(200);
  digitalWrite(b, LOW); 
  digitalWrite(d, HIGH); 
  delay(200);
  digitalWrite(c, LOW); 
  digitalWrite(e, HIGH); 
  delay(200);
  digitalWrite(d, LOW); 
  digitalWrite(f, HIGH); 
  delay(200);
  digitalWrite(e, LOW);
  digitalWrite(a, HIGH);  
  delay(200); 
  digitalWrite(f, LOW); 
}


// wait for key: wait for a limited time period if a key gets pressed or not
long waitForData() {
  long key = NO_KEY;
  displayWaiting();
  Serial.println("Waiting...");
  while (key == NO_KEY) {
    displayWaiting();
    if (IrReceiver.decode()) {
      Serial.println("Data received 3: " + String(IrReceiver.decodedIRData.decodedRawData, HEX));
      key = IrReceiver.decodedIRData.decodedRawData;
      IrReceiver.resume();
    }
  }
  return key;
}


bool isNumber(long val) {
  if (val == Button_0 || val == Button_1 || val == Button_2 || val == Button_3 || val == Button_4 || val == Button_5 || val == Button_6 || val == Button_7 || val == Button_8 || val == Button_9) {
    return true;
  }
  return false;
}


char mapDataToChar(long val) {
  switch (val) {
    case Button_0: clearDisplay(); display0(); return '0';
    case Button_1: clearDisplay(); display1(); return '1';
    case Button_2: clearDisplay(); display2(); return '2';
    case Button_3: clearDisplay(); display3(); return '3';
    case Button_4: clearDisplay(); display4(); return '4';
    case Button_5: clearDisplay(); display5(); return '5';
    case Button_6: clearDisplay(); display6(); return '6';
    case Button_7: clearDisplay(); display7(); return '7';
    case Button_8: clearDisplay(); display8(); return '8';
    case Button_9: clearDisplay(); display9(); return '9';
    default: return ' ';
  }
}


void loop() {
  while (keypressed == NO_KEY) {
    delay(1000);
    if (IrReceiver.decode()) {
      Serial.println("Data received 2.2: " + String(IrReceiver.decodedIRData.decodedRawData, HEX));
      //  IrReceiver.printIRResultShort(&Serial); //optional use new print version, we probably wont need it for our project
      keypressed = IrReceiver.decodedIRData.decodedRawData;
      IrReceiver.resume();
    }
    
    myRTC.updateTime();
    
    if (myRTC.hours == A_hour && myRTC.minutes == A_minute && AlarmIsActive == 1 && myRTC.seconds >= 0 && myRTC.seconds <= 2) {
      while (keypressedx == NO_KEY || keypressedx != PlusHundred_Button || keypressedx != Play_Button) {
        if (keypressedx == Play_Button) {
          AlarmIsActive = false;
          Serial.println("Alarm was deactivated.");
          break;
        } else if (keypressedx == PlusHundred_Button) {
          A_minute += 5;
          Serial.println("Alarm snoozed. You prolonged your sleeping time by 5 min.");
          break;
        }
        
        myStepper.step(STEPS);
        
        digitalWrite(PLAY_E, HIGH);
        delay(50);
        digitalWrite(PLAY_E, LOW);
        Serial.println("Get up !!!"); //Message to show when the alarm is ringing
        if (IrReceiver.decode()) {
          Serial.println("Data received 1: " + String(IrReceiver.decodedIRData.decodedRawData, HEX));
          keypressedx = IrReceiver.decodedIRData.decodedRawData;
          IrReceiver.resume();
        }
      }
    }
    keypressedx = NO_KEY;

    Serial.print(String(myRTC.dayofmonth) + "/" + String(myRTC.month) + "/" + String(myRTC.year) + "\n");
    Serial.print(String(myRTC.hours) + ":" + String(myRTC.minutes) + ":" + String(myRTC.seconds) + "\n");
  }

  //////////////////////////////////////// Clock setup /////////////////////////////////////////

  if (keypressed == CH_Button) {
    Serial.println("Setup");
    delay(1000);
    Serial.println("Setup year");

    long keypressed2 = waitForData();
    while (!isNumber(keypressed2)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed2 = waitForData();
    }
    c1 = mapDataToChar(keypressed2);
    Serial.println(c1);

    long keypressed3 = waitForData();
    while (!isNumber(keypressed3)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed3 = waitForData();
    }
    c2 = mapDataToChar(keypressed3);
    Serial.println(c2);

    long keypressed4 = waitForData();
    while (!isNumber(keypressed4)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed4 = waitForData();
    }
    c3 = mapDataToChar(keypressed4);
    Serial.println(c3);

    long keypressed5 = waitForData();
    while (!isNumber(keypressed5)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed4 = waitForData();
    }
    c4 = mapDataToChar(keypressed5);
    Serial.println(c4);

    i1 = (c1 - 48) * 1000;  //the keys pressed are stored into chars I convert them to int then i did some multiplication to get the code as an int of xxxx
    i2 = (c2 - 48) * 100;
    i3 = (c3 - 48) * 10;
    i4 = c4 - 48;
    int N_year = i1 + i2 + i3 + i4;
    Serial.println("Year: " + String(N_year));

    delay(500);
    Serial.println("Setup month");

    long keypressed6 = waitForData();
    while (!isNumber(keypressed6)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed6 = waitForData();
    }
    c1 = mapDataToChar(keypressed6);
    Serial.println(c1);

    long keypressed7 = waitForData();
    while (!isNumber(keypressed7)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed7 = waitForData();
    }
    c2 = mapDataToChar(keypressed7);
    Serial.println(c2);

    i1 = (c1 - 48) * 10;
    i2 = c2 - 48;
    int N_month = i1 + i2;
    Serial.println("Month: " + String(N_month));

    delay(500);
    Serial.println("Setup Day");

    long keypressed8 = waitForData();
    while (!isNumber(keypressed8)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed8 = waitForData();
    }
    c1 = mapDataToChar(keypressed8);
    Serial.println(c1);


    long keypressed9 = waitForData();
    while (!isNumber(keypressed9)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed9 = waitForData();
    }
    c2 = mapDataToChar(keypressed9);
    Serial.println(c2);


    i1 = (c1 - 48) * 10;
    i2 = c2 - 48;
    int N_day = i1 + i2;
    Serial.println("Day: " + String(N_day));

    delay(500);
    Serial.println("Setup hour");

    long keypressed10 = waitForData();
    while (!isNumber(keypressed10)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed10 = waitForData();
    }
    c1 = mapDataToChar(keypressed10);
    Serial.println(c1);


    long keypressed11 = waitForData();
    while (!isNumber(keypressed11)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed11 = waitForData();
    }
    c2 = mapDataToChar(keypressed11);
    Serial.println(c2);


    i1 = (c1 - 48) * 10;
    i2 = c2 - 48;
    int N_hour = i1 + i2;
    Serial.println("Hour: " + String(N_hour));

    delay(500);
    Serial.println("Setup minutes");

    long keypressed12 = waitForData();
    while (!isNumber(keypressed12)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed12 = waitForData();
    }
    c1 = mapDataToChar(keypressed12);
    Serial.println(c1);


    long keypressed13 = waitForData();
    while (!isNumber(keypressed13)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed13 = waitForData();
    }
    c2 = mapDataToChar(keypressed13);
    Serial.println(c2);


    i1 = (c1 - 48) * 10;
    i2 = c2 - 48;
    int N_minutes = i1 + i2;
    Serial.println("Minutes: " + String(N_minutes));

    delay(500);

    myRTC.setDS1302Time(22, N_minutes, N_hour, 1, N_day, N_month, N_year);
    keypressed = NO_KEY;
    clearDisplay();
  }

  ///////////////////////////////////////// Alarm setup /////////////////////////////////////////////

  if (keypressed == EQ_Button) {
    Serial.println("Alarm setup");
    delay(1000);
    Serial.println("Set alarm hour");

    long keypressed14 = waitForData();
    while (!isNumber(keypressed14)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed14 = waitForData();
    }
    c1 = mapDataToChar(keypressed14);
    Serial.println(c1);

    long keypressed15 = waitForData();
    while (!isNumber(keypressed15)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed15 = waitForData();
    }
    c2 = mapDataToChar(keypressed15);
    Serial.println(c2);

    //TODO change remote input to version above

    i1 = (c1 - 48) * 10;
    i2 = c2 - 48;
    A_hour = i1 + i2;
    Serial.println("Alarm hours: " + String(A_hour));

    delay(500);
    Serial.println("Set alarm minutes");

    long keypressed16 = waitForData();
    while (!isNumber(keypressed16)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed16 = waitForData();
    }
    c1 = mapDataToChar(keypressed16);
    Serial.println(c1);

    long keypressed17 = waitForData();
    while (!isNumber(keypressed17)) {
      Serial.println("Wrong Data received. Please press the button again.");
      keypressed17 = waitForData();
    }
    c2 = mapDataToChar(keypressed17);
    Serial.println(c2);

    i1 = (c1 - 48) * 10;
    i2 = c2 - 48;
    A_minute = i1 + i2;
    Serial.println("Alarm minutes: " + String(A_minute));

    delay(500);
    AlarmIsActive = 1;
    keypressed = NO_KEY;
    clearDisplay();
  }

  ///////////////////////////////// Deactivate alarm ////////////////////////////////////

  if (keypressed == Play_Button) {
    Serial.println("Alarm deactivated");
    AlarmIsActive = 0;
    keypressed = NO_KEY;
    delay(500);
  } else {
    myRTC.updateTime();
    keypressed = NO_KEY;
  }
}
