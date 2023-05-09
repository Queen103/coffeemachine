#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ezButton.h>
#include <Servo.h>
#include <SD.h>
#include "WiFi.h"
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


//define file 
File dataFile;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

//define lcd 
#define LCD_ADDRESS 0x3F // I2C address of the LCD
#define LCD_COLUMNS 16   // Number of columns in the LCD
#define LCD_ROWS 2       // Number of rows in the LCD

//define recipe by second
#define caphe 5000
#define sua 5000
#define duong 1000

//define gate
#define pump 25
#define servo 26
#define coffee1 2
#define coffee2 15
#define oyster 32
#define milkGate 33 

//servo sugar 
Servo servoSugar;
#define degreeSugar 30
#define timeDegree 1000

//define other
#define waiting_pump 1500
int y=0; // degress start

// define button
ezButton up(12);
ezButton down(13); 
ezButton enter(27);
ezButton back(14);
//define other 
int x=0;
int z=x;
unsigned long debounceDelay = 30;
unsigned long timeStart;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

// WiFi credentials
const char* ssid = "QUEEN";         // change SSID
const char* password = "1234567890";    // change password

String GOOGLE_SCRIPT_ID = "AKfycbwW5z1ctHt0Xcw8uc9KKtLgih5K_TSf4m8huq3F8UAfwmP8OUAiTZjX9JR7huP0OePg"; 

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// function 
// display on screen line1 and line 2
void screen(String line1, String line2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}
//write file 
void FileData(String text){
  if (!SD.begin(5)) {
    Serial.println("Không thể khởi tạo thẻ SD");
    return;
  }
  dataFile = SD.open("/data.txt", FILE_APPEND);
  if (dataFile) {
    dataFile.println(text);

    Serial.write(dataFile.read());
    dataFile.close();
    Serial.println("Đã ghi dữ liệu vào tệp");
  } else {
    Serial.println("Không thể mở tệp để ghi dữ liệu");
  }
}

//Class 
class Drink{
  public:
    int ip=1;
    String date;
    int type;
    int sugar;
    int milk;
    void display(){
      Serial.println(type);
      Serial.println(milk);
      Serial.println(sugar);
    }
};
Drink drink; 

void getDrink() {
  up.loop();
  down.loop();
  enter.loop();
  back.loop();
    switch(x){
      case 0:{
        screen("Press Enter","to Start");
        if(enter.isPressed()){
          x=10;
          timeStart = millis();
          digitalWrite(oyster,1);          
        }
        delay(60);
        break;
      }
      //screen 1
      case 10:{
        screen("Type","type 1");
        if(enter.isPressed()){
          x=20;
          drink.type=1;
        }
        if(down.isPressed()){
          x=11;
        }
        if(back.isPressed()){
          x=0;
          digitalWrite(oyster,0); 
        }
        delay(60);
        break;    
      }
      case 11:{
        screen("Type","type 2");
        if(enter.isPressed()){
          x=20;
          drink.type=2;
        }
        if(up.isPressed()){
          x=10;
        }
        if(back.isPressed()){
          x=0;
        }
        delay(60);
        break;    
      }
      //screen 2
      case 20:{
        screen("Sugar","0%");
        if(enter.isPressed()){
          x=30;
          drink.sugar=0;
        }
        if(down.isPressed()){
          x=21;
        }
        if(back.isPressed()){
          x=10;
        }
        delay(60);
        break;    
      }
      case 21:{
        screen("Sugar","30%");
        if(enter.isPressed()){
          x=30;
          drink.sugar=30;
        }
        if(down.isPressed()){
          x=22;
        }
        if(up.isPressed()){
          x=20;
        }
        if(back.isPressed()){
          x=10;
        }
        delay(60);
        break;    
      }
      // case 22:{
      //   screen("Sugar","50%");
      //   if(enter.isPressed()){
      //     x=30;
      //     drink.sugar=50;
      //   }
      //   if(down.isPressed()){
      //     x=23;
      //   }
      //   if(up.isPressed()){
      //     x=21;
      //   }
      //   if(back.isPressed()){
      //     x=10;
      //   }
      //   delay(60);
      //   break;    
      // }
      case 22:{
        screen("Sugar","70%");
        if(enter.isPressed()){
          x=30;
          drink.sugar=70;
        }
        if(down.isPressed()){
          x=24;
        }
        if(up.isPressed()){
          x=21;
        }
        if(back.isPressed()){
          x=10;
        }
        delay(60);
        break;    
      }
      case 24:{
        screen("Sugar","100%");
        if(enter.isPressed()){
          x=30;
          drink.sugar=100;
        }
        if(up.isPressed()){
          x=22;
        }
        if(back.isPressed()){
          x=10;
        }
        delay(60);
        break;    
      }
      //screen 3 
      case 30:{
        screen("Milk","0%");
        if(enter.isPressed()){
          x=40;
          drink.milk=0;
        }
        if(down.isPressed()){
          x=31;
        }
        if(back.isPressed()){
          x=20;
        }
        delay(60);
        break;    
      }
      case 31:{
        screen("Milk","30%");
        if(enter.isPressed()){
          x=40;
          drink.milk=30;
        }
        if(up.isPressed()){
          x=30;
        }
        if(down.isPressed()){
          x=32;
        }
        if(back.isPressed()){
          x=20;
        }
        delay(60);
        break;    
      }
      case 32:{
        screen("Milk","50%");
        if(enter.isPressed()){
          x=40;
          drink.milk=50;
        }
        if(up.isPressed()){
          x=31;
        }
        if(down.isPressed()){
          x=33;
        }
        if(back.isPressed()){
          x=20;
        }
        delay(60);
        break;    
      }
      case 33:{
        screen("Milk","70%");
        if(enter.isPressed()){
          x=40;
          drink.milk=70;
        }
        if(up.isPressed()){
          x=32;
        }
        if(down.isPressed()){
          x=34;
        }
        if(back.isPressed()){
          x=20;
        }
        delay(60);
        break;    
      }
      case 34:{
        screen("Milk","100%");
        if(enter.isPressed()){
          x=40;
          drink.milk=100;
        }
        if(up.isPressed()){
          x=33;
        }
        if(back.isPressed()){
          x=20;
        }
        delay(60);
        break;    
      }
      case 40:{
        screen("Are you","Sure!!!");
        if(enter.isPressed()){
          x=50;
        }
        if(back.isPressed()){
          x=30;
        }
        delay(60);
        break;    
      }
    }
}


