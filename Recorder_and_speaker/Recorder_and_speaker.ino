/*
* ISD1820 Arduino Voice Recorder
* to record and play sound using Arduino and ISD1820 Sound Recorder
* get this code from https://www.gadgetprogrammers.online
/*
* ISD1820 Arduino Voice Recorder
* Code Written by Anshul Pareek for the purpose of education
* on June 06, 2018 at 11:20 pm, 
*/
#define REC 8 // pin 2 is used for recording
//#define PLAY_E 3 // pin 3 is used for playback-edge trigger
#define PLAY_L 9


#define FT 5 // pin 5 is used for feed through

// and will NOT record
#define playTime 5000 // playback time 5 seconds
#define recordTime 3000 // recording time 3 seconds you can extend time upto 10 seconds

bool keepPlaying = false;
char inChar;

void setup() 
{

 pinMode(REC,OUTPUT);// set the REC pin as output

 pinMode(PLAY_L,OUTPUT);// set the PLAY_L pin as output
 pinMode(FT,OUTPUT);// set the FT pin as output  
 Serial.begin(9600);// set up Serial monitor   
}
void loop() {

  
   while (Serial.available() > 0 || keepPlaying || inChar != 's' || inChar != 'S') {
    inChar = (char)Serial.read();
         Serial.println(inChar);
           if(inChar =='p' || inChar =='P' || keepPlaying && inChar != 's' || keepPlaying && inChar != 'S'){
            Serial.println("In if: " + String(inChar));
            if (inChar == 's' || inChar == 'S') {
              keepPlaying = false;
              break;
            }
            keepPlaying = true;
           digitalWrite(PLAY_L, HIGH);
           delay(50);
           digitalWrite(PLAY_L, LOW);  
             Serial.println("Playbak Started");  
           delay(playTime);
             Serial.println("Playbak Ended");
//           break; 
           }       
           else if(inChar =='r' || inChar =='R'){
            keepPlaying = false;
             digitalWrite(REC, HIGH);
             Serial.println("Recording started");
             delay(recordTime);
             digitalWrite(REC, LOW);
             Serial.println("Recording Stopped ");              
           } 
           else if(inChar == 's' || inChar == 'S') {
            keepPlaying = false;
            break;
           }
                         
//     Serial.println("###Serial Monitor Exited");      
   }// wihile
Serial.println("### Enter r to record, p to play");
 delay(500);
}
 
