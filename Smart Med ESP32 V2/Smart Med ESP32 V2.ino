#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <TridentTD_LineNotify.h>

#define LINE_TOKEN "5mUlpwBg7cbwlg7myUcJDgkOWkleTGVQvLRFJxiiCV0"
const char* ssid = "ff8";
const char* password = "Tekanawww9735";
//Your Domain name with URL path or IP address with path
String serverName = "http://ikwmystery.atwebpages.com";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 0;

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

bool eat = false;
int timeOutMin = 2; // minutes
int noBottleStart = 0;
const int LDR = 34;
int status = 0; // 0=ok  1=it's time 2=forgot 3=placeBottleBack
int previousStatus = 0;
int alarms;
bool sw = 0;
int ldrVal;

struct tm alarmTime[20];
struct tm timeNow;
struct tm timeOut;

TaskHandle_t Task1;
TaskHandle_t Task2;

void getTime(){
  struct tm timeinfo; 
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
  }
  timeNow = timeinfo;
}

void getTimeSet(){
  HTTPClient http;
  String serverPath = serverName + "/timeSet.txt";
  http.begin(serverPath.c_str());
  int httpResponseCode = http.GET();
  if (httpResponseCode>0) {
    String cod = http.getString();
    char *str = new char[cod.length() + 1];
    strcpy(str, cod.c_str());
    int arr[5];
    char *p = strtok(str, ",");
    size_t index = 0;
    int arrSize = atoi(p);
    p = strtok(NULL, ",");
    while (p != nullptr && index < arrSize) {
      arr[index++] = atoi(p);
      p = strtok(NULL, ",");
    }

    alarms = arrSize/2;
    for(int i = 0;i < alarms;i++){
      alarmTime[i].tm_hour = arr[2*i];
      alarmTime[i].tm_min = arr[2*i+1];
    }
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void notifyStatus(int sta){
  if(WiFi.status()== WL_CONNECTED){
    Serial.print("sending ");
    Serial.println(sta);
    HTTPClient http;
    String serverPath = serverName;
    if(sta == 1) serverPath += "/SMC.php?send=1";
    if(sta == 2) serverPath += "/SMC.php?send=2";
    if(sta == 0) serverPath += "/SMC.php?send=3";
    if(sta == 3) serverPath += "/SMC.php?send=4";
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode>0) {
      //Serial.print("HTTP Response code: ");
      //Serial.println(httpResponseCode);
      String payload = http.getString();
      //Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  lastTime = millis();
  if(sta == 0) LINE.notify("OK");
  if(sta == 1) LINE.notify("Time to take medicine");
  if(sta == 2) LINE.notify("Forgot to take medicine, take it now");
  if(sta == 3) LINE.notify("Place Med Container back");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  LINE.setToken(LINE_TOKEN);
  
  getTime();
  getTimeSet();
  Serial.println(alarmTime[0].tm_hour);
  Serial.println(alarmTime[0].tm_min);
  Serial.println(timeNow.tm_hour);
  Serial.println(timeNow.tm_min);
  
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */                  
  delay(500);
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
  delay(500);
}

void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  Serial.println(status);
  
  for(;;){
    getTime();
    getTimeSet();
    Serial.print(timeNow.tm_hour);
    Serial.print(" ");
    Serial.println(timeNow.tm_min);

    for(int i = 0;i < alarms;i++){
      if(timeNow.tm_hour == alarmTime[i].tm_hour && timeNow.tm_min == alarmTime[i].tm_min){
        status = 1;
        Serial.println(status);
        timeOut.tm_hour = alarmTime[i].tm_hour + 0;
        timeOut.tm_min += alarmTime[i].tm_min + timeOutMin;
        if(timeOut.tm_min >= 60) {
          timeOut.tm_min -= 60; timeOut.tm_hour += 1;
        }
      }
    }
    ldrVal = analogRead(LDR);
    if(ldrVal < 4080) noBottleStart++;
    else noBottleStart = 0;
    if(noBottleStart >= 5){
      notifyStatus(3);
    }
    delay(60000);
  } 
}

void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  Serial.println(status);

  for(;;){
    if(status == 1){
      Serial.println("eatttttt");
      notifyStatus(1);
      eat = false;
      while(eat == false){
        ldrVal = analogRead(LDR);
        Serial.println(ldrVal);
        eat = true;
        for(int i = 0;i < 10;i++){
          ldrVal = analogRead(LDR);
          if(ldrVal >= 4080) eat = false;
          delay(100);
        }
        Serial.print("timeOut ");
        Serial.print(timeOut.tm_min);
        Serial.print(" ");
        Serial.println(timeOut.tm_hour);
        if(timeNow.tm_min == timeOut.tm_min && timeNow.tm_hour == timeOut.tm_hour && status != 2){
          status = 2;
          notifyStatus(2);
        }
      }
      status = 0;
      notifyStatus(0);
    }
    delay(1000);
  }
}

void loop() {
}