
#include <Wire.h>
#include <virtuabotixRTC.h>
#include <IRremote.hpp>

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

// define the values for the remote functionality
#define CH_Button 0xB946FF00 // CH == *
#define Play_Button 0xBC43FF00 // >|
#define EQ_Button 0xF609FF00 // EQ
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

virtuabotixRTC myRTC(2, 3, 4); //Wiring of the RTC (CLK,DAT,RST)
//If you change the wiring change the pins here also


int i1, i2, i3, i4;
char c1, c2, c3, c4;
long keypressed, keypressedx; // long datatype to store the received data correctly

int A_hour = NULL;
int A_minute = NULL;
int AlarmIsActive = NULL;

int buzzer = 13;


const int IR_RECEIVE_PIN = 7;
//const bool ENABLE_LED_FEEDBACK = false;
unsigned long key_value = 0;


void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // start the receiver
}


// wait for key: wait for a limited time period if a key gets pressed or not
long waitForData() {
  long key = NO_KEY;
  Serial.println("Waiting...");
  while (key == NO_KEY) {
    if (IrReceiver.decode()) {
      Serial.println("Data received: " + String(IrReceiver.decodedIRData.decodedRawData, HEX));
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
    case Button_0: return '0';
    case Button_1: return '1';
    case Button_2: return '2';
    case Button_3: return '3';
    case Button_4: return '4';
    case Button_5: return '5';
    case Button_6: return '6';
    case Button_7: return '7';
    case Button_8: return '8';
    case Button_9: return '9';
    default: return ' ';
  }
}


void loop() {

  while (keypressed == NO_KEY) {
    if (IrReceiver.decode()) {
      Serial.println("Data received: " + String(IrReceiver.decodedIRData.decodedRawData, HEX));
      //  IrReceiver.printIRResultShort(&Serial); //optional use new print version, we probably wont need it for our project
      keypressed = IrReceiver.decodedIRData.decodedRawData;
      IrReceiver.resume();
    }
    myRTC.updateTime();

    if (myRTC.hours == A_hour && myRTC.minutes == A_minute && AlarmIsActive == 1 && myRTC.seconds >= 0 && myRTC.seconds <= 2) {
      while (keypressedx == NO_KEY) {
        tone(buzzer, 1000); //You can modify the tone or make your own sound
        delay(100);
        tone(buzzer, 2000);
        delay(100);
        Serial.println("Get up !!!"); //Message to show when the alarm is ringing
        if (IrReceiver.decode()) {
          Serial.println("Data received: " + String(IrReceiver.decodedIRData.decodedRawData, HEX));
          keypressedx = IrReceiver.decodedIRData.decodedRawData;
          IrReceiver.resume();
        }
      }
    }
    keypressedx = NO_KEY;
    noTone(buzzer);

    Serial.print(String(myRTC.dayofmonth) + "/" + String(myRTC.month) + "/" + String(myRTC.year) + "\n");
    Serial.print(String(myRTC.hours) + ":" + String(myRTC.minutes) + ":" + String(myRTC.seconds) + "\n");

    delay(100);
  }

//  Serial.println("Decode: " + String(IrReceiver.decode())); //0 if nothing pressed
//  Serial.println("RawData: " + String(IrReceiver.decodedIRData.decodedRawData)); //0 if nothing pressed, value of the key which was pressed

  //////////////////////////////////////// Clock setup /////////////////////////////////////////

  if (keypressed == CH_Button) {
    Serial.println("Setup");
    delay(1000);
    Serial.println("Setup year");

    long keypressed2 = waitForData();
    if (isNumber(keypressed2)) {
      c1 = mapDataToChar(keypressed2);
      Serial.println(c1);
    }

    long keypressed3 = waitForData();
    if (isNumber(keypressed3)) {
      c2 = mapDataToChar(keypressed3);
      Serial.println(c2);
    }

    long keypressed4 = waitForData();
    if (isNumber(keypressed4)) {
      c3 = mapDataToChar(keypressed4);
      Serial.println(c3);
    }

    long keypressed5 = waitForData();
    if (isNumber(keypressed5)) {
      c4 = mapDataToChar(keypressed5);
      Serial.println(c4);
    }

    i1 = (c1 - 48) * 1000;  //the keys pressed are stored into chars I convert them to int then i did some multiplication to get the code as an int of xxxx
    i2 = (c2 - 48) * 100;
    i3 = (c3 - 48) * 10;
    i4 = c4 - 48;
    int N_year = i1 + i2 + i3 + i4;
    Serial.println("Year: " + String(N_year));

    delay(500);
    Serial.println("Setup month");

    long keypressed6 = waitForData();
    if (isNumber(keypressed6)) {
      c1 = mapDataToChar(keypressed6);
      Serial.println(c1);
    }

    long keypressed7 = waitForData();
    if (isNumber(keypressed7)) {
      c2 = mapDataToChar(keypressed7);
      Serial.println(c2);
    }

    i1 = (c1 - 48) * 10;
    i2 = c2 - 48;
    int N_month = i1 + i2;
    Serial.println("Month: " + String(N_month));

    delay(500);
    Serial.println("Setup Day");

    long keypressed8 = waitForData();
    if (isNumber(keypressed8)) {
      c1 = mapDataToChar(keypressed8);
      Serial.println(c1);
    }

    long keypressed9 = waitForData();
    if (isNumber(keypressed9)) {
      c2 = mapDataToChar(keypressed9);
      Serial.println(c2);
    }

    i1 = (c1 - 48) * 10;
    i2 = c2 - 48;
    int N_day = i1 + i2;
    Serial.println("Day: " + String(N_day));

    delay(500);
    Serial.println("Setup hour");

    long keypressed10 = waitForData();
    if (isNumber(keypressed10)) {
      c1 = mapDataToChar(keypressed10);
      Serial.println(c1);
    }

    long keypressed11 = waitForData();
    if (isNumber(keypressed11)) {
      c2 = mapDataToChar(keypressed11);
      Serial.println(c2);
    }

    i1 = (c1 - 48) * 10;
    i2 = c2 - 48;
    int N_hour = i1 + i2;
    Serial.println("Hour: " + String(N_hour));

    delay(500);
    Serial.println("Setup minutes");

    long keypressed12 = waitForData();
    if (isNumber(keypressed12)) {
      c1 = mapDataToChar(keypressed12);
      Serial.println(c1);
    }

    long keypressed13 = waitForData();
    if (isNumber(keypressed13)) {
      c2 = mapDataToChar(keypressed13);
      Serial.println(c2);
    }

    i1 = (c1 - 48) * 10;
    i2 = c2 - 48;
    int N_minutes = i1 + i2;
    Serial.println("Minutes: " + String(N_minutes));

    delay(500);

    myRTC.setDS1302Time(22, N_minutes, N_hour, 1, N_day, N_month, N_year);
    keypressed = NO_KEY;
  }

  ///////////////////////////////////////// Alarm setup /////////////////////////////////////////////

  if (keypressed == EQ_Button) {
    Serial.println("Alarm setup");
    delay(1000);
    Serial.println("Set alarm hour");

    long keypressed14 = waitForData();
    if (isNumber(keypressed14)) {
      c1 = mapDataToChar(keypressed14);
      Serial.println(c1);
    }

    long keypressed15 = waitForData();
    if (isNumber(keypressed15)) {
      c2 = mapDataToChar(keypressed15);
      Serial.println(c2);
    }

    i1 = (c1 - 48) * 10;
    i2 = c2 - 48;
    A_hour = i1 + i2;
    Serial.println("Alarm hours: " + String(A_hour));

    delay(500);
    Serial.println("Set alarm minutes");

    long keypressed16 = waitForData();
    if (isNumber(keypressed16)) {
      c1 = mapDataToChar(keypressed16);
      Serial.println(c1);
    }

    long keypressed17 = waitForData();
    if (isNumber(keypressed17)) {
      c2 = mapDataToChar(keypressed17);
      Serial.println(c2);
    }

    i1 = (c1 - 48) * 10;
    i2 = c2 - 48;
    A_minute = i1 + i2;
    Serial.println("Alarm minutes: " + String(A_minute));

    delay(500);
    AlarmIsActive = 1;
    keypressed = NO_KEY;
  }

  ///////////////////////////////// Deactivate alarm ////////////////////////////////////

  if (keypressed == 'B') {
    Serial.println("Alarm deactivated");
    AlarmIsActive = 0;
    keypressed = NO_KEY;
    delay(500);
  } else {
    myRTC.updateTime();
    keypressed = NO_KEY;
  }
  //TODO solve problem that alarm is only one time setable -> why?
}
