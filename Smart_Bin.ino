#include<SoftwareSerial.h> // Supports Serial Communication 
SoftwareSerial GPRS(5, 6); // Set up communication on pin 5 and 6 
int trigPin = 11; // trigger pin set to 11 for the ultrasonic sensor 
int echoPin = 12; //ehco pin set to 11 for the ultrasonic sensor 
int led = 2; 
int ledg = 4; 
int c = 0; 
void setup() { 
  pinMode(led, OUTPUT); 
  pinMode(ledg, OUTPUT); 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  GPRS.begin(9600); 
  Serial.begin(9600);  
  delay(10000); 
} 
// main program 
void loop() { 
  long duration, distance; 
  digitalWrite(trigPin,HIGH); 
  delayMicroseconds(1000); 
  digitalWrite(trigPin, LOW); 
  duration=pulseIn(echoPin, HIGH); 
  distance =(duration/2)/29.1; // calculating the distance using the ultrasonic sensor 
  Serial.print(distance); //printing distance on serial monitor 
 
  Serial.println("CM"); 
  delay(1000); 
  if((distance<=10))  
  { 
//setting counter to estimate time when the dustbin is full, this is done because sensor may be triggered //accidently some times 
    c = c + 1;  
    digitalWrite(ledg, LOW); 
    delay(500); 
    digitalWrite(led, HIGH); 
    Serial.println("c ="); 
    Serial.print(c); 
    delay(500); 
    if((c==3)){  
    delay(1000); 
    sendSMS(); // calling sendSMS module 
    delay(500); 
    Serial.println("Sent SMS!");    
    Serial.println( "dustbin is reached max level");   
    delay(100); 
    } 
    delay(6000); 
     
  } 
  else if(distance>10) 
  { 
      c = 0; 
 
      Serial.println("c ="); 
      Serial.print(c); 
      digitalWrite(ledg, HIGH); 
      delay(500); 
      digitalWrite(led, LOW); 
      delay(6000); 
      } 
} 
void sendSMS()   
{  
GPRS.println("AT+CMGF=1");    // Set modem to text mode  
delay(2000); 
GPRS.write("AT+CMGS=");      // Start composing message   
GPRS.write(0x22);            // hex equivalent of double-quote '"'  
GPRS.write("+9779813623309"); // the number to send the SMS to   
GPRS.write(0x22);  GPRS.write(0x0D);            // hex equivalent of Carraige return   
GPRS.write(0x0A);            // hex equivalent of newline   
delay(2000);  
GPRS.print("ID:=1 LOCATION:=KU DOCSE : THE ABOVE DUSTBIN REACHED ITS MAXIMUM CAPACITY PLEASE COME AND COLLECT TO KEEP CITY CLEAN");  // Send the text message to the GSM module   
GPRS.write(0x0D);            // hex equivalent of Carraige return   
GPRS.write(0x0A);            // hex equivalent of newline   
delay(1000);  
GPRS.write(26);        // equivalent of CTRL-Z   
delay(5000); 
} 