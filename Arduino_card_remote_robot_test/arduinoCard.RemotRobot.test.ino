#include <IRremote.h>

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;

/*LED TEST*/

const int led_RED = 9;
const int led_YELLOW = 8;


/*motor set up*/
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position


void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
   pinMode(led_RED, OUTPUT);
   pinMode(led_YELLOW, OUTPUT);
   
    myservo.attach(10);  // attaches the servo on pin 10 to the servo object
}

void loop(){
  if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;
         

        switch(results.value){
          case 0xFFA25D:
          Serial.println("CH-");
          LED(led_RED,true);
          break;
          case 0xFF629D:
          Serial.println("CH");
          LED(led_YELLOW,true);
          break;
          case 0xFFE21D:
          Serial.println("CH+");
          break;
          case 0xFF22DD:
          Serial.println("|<<");
          LED(led_RED,false);
          break;
          case 0xFF02FD:
          Serial.println(">>|");
          LED(led_YELLOW,false);
          break ;  
          case 0xFFC23D:
          Serial.println(">|");
          break ;               
          case 0xFFE01F:
          Serial.println("-");
          break ;  
          case 0xFFA857:
          Serial.println("+");
          break ;  
          case 0xFF906F:
          Serial.println("EQ");
          break ;  
          case 0xFF6897:
          Serial.println("0");
          break ;  
          case 0xFF9867:
          Serial.println("100+");
          break ;
          case 0xFFB04F:
          Serial.println("200+");
          break ;
          case 0xFF30CF:
          Serial.println("1");
          
          Motor(15);
          break ;
          case 0xFF18E7:
          Serial.println("2");
         
          Motor(10); 
          break ;
          case 0xFF7A85:
          Serial.println("3");
      
           Motor(5); 
          break ;
          case 0xFF10EF:
          Serial.println("4");
          break ;
          case 0xFF38C7:
          Serial.println("5");
          break ;
          case 0xFF5AA5:
          Serial.println("6");
          break ;
          case 0xFF42BD:
          Serial.println("7");
          break ;
          case 0xFF4AB5:
          Serial.println("8");
          break ;
          case 0xFF52AD:
          Serial.println("9");
          break ;      
        }
        key_value = results.value;
        irrecv.resume(); 
  }
}

void Motor(int Speed) 
{
   for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(Speed);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(Speed);                       // waits 15ms for the servo to reach the position
  }
 
}

void LED(int ledPin, bool toLight)
{

  if(toLight)
    digitalWrite(ledPin, HIGH);
  
  else
    digitalWrite(ledPin, LOW); 

}
