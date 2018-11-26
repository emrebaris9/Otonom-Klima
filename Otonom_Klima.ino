#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


#define DHTPIN 2
#define FANPIN 14
#define DHTTYPE DHT11
int counter=0;
String API = "CMZFVRG5ZW84Y1H9";
const char* ssid = "Xperia";
const char* password = "dell2345";
int tweetsayi = 0;
String sTweetSayi = String(tweetsayi);
String tweet =  sTweetSayi + "+Dk+Sistem+Baslatildi+Sicaklik+(C):+";
boolean fan1;
float s = 0.0;
int sicaklik_alt = 23;
int sicaklik_ust = 24;




DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.println("Otonom klima Başlatılıyor...");
  //ticker.attach(60, isrFunc);
  pinMode(FANPIN, OUTPUT);
  digitalWrite(FANPIN, HIGH);
  Serial.begin(115200);
  fan1 = false;
  s = dht.readTemperature();
  String ss = String(s);
  Serial.println("sicaklik" + ss);
  tweet_at();
  sTweetSayi = String(tweetsayi);
  tweet =  sTweetSayi + "+Dk+Sistem+Baslatildi+Sicaklik+(Der):+";

}
void loop()
{
  delay(10000);
  counter++;
  if(counter==6){
    tweet_at();
    counter=0;
  }
  Serial.println("\n");
  float n = dht.readHumidity();//nem
  s = dht.readTemperature();//sıcaklık
  if (s >= sicaklik_ust) {
    digitalWrite(FANPIN, LOW); //0 İse Fan çalışıyor
    fan1 = true;
  }
  else if (s <= sicaklik_alt && fan1) {
    digitalWrite(FANPIN, HIGH);
  }
  Serial.print("Nem (%): ");
  Serial.println(n);
  Serial.print("Sıcaklık (Der): ");
  Serial.println(s);
  
}
void isrFunc() {
  tweet_at();
  Serial.print("kesmeye girdi");
  return;
}
void tweet_at() {
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("deneniyor..");
    delay(500);
  }
  WiFiClient client;
  Serial.print("bağlati bas");
  while (!client.connect("184.106.153.149", 80)) {
    delay(500);
  }
  Serial.print("baglandi");
  String ss = String(s);
  client.print("GET /apps/thingtweet/1/statuses/update?key=" + API + "&status=" + tweet + "+" + ss + " HTTP/1.1\r\n");
  client.print("Host: api.thingspeak.com\r\n");
  client.print("Accept: */*\r\n");
  client.print("User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n");
  client.print("\r\n");
  Serial.println();
  Serial.println(tweet);
  tweetsayi = tweetsayi + 1;
  sTweetSayi = String(tweetsayi);
  tweet =  sTweetSayi + "+Dk+Sistem+Baslatildi+Sicaklik+(Der):+";
  
  client.stop();
  client.flush();
  WiFi.disconnect();
  return;
  
}

