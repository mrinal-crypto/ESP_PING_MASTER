#include <WiFi.h>
#include <ESP32Ping.h>
#include "time.h"
#include <LiquidCrystal.h>
LiquidCrystal My_LCD(13, 12, 14, 27, 26, 25);
#define yellow 15
#define blue 4
#define green 2
#define red 16
#define buz 17
int flag = 0;
int flag2 = 0;

const int ledfreq = 5000;
const int buzfreq = 450;
const int leddutyCycle = 80;
const int buzdutyCycle = 0;

const int redChannel = 3;
const int greenChannel = 2;
const int blueChannel = 1;
const int yellowChannel = 0;
const int buzChannel = 4;
const int resolution = 8;

const char* ssid     = "MRINAL";
const char* password = "mrinal maity";

const IPAddress remote_ip(1, 1, 1, 1);
const char* remote_host = "www.google.com";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

void setup() 
{
  pinMode(yellow, OUTPUT);//yellow
  pinMode(blue, OUTPUT);//green
  pinMode(green, OUTPUT);//blue
  pinMode(red, OUTPUT);//red
  pinMode(buz, OUTPUT);//buzzer
  
  ledcSetup(redChannel, ledfreq, resolution);
  ledcSetup(greenChannel, ledfreq, resolution);
  ledcSetup(blueChannel, ledfreq, resolution);
  ledcSetup(yellowChannel, ledfreq, resolution);
  ledcSetup(buzChannel, buzfreq, resolution);

  ledcAttachPin(green, greenChannel);
  ledcAttachPin(red, redChannel);
  ledcAttachPin(blue, blueChannel);
  ledcAttachPin(yellow, yellowChannel);
  ledcAttachPin(buz, buzChannel);

  
  for(int i = 0; i<=3; i++)
  {
    ledcWrite(i, 250);
    delay(250);
    ledcWrite(i, 0);
    delay(250);
  }
  for(int i = 0; i<=3; i++)
  {
    ledcWrite(0, 250);
    ledcWrite(1, 250);
    ledcWrite(2, 250);
    ledcWrite(3, 250);
    delay(250);
    ledcWrite(0, 0);
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    ledcWrite(3, 0);
    delay(250);
  }
  
  My_LCD.begin(20, 4);
  My_LCD.clear();
  My_LCD.setCursor(3,1);
  My_LCD.print("ESP PING-MASTER");
  delay(4000);
  My_LCD.clear();
  
  My_LCD.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    ledcWrite(blueChannel, leddutyCycle);
    delay(250);
    ledcWrite(blueChannel, 0);
    delay(250);
  }
  ledcWrite(yellowChannel, leddutyCycle);
  My_LCD.setCursor(5,2);
  My_LCD.print("Connected!");
  delay(1000);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    My_LCD.setCursor(0,2);
    My_LCD.print("Time Failed");
    return;
  }
  My_LCD.setCursor(0,2);
  My_LCD.print(&timeinfo, "%b %d %Y %H:%M:%S");
}

void ipCheck()
{
  My_LCD.setCursor(0,0);
  My_LCD.print("IP: ");
  My_LCD.setCursor(4,0);
  My_LCD.print(WiFi.localIP());
  My_LCD.setCursor(0,1);
  My_LCD.print("Ping: ");
  My_LCD.setCursor(6,1);
  My_LCD.print(remote_host);
}

void wifiCheck()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    ledcWrite(yellowChannel, 0);
   
    My_LCD.clear();
  
    My_LCD.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    ledcWrite(blueChannel, leddutyCycle);
    delay(250);
    ledcWrite(blueChannel, 0);
    delay(250);
  }
    ledcWrite(yellowChannel, leddutyCycle);
    My_LCD.setCursor(5,2);
    My_LCD.print("Connected!");
    delay(1000);
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  }
}

void pingTest()
{
    while(flag != 1)
  {
    ledcWrite(blueChannel, leddutyCycle);

    if(flag2 == 1)
    {
      My_LCD.setCursor(0,3);
      My_LCD.print("Online");
    }
    if(flag2 == 0)
    {
      My_LCD.setCursor(0,3);
      My_LCD.print("No internet");
    }
    
    if(Ping.ping(remote_host))//if(Ping.ping(remote_ip)) 
    {
      flag = flag + 1;
      flag2 = 1;
      ledcWrite(blueChannel, 0);
      ledcWrite(greenChannel, leddutyCycle);
      My_LCD.setCursor(0,3);
      My_LCD.print("Online");
      My_LCD.setCursor(16,3);
      My_LCD.print(WiFi.RSSI());
     
    }
    else
    {  
      flag = flag + 1;
      flag2 = 0;
      ledcWrite(blueChannel, 0);
      ledcWrite(redChannel, leddutyCycle);
      ledcWrite(buzChannel, buzdutyCycle);
      My_LCD.setCursor(0,3);
      My_LCD.print("No internet");
      My_LCD.setCursor(16,3);
      My_LCD.print(WiFi.RSSI());
    }
  
  }
}

void loop() 
{
  wifiCheck();
  My_LCD.clear();
  printLocalTime();
  ipCheck();
  pingTest();
  printLocalTime();
  delay(2000);
  ledcWrite(greenChannel, 0);
  ledcWrite(redChannel, 0);
  ledcWrite(buzChannel, 0);
  flag = 0;
}