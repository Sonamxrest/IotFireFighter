#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SoftwareSerial.h>
#define forward 50
#define left 52
#define right 53
#define LM1 2       // left motor
#define LM2 3       // left motor
#define RM1 4       // right motor
#define RM2 5 
#define pump 6
int TRIGGER= 9;
int ECHO = 10;
 #include <Servo.h>
SoftwareSerial SIM900(7, 8); 
Servo myservo;
String latitude="";
String longtitute="";
TinyGPSPlus gps;
static const int RXPin =1, TXPin =0;
static const uint32_t GPSBaud = 9600;
SoftwareSerial ss(RXPin, TXPin);


int fire =0;
int dis(){

  long duration;
  long distance;
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
   ss.begin(115200);
  SIM900.begin(19200);
  myservo.attach(11);
  myservo.write(90);

 pinMode(forward,INPUT);
  pinMode(left,INPUT);
    pinMode(right,INPUT);
  
 pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
    pinMode(pump, OUTPUT);
   pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(pump,HIGH);
}

void loop() {

 displayInfo();
digitalWrite(pump,HIGH);
while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
//      Serial.print("Latitude= "); 

   String a =String(gps.location.lat(),6)+","+String(gps.location.lng(),6);
 
      Serial.println(a);
      
    }
  }
  
  // put your main code here, to run repeatedly:
  int distance = dis();
//Serial.println(dis());
 
Serial.println(distance);
   if (distance <= 8)
  {
    digitalWrite(LM1,HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW); 
}
else{


  if(digitalRead(forward)==LOW)
  {fire = 1;
   Serial.println("Fire Forward");
       digitalWrite(LM1,LOW);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, HIGH); 
    
   
  }
  else if(digitalRead(left)==LOW)
  { fire =1;

         digitalWrite(LM1,HIGH);
      digitalWrite(LM2, HIGH);
      digitalWrite(RM1, LOW);
      digitalWrite(RM2, HIGH);
  }else if(digitalRead(right)==LOW)
  {
 Serial.println("Right");
   fire =1;
      digitalWrite(LM1, LOW);
      digitalWrite(LM2, HIGH);
      digitalWrite(RM1, LOW);
     
      digitalWrite(RM2, LOW);
      
       
  }
  else{
    fire =0;
     digitalWrite(LM1,LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW); 
    
    
    Serial.println("No fire detected");
    
    }
    
    
    
    
    while(fire == 1){

    
     off();
     callSomeone();
     sendSms();
     
     
   
      }
      
    
    
  
  



  //backwards
//  digitalWrite(LM1, HIGH);
//  digitalWrite(LM2, LOW);
//  digitalWrite(RM1,HIGH);
//  digitalWrite(RM2, LOW);
//  delay(3000);
}
  
}
void off(){

 
    
  delay(1500);
   digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1,LOW);
  digitalWrite(RM2, LOW);
    int pos =50;
  digitalWrite(pump, LOW);
  
    for (pos = 0; pos <= 180; pos += 1) {
      
    myservo.write(pos);
delay(20);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(20);

  }
  

  delay(1000);
  digitalWrite(pump,HIGH);
  fire =0;
   
}



void sendSms() {
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT+CMGS=\"+9779818780177\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println("There is fire in  27.706250, 85.330028"); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
 
  // Give module time to send SMS
  delay(5000); 
  
}

  
  void callSomeone() {
    Serial.println("Calling");
  // REPLACE THE X's WITH THE NUMER YOU WANT TO DIAL
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("ATD + +9779818780177;");
  delay(100);
  SIM900.println();
  
 // In this example, the call only last 30 seconds
 // You can edit the phone call duration in the delay time
  delay(10000);
  // AT command to hang up
  SIM900.println("ATH"); // hang up
  Serial.println("Done calling");
  }

void displayInfo()
{
  
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
    {

      Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
latitude = gps.location.lat();
longtitute =gps.location.lng();
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
     Serial.print(F("INVALID"));
  }

 

  if (gps.date.isValid())
  {
      Serial.print(F("  Date "));  
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  
  Serial.println();
      }
      
  else if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);  }
  else{
    Serial.println("Hello World");
    }
  
}
