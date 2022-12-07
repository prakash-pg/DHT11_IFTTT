/* 
ESP8266 DHT11 sensor to send you temp and humidity notifications via IFTTT
Github here: https://github.com/Michael-Coetzee/esp8266_dht11.git 
GPIO16 must be tied to reset to wake!
Video explains: https://www.youtube.com/watch?v=Q8GT-GBH7ok
DHT Signal> D3 or GPIO0 
DHT VCC> 3V
DHT GND> GND
RST> D0 for wakeup (GPIO16/WAKE on NodeMCU)
NOTE: RST line wakeup Jumper must be removed to program!!!!!!!!!!!!!!
*/

#include <WiFi.h>
const char* ssid     = "GNXS-2EC0D0";      // SSID of local network---------------------USER Input here
const char* password = "9629090034";   // Password on network -----------------------------------------------------
String result;
float h = 111;
float t = 222;
#include "DHT.h"
#define DHTPIN 4    // GPIO 0 on ESP12E Lolin is labelled pin D3 
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
#define DHT_max_temp 45
#define DHT_max_humid 70
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

const char host[ ]        = "maker.ifttt.com";
const char trigger[ ]     = "dht_11Sensor";
const char APIKey[ ]      = "X1pWRSCxEm454oL2k_I0XsY9e7GVZN1lXCARohxKTX";

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.

DHT dht(DHTPIN, DHTTYPE);
void setup() 
{
  Serial.begin(115200);
  Serial.setTimeout(2000);
  WiFi.hostname("dht_11Sensor");
  WiFi.begin(ssid, password);
  dht.begin();
  delay(2000);
  
 
}
void loop()
{
   while (true) {
  h = dht.readHumidity();
  t = dht.readTemperature();
  delay(1000);
  WiFiClient client;
   if (!client.connect(host, 80)) {
    Serial.println("connection ");
    delay(100);
   }
   
    
  
  if (t >= DHT_max_temp) {
      Serial.print("Temp High!");
      Serial.print(t);
      String url = String("/trigger/") + trigger + String("/with/key/") + APIKey + String("?value1=") + String("TemperatureHigh") + String("&value2=") + String("Temp=") + t + String("&value3=") + String("Humidity=") + h;
      client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      delay(20);
      client.stop();
      ESP.deepSleep(30e6);
    } else if (h >= DHT_max_humid) {
      Serial.print("Humidity High!"); 
      Serial.print(h);     
      String url = String("/trigger/") + trigger + String("/with/key/") + APIKey + String("?value1=") + String("HumidityHigh") + String("&value2=") + String("Temp=") + t + String("&value3=") + String("Humidity=") + h;
      client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      delay(20);
      client.stop();
      ESP.deepSleep(30e6);   
    } else {
      Serial.println("Nominal!");
      Serial.print(t);
      delay(20);
      Serial.print(h);
      delay(300);
      ESP.deepSleep(30e6);
    }  }
   
}
