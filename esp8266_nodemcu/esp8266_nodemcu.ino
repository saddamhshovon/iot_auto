#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>
#include <addons/RTDBHelper.h> // Provide the RTDB payload printing info and other helper functions.
#include "DHT.h"               // DHT temperature and humidity sensor Predefined library

/* Define the WiFi credentials */
#define WIFI_SSID "XX_XX_XX"
#define WIFI_PASSWORD "XX_XX_XX"

/* If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "https://XX_XX_XX.firebaseio.com/" // <databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "XX_XX_XX"

/* Define DHT22 sensor & pin */
#define TMPHUM DHT22
#define tmphum_pin 0

/* Define DHT object */
DHT tmphum(tmphum_pin, TMPHUM);

float temp, humi;

/* Define the Firebase Data object */
FirebaseData fbdo;

/* Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(9600);

  tmphum.begin();

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

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the database URL and database secret(required) */
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;

  Firebase.reconnectWiFi(true);

  /* Initialize the library with the Firebase authen and config */
  Firebase.begin(&config, &auth);
}

void loop()
{
  FirebaseJson json, json2;

  temp = tmphum.readTemperature();
  humi = tmphum.readHumidity();

  json2.add("temp", temp).add("humi", humi);

  if (!Firebase.RTDB.set(&fbdo, "/XX_XX", &json2)) // '/XX_XX' firebase node to set temperature & humidity
  {
    Serial.println(fbdo.errorReason().c_str());
  }

  if (Firebase.RTDB.getJSON(&fbdo, "/XXX_XXX")) // '/XXX_XXX' firebase node to get directional inputs from app
  {
    json = fbdo.to<FirebaseJson>().raw();
    json.toString(Serial1, true);
  }
  else
  {
    Serial.println(fbdo.errorReason().c_str());
  }
}