void saveToSheet(){
  String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"date=" + drink.date + "&id=" + drink.ip + "&type="+drink.type+"&sugar="+drink.sugar+"&milk="+drink.milk;
  Serial.print("POST data to spreadsheet:");
  Serial.println(urlFinal);
  HTTPClient http;
  http.begin(urlFinal.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET(); 
  Serial.print("HTTP Status Code: ");
  Serial.println(httpCode);
  //---------------------------------------------------------------------
  //getting response from google sheet
  String payload;
  if (httpCode > 0) {
      payload = http.getString();
      Serial.println("Payload: "+payload);    
  }
  http.end();
}


//open gate control pump 
void openGate(int gate,int value,int percent){
  digitalWrite(gate,1);
  digitalWrite(pump,1);
  delay(value*percent/100);
  digitalWrite(gate,0);
  delay(waiting_pump);
  digitalWrite(pump,0);
}
//Servo 
void turnServo(int s){
  int t = s/30;
  for(int i=0;i<(6*t);i++){
    for(int posDegrees = y; posDegrees <= (y+degreeSugar); posDegrees++) {
      servoSugar.write(posDegrees);
      delay(5);
    }
    delay(100);
    for(int posDegrees = (degreeSugar+y); posDegrees >= y; posDegrees--) {
      servoSugar.write(posDegrees);
      delay(5);
    }
    delay(100);
  }
  
}
// void SETUP
void setup() {
  servoSugar.attach(servo);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(+6*60*60);

  up.setDebounceTime(50);
  down.setDebounceTime(50);
  enter.setDebounceTime(50);
  back.setDebounceTime(50);
  pinMode(pump,OUTPUT);
  pinMode(coffee1,OUTPUT);
  pinMode(coffee2,OUTPUT);
  pinMode(oyster,OUTPUT);
  pinMode(milkGate,OUTPUT); 

}
void loop(){
  //select drink 

  if (x!=50){
    getDrink();
    if(x!=z){
      z=x;
      timeStart=millis();
    }
    else{
      if(millis()-timeStart >=10000){
        x=0;
        digitalWrite(oyster,0);
      }
    }
  }
  else{
    screen("Producting","");
    turnServo(drink.sugar);
    delay(1000);
    openGate(milkGate,sua,drink.milk);
    delay(1000);
    if(drink.type == 1){
      openGate(coffee1,caphe,100);
    }
    else{
      openGate(coffee2,caphe,100);
    }
    delay(1000);
    digitalWrite(oyster,0);
    String text = (String)drink.ip+","+drink.type+","+drink.sugar+","+drink.milk;
    if (WiFi.status() == WL_CONNECTED){
      drink.date=timeClient.getFormattedTime();   
      saveToSheet();
      Serial.println("save to sheet");
    }
    else{
      FileData(text);
      Serial.println("save to SD");
    }
    // FileData(text);
    screen("DONE !!!!!!!!","");
    x=0;
    delay(3000);
  } 
  
}