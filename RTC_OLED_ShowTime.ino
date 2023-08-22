#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Pin_define.h>
#include <Bounce2.h>    //最新版本2.71
#include <EEPROM.h>
#include "RTClib.h"  //至線上函式庫搜尋 RTClib 然後下載

RTC_DS3231 rtc;   //RTC 物件宣告，配合 RTClib.h 函式庫

//---------以下為OLED的各種參數設定------------
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET 4         // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


unsigned long old_time;

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  //啟動OLED，若失敗，程式會停在這裏不動
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever
  }
  
//啟動 RTC，若失敗，程式也會停在這裏不動
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  //----------OLED初始化顯示資料 2 秒-------------
  display.clearDisplay();
  showString("   NKHS", 00, 0, 1);
  showString("   C206", 00, 20, 1);
  display.display();
  delay(2000);

  //----------系統指示燈用時間紀錄----------
  old_time = millis();

}

void loop() {
  //---------系統LED每隔0.5秒閃爍一次--------------
  String y,m,d,hh,mm,ss,sdate,stime;
  
  if (millis() - old_time > 500) {
    old_time = millis();

    DateTime now = rtc.now();

    y=chgstring(String(now.year()),2);
    m=chgstring(String(now.month()),2);
    d=chgstring(String(now.day()),2);
    hh=chgstring(String(now.hour()),2);
    mm=chgstring(String(now.minute()),2);
    ss=chgstring(String(now.second()),2);
    sdate=y+"/"+m+"/"+d;
    stime=hh+":"+mm+":"+ss;
    display.clearDisplay();
    showString(sdate,0,0,1);
    showString(stime,0,16,1);   
    display.display();
  }
}

//--------把文字放進指定的 oled 記憶體位置(並不會顯示)-------
void showString(String s, int x, int y, int color) {
  
  display.setTextSize(2);               // Normal 1:1 pixel scale
  if (color == 1)
    display.setTextColor(SSD1306_WHITE);  // Draw white text
  else
    display.setTextColor(SSD1306_BLACK);  // Draw white text
  display.setCursor(x, y);              // Start at top-left corner
  display.println(s);
}

String chgstring(String s,int len){
  if(s.length()==1)
    return '0'+s;
  else
    return s;
}
