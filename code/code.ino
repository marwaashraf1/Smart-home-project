#include <LiquidCrystal.h>
#define TREG 0
#define ECHO 1
#define buzzer A2
#define control A3
#define ACD A4


#include <EEPROM.h>
#include <Keypad.h>

#define num_rows 4
#define num_cols 4
LiquidCrystal lcd(7,6,5,4,3,2);
char keymap [num_rows][num_cols] = {
  {'7', '8', '9', 'X'},
  {'4', '5', '6', '/'},
  {'1', '2', '3', '-'},
  {'C', '0', '=', '+'}
};
  
//byte rowPins[num_rows] = {8, 9, 10, 11};
//byte colPins[num_cols] = {12, 13, A0, A1};
byte rowPins[num_rows] = {A1, A0, 13, 12};
byte colPins[num_cols] = {11, 10, 9, 8};

Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, num_rows, num_cols);

void setup() {
  lcd.begin(16,2);
  lcd.clear();
  pinMode(TREG,OUTPUT);
  pinMode(ECHO,INPUT);
  pinMode(buzzer, OUTPUT);
   EEPROM.write(0, 0);
   pinMode(control,OUTPUT);
   pinMode(ACD,INPUT);
   digitalWrite(control,LOW);

}
void writeStringToEEPROM(int location, const String &stringToWrite);
String readStringFromEEPROM(int location);
int soilmoisture();

float duration , distanceCM , distanceM ;
void loop() {
   digitalWrite(TREG,LOW);
  delayMicroseconds(2);
    digitalWrite(TREG,HIGH);
      delayMicroseconds(10);
        digitalWrite(TREG,LOW);
duration =pulseIn(ECHO,HIGH);
distanceCM = duration * 0.5 *0.034 ;
distanceM = distanceCM / 100.0; 

if (distanceM <= 0.5)
{
 lcd.clear();


 if(EEPROM.read(0) != 1){
    lcd.setCursor(0, 0);
    lcd.print("New Password: ");
    lcd.setCursor(0, 1);
    lcd.print("=> ");
    String password = "";
    while(true){
      char key = keypad.getKey();
      if(key == '=' ){
        EEPROM.write(0, 1);
        EEPROM.write(1, 0);
        writeStringToEEPROM(2, password);
        break;
      }
      else if(key) {
        password += String(key);
        lcd.print("*");
      }
    }
    lcd.clear();
    lcd.print("The password is");
    lcd.setCursor(0, 1);
    lcd.print("Set Successfully!");
  }
  else{
    int num_of_trials = EEPROM.read(1);
    String password = readStringFromEEPROM(2);
    bool isCorrect = false;

    for(int i = num_of_trials; i < 3; i++){
      delay(1000);
      lcd.clear();
      lcd.print("Enter password: ");
      lcd.setCursor(0, 1);
      lcd.print("=> ");
      String userPass = "";

      while(true){
        char key = keypad.getKey();
        if(key == '='){
          if(userPass.equals(password)){
            isCorrect = true;
            lcd.clear();
            lcd.print("Correct");
            EEPROM.write(1, 0);
            
          }else{
            lcd.clear();
            lcd.print("incorrect");
            EEPROM.write(1, i + 1);
          }
          break;
        } else if (key){
            userPass += String(key);
            lcd.print("*");
          }
      }
      if (isCorrect){
        break;
      }
    }
    if(isCorrect){
      
        lcd.clear();
        lcd.print("Welome Home!");
        delay(2000);
        lcd.clear();
       
      
    int soil = soilmoisture();
    lcd.print("moisture : " + String(soil));
    delay(25000);

    } else{
      lcd.clear();
      lcd.print("Alarm");
      digitalWrite(buzzer, true);
      while(true);
    }
  }
 }
 else
 {
    lcd.clear();
 }
 
 

}
 
 



void writeStringToEEPROM(int location, const String &stringToWrite){
  byte len = stringToWrite.length();
  EEPROM.write(location, len);
  for(int i = 0;i < len; i++){
    EEPROM.write(location + 1 + i, stringToWrite[i]);
  }
}


  
String readStringFromEEPROM( int location){
  int len = EEPROM.read(location);
  char string_arr[len + 1];
    
    
  for (int i = 0; i < len;  i++){
    string_arr[i] = EEPROM.read(location + 1 + i);
  }
  string_arr[len] = '\0';
  return String(string_arr);
}
int soilmoisture(){
  digitalWrite(control,HIGH);
  delay(25);
  int soil = analogRead(ACD);
  delay(25);
  digitalWrite(control,LOW);
  return soil;
  }
