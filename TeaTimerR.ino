// Ngan Tran
// Tea Timer(r)


#include "IRremote.h"
#include "TimerFreeTone.h"
#include <Servo.h>
#include <LiquidCrystal.h>


// initialize the library with the numbers of the interface pins
int receiver = 13;
int posOne = 0;
int buzzer = 4;
boolean makingTea = LOW;
int buttonState = 0;
int start;
int startPos = 180; 
int lowPos = 125; 
int midPos = 150;

Servo servo;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
IRrecv irrecv(receiver);
decode_results results;

int melody[] = { 2000, 2000 };
int duration[] = { 200, 125 };

int minutes = 0;
int seconds = 0;
String teaName = " "; 


void setup() {
  Serial.begin(9600);
  
  irrecv.enableIRIn(); // Start the receiver
  
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.noDisplay();
}
  
void loop() {  
  if (irrecv.decode(&results)) { //IR remote cases 
    remoteIndicator(); 
    
    switch (results.value){
      case 0xFFA25D: // POWER
      lcd.clear();
      lcd.display();
      lcd.print("it's tea time :) ");
      delay(4000);
      lcd.clear();
      lcd.print("choose tea type");
      lcd.setCursor(0,1);
      lcd.print("  using keypad");
      break;
    }

    switch (results.value){
      case 0xFF02FD: //PLAY PAUSE
      lcd.clear();
        if(start == 0 && minutes !=0){
        start = 255;
        }
        else{
          start = 0;
          lcd.setCursor(3,0);
          lcd.println("select tea");
        }
      break;
    }

    switch(results.value){
      case 0xFF30CF: // 1
      lcd.clear();
      teaName = "   White  Tea"; 
      lcd.print(teaName);
      lcd.setCursor(0,1);
      lcd.print("      2:00");
      minutes = 2;
      seconds = 0;
      Serial.println(minutes);
      Serial.println(seconds);
      break;
    }

    switch(results.value){
      case 0xFF18E7: // 2
      lcd.clear();
      teaName = "   Green  Tea";
      lcd.print(teaName);
      lcd.setCursor(0,1);
      lcd.print("      3:00");
      minutes = 3;
      seconds = 0; 
      Serial.println(minutes);
      Serial.println(seconds);
      break;
    }

    switch(results.value){
      case 0xFF7A85: // 3
      lcd.clear();
      teaName = "     Oolong"; 
      lcd.print(teaName);
      lcd.setCursor(0,1);
      lcd.print("      4:00");
      minutes = 4;
      seconds = 0;
      Serial.println(minutes);
      Serial.println(seconds);
      break;
    }

    switch(results.value){
      case 0xFF10EF: // 4
      lcd.clear();
      teaName = "   Black  Tea"; 
      lcd.print(teaName);
      lcd.setCursor(0,1);
      lcd.print("      4:30");
      minutes = 4;
      seconds = 30;
      Serial.println(minutes);
      Serial.println(seconds);
      break;
    }

     switch(results.value){
      case 0xFF38C7: // 5
      lcd.clear();
      teaName = "   Herbal Tea";
      lcd.print(teaName);
      lcd.setCursor(0,1);
      lcd.print("      6:00");
      minutes = 6;
      seconds = 0;
      Serial.println(minutes);
      Serial.println(seconds);
      break;
    }
    irrecv.resume(); // receive the next signal
  }  

  if(start > 0){
    Serial.println("Start button pressed");
    servo.attach(6); // attaches the servo on pin 6 to the servo object 
    servo.write(startPos);
    makingTea = HIGH; 
    lcd.clear();
    lcd.print("  time to brew!");
    digitalWrite(5, HIGH); // turn on brewing light 
    delay(2000);

    for (int i = startPos; i >= lowPos; i--){
      servo.write(i);
      delay(50);
    }

    dunkTea();
    
    lcd.clear();
  }

  if (makingTea == HIGH){
   lcd.print(teaName);  
   while (minutes > 0 || seconds > 0){
     lcd.setCursor(6,1);
     lcd.print(minutes);
     lcd.print(":");
      if (seconds < 10){
        lcd.print("0"); 
      }
     lcd.print(seconds);
     lcd.display(); 
     countDown();
     delay(1000);
    }
   lcd.setCursor(6,1);
   lcd.print("0:00");

   done();
  }  
  
  makingTea = LOW;
  start = 0; //end loop 
}


void done(){
  dunkTea();
  
  for (int i = lowPos; i < startPos; i++){
    servo.write(i);
    delay(80);
  }
  //servo.detach(); // detach the servo from power 
  lcd.clear();
  lcd.print("* tea is ready *");
  digitalWrite(5, LOW); //turn off brewing light

  for (int i = 0; i < 10; i++){
    for(int tones = 0; tones < 2 ; tones++){
      TimerFreeTone(buzzer, melody[tones], duration[tones]); // Play melody for duration.
      digitalWrite(3, HIGH);
      delay(500);
      digitalWrite(3, LOW);
      delay(500);
    }
  }
}

void remoteIndicator(){
  digitalWrite(5, HIGH);
  digitalWrite(5, LOW);
}


void countDown(){
  if (seconds > 0){
    seconds -= 1;
  }
  else if(minutes > 0){
    seconds = 59;
    minutes -= 1;
  }
}

void dunkTea(){
  for (int i = 0; i < 4; i++){
    for (int i = lowPos; i < midPos; i++) { 
      servo.write(i);              
      delay(40);   
      }
    for (int i = midPos; i > lowPos; i--){ 
      servo.write(i);
      delay(40);
    } 
  }
}

