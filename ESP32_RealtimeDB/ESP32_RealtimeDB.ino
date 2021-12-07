//#include <Arduino.h>
#include <WiFi.h>
//#include <HTTPClient.h>
//#include <time.h>

// line notification
//#include <TridentTD_LineNotify.h>
//#define LINE_TOKEN "5mUlpwBg7cbwlg7myUcJDgkOWkleTGVQvLRFJxiiCV0"


#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#define API_KEY "AIzaSyDC06NHOw7ZXuUneZsZvUcaW4PkfOMsNWc"
#define DATABASE_URL "https://smart-med-con-default-rtdb.asia-southeast1.firebasedatabase.app/"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
unsigned long count = 0;


char* ssid =  "NAV";//"ff8";
char* password = "12345678";//"Tekanawww9735";
void setup()
{

    Serial.begin(115200);

    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

    if (Firebase.signUp(&config, &auth, "", "")){
      Serial.println("ok");
    }
    else{
      Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }
    
    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

    //Or use legacy authenticate method
    //config.database_url = DATABASE_URL;
    //config.signer.tokens.legacy_token = "<database secret>";

    Firebase.begin(&config, &auth);

    //Comment or pass false value when WiFi reconnection will control by your code or third party library
    Firebase.reconnectWiFi(true);
}
FirebaseJson json;
FirebaseJson json2;
FirebaseJsonData result;

void loop()
{
    //Flash string (PROGMEM and  (FPSTR), String C/C++ string, const char, char array, string literal are supported
    //in all Firebase and FirebaseJson functions, unless F() macro is not supported.
    
    if (Firebase.ready() && (millis() - sendDataPrevMillis > 150000 || sendDataPrevMillis == 0))
    {
        sendDataPrevMillis = millis();

        //Serial.printf("Get json... %s\n", Firebase.RTDB.getJSON(&fbdo, "/alarms") ? fbdo.to<FirebaseJson>().raw() : fbdo.errorReason().c_str());
      
        //Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, "/test/json", &json) ? "ok" : fbdo.errorReason().c_str());
        //Firebase.RTDB.getJSON(&result,"/alarms");
        //FirebaseJson jVal;
        //Serial.printf("Get json ref... %s\n", Firebase.RTDB.getJSON(&fbdo, "/test/json", &jVal) ? jVal.raw() : fbdo.errorReason().c_str());
        //FirebaseJsonArray arr;
        //Serial.printf("Get array... %s\n", Firebase.RTDB.getArray(&fbdo, "/test/array") ? fbdo.to<FirebaseJsonArray>().raw() : fbdo.errorReason().c_str());

        Firebase.RTDB.getJSON(&fbdo, "/alarms");
        json2 = fbdo.to<FirebaseJson>();
        size_t len = json2.iteratorBegin();
        int hour, minute;
        for(size_t i = 0;i < len;i++){
          FirebaseJson::IteratorValue value = json2.valueAt(i);
//          Firebase.RTDB.getInt(&fbdo, "/alarms/"+String(value.key.c_str())+"/hour", &hour);
//          Firebase.RTDB.getInt(&fbdo, "/alarms/"+String(value.key.c_str())+"/minute", &minute);
          if(String(value.key.c_str()) == "hour") hour = atoi(value.value.c_str());
          if(String(value.key.c_str()) == "minute") {
            minute = atoi(value.value.c_str());
            Serial.println(hour);
            Serial.println(minute);
          }
          //Serial.printf("Alarms %s, Value %s, type %s",value.key.c_str(), value.value.c_str(), value.type == FirebaseJson::JSON_OBJECT ? "object" : "array");
        }
        Serial.println();
        json2.iteratorEnd();
        count++;
    }
}

//void loop()
//{
//    if (Firebase.ready() && (millis() - sendDataPrevMillis > 150000 || sendDataPrevMillis == 0))
//    {
//        if(Firebase.RTDB.getJSON(&result, "/alarms") ? result.to<FirebaseJson>().raw() : result.errorReason().c_str()){
//          json.get(result, "/alarms");
//          Serial.println(result.success);
//          Serial.println(result.type);
//          if(result.success){
//            result.get<FirebaseJson>(json2);
//            json2.toString(Serial,true);
//            size_t len = json2.iteratorBegin();
//            for(size_t i = 0;i < len;i++){
//              FirebaseJson::IteratorValue value = json2.valueAt(i);
//              Serial.printf("Alarms %s, Value %s, type %s",value.key.c_str(), value.value.c_str(), value.type == FirebaseJson::JSON_OBJECT ? "object" : "array");
//            }
//            Serial.println();
//            json.iteratorEnd();
//          }
