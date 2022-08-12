  // 음성인식모듈 pin number
//#define rx2 16
//#define tx2 17

int pretty_word=0;

int voice_recogn= 0; // 음성인식 번호

#include <ESPDateTime.h>

//dht 관련
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 25    // Signal핀과 연결된 디지털 핀 넘
#define DHTTYPE DHT11   // DHT11 사용 명시
 
DHT dht(DHTPIN, DHTTYPE); // 사용핀넘버 타입 등록

//와이파이
#include <WiFi.h>
/* 1. Define the WiFi credentials */
#define WIFI_SSID "dorm306"
#define WIFI_PASSWORD "hdorm306"

//파이어베이스
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Define the api key 
#define API_KEY "AIzaSyD-2RObhpnjJkV1LC3lFZOmYfPSV0L41hE"

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "hwabooni-f82b1"


// 새로 입력해야 하는거!!!
// 이메일, 비밀번호, plant 문서번호

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "ndmeng12@gmail.com"
#define USER_PASSWORD "postit@1"

// 각 plant별 문서 번호
#define USER_PLANT "crusia"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;


unsigned long dataMillis = 0;
int count = 0;

int tmp11 = 0;

#define humidpin 39

int humidLevel=0;
int tempLevel=0;

String current_today="";

boolean today_voice=false;
boolean current_water=false;

void setup()
{
  Serial.begin(9600);
  // 음성인식모듈 시리얼통신
  Serial2.begin(9600);
  Serial.println("wait settings are in progress");
  delay(1000);
  Serial2.write(0xAA);
  Serial2.write(0x37);
  delay(1000);
  Serial2.write(0xAA);
  Serial2.write(0x21);
  Serial.println("The settings are complete");

  // 와이파이
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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

    pinMode(humidpin,INPUT); // 토양수분센서
    dht.begin(); // 온습도센서

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

    Firebase.begin(&config, &auth);
    
    Firebase.reconnectWiFi(true);

     DateTime.begin(/* timeout param */);
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  }
    
}

void loop()
{
  // 음성인식모듈
  while(Serial2.available())
  {
    Serial.println("voice input");
    voice_recogn=Serial2.read();
    }
   if(voice_recogn!=0){
    pretty_word++;
    Serial.println(pretty_word);
    voice_recogn=0;
   }


  int humidity = analogRead(humidpin); // 땅에 있는 습도
  //Serial.println(humidity);
  float hum = dht.readHumidity(); // 습도 읽기 / 공기중
  float temp = dht.readTemperature(); // 온도 읽기 / 공기중

  if (isnan(hum) || isnan(temp)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

   //결과 시리얼 출력  
//   Serial.print("Humidity: ");
//   Serial.print(hum);
//   Serial.print(" %\t");
//   Serial.print("Temperature: ");
//   Serial.print(temp);
//   Serial.println("C");



   //||calculateHumid(humidity)!=humidLevel||calculateTemp(temp)!=tempLevel
   if (Firebase.ready() && (millis() - dataMillis > 10000)|| dataMillis == 0||calculateHumid(humidity)!=humidLevel||calculateTemp(temp)!=tempLevel)
    {
      
        dataMillis = millis();

        //For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create.ino
        FirebaseJson content;

        //If the document path contains space e.g. "a b c/d e f"
        //It should encode the space as %20 then the path will be "a%20b%20c/d%20e%20f"
         

        String doc_path = "projects/";
        doc_path += FIREBASE_PROJECT_ID;
        doc_path += "/databases/(default)/documents/coll_id/doc_id"; //coll_id and doc_id are your collection id and document id
        
        //timestamp
        String str = DateTime.toString();
        str.replace(" ","T");
        str += "Z";
        content.set("fields/timestamp/timestampValue", str); //RFC3339 UTC "Zulu" format

        //하루가 지나면 리셋
        current_today=str.substring(11,16);

        //Serial.println(current_today);
        
        if(current_today.equals("15:00")){
          pretty_word=0;
          today_voice=false;
        }
        
        //We will create the nested document in the parent path "a0/b0/c0
        //a0 is the collection id, b0 is the document id in collection a0 and c0 is the collection id in the document b0.
        //and d? is the document id in the document collection id c0 which we will create.

        // plant 컬렉션의 record

        //double
        content.set("fields/temp/doubleValue", temp);

        //integer
        content.set("fields/humid/integerValue", String(humidity));
        
        String documentPath = "User/"+String(USER_EMAIL);
        documentPath+="/plant/"+String(USER_PLANT)+"/record/"+ str;

//        Serial.print("Create a document... ");

        if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw())){
          //Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());  
        }
        
        else{
          Serial.println(fbdo.errorReason());
        }

        //////////// plant 컬렉션의 prettyWord
        FirebaseJson content1;

        String documentPath1 = "User/"+String(USER_EMAIL);
        documentPath1+="/plant/"+String(USER_PLANT)+"/prettyWord/"+ str;

        if(pretty_word==1&&today_voice==false){
          content1.set("fields/prettyWord/integerValue", pretty_word);
          content1.set("fields/timestamp/timestampValue", str); //RFC3339 UTC "Zulu" format

          if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath1.c_str(), content1.raw())){
          //Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
          }
        }


        ////////////EVENT 컬렉션//////////////

        FirebaseJson content2;

        String documentPath2 = "User/"+String(USER_EMAIL);
        documentPath2+="/event/"+ str;

        // 물줬을때 이벤트
        if(calculateHumid(humidity)>humidLevel&&calculateHumid(humidity)==3&&current_water==false){
          content2.set("fields/type/integerValue", String(1));
          content2.set("fields/plant/stringValue", USER_PLANT);
          content2.set("fields/timestamp/timestampValue", str); //RFC3339 UTC "Zulu" format

          Serial.println("calculateHumid(humidity): "+String(calculateHumid(humidity)));
          Serial.println("humidLevel: "+String(humidLevel));

          current_water=true;
          
          if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath2.c_str(), content2.raw())){
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str()); 
          }
        }
        else{
          current_water=false;
        }

        // 예쁜말 여부
        if(pretty_word==1&&today_voice==false){
          content2.set("fields/type/integerValue", 2);
          content2.set("fields/plant/stringValue", USER_PLANT);
          content2.set("fields/timestamp/timestampValue", str); //RFC3339 UTC "Zulu" format
          today_voice=true;

          if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath2.c_str(), content2.raw())){
          //Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
          }
        }

    }
    humidLevel=calculateHumid(humidity);
    tempLevel=calculateTemp(temp);
  
}

int calculateHumid(int humid){

if(humid < 700+200){
  return 3;
}
else if(humid < 700+(4000-700)/3){
  return 0;
}
else if(humid < 700+(4000-700)/3*2){
  return 0;
}
else if(humid < 4000){
  return 0;
}
else if(humid >= 4000){
  return 0;
}

}


int calculateTemp(int temp){
  
if(temp < 15){
  return 0;
}
else if(temp < 15+(25-15)/3){
  return 1;
}
else if(temp < 15+(25-15)/3*2){
  return 2;
}
else if(temp < 25){
  return 3;
}
else if(temp >= 25){
  return 4;
}
  
}
