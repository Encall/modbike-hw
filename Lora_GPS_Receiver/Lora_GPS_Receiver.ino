#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#define ss 5
#define rst 27
#define dio0 14

const char* ssid     = "Encall Wifi";
const char* password = "0983522011";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "https://iot.encall.space/php-post-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "8CTyc0U0RVRDHdJ";

const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDxjCCA0ugAwIBAgISA7qUzhru5DNpqcutGaw88vG/MAoGCCqGSM49BAMDMDIx\n" \
"CzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQDEwJF\n" \
"MTAeFw0yMjEwMDQwNTEzMjRaFw0yMzAxMDIwNTEzMjNaMBkxFzAVBgNVBAMMDiou\n" \
"ZW5jYWxsLnNwYWNlMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEbNXXcVMtnwzn\n" \
"UFspU6lXwJAASB4Yak/8ZMZITAg7c7pvHq0aIJd3LCk/ELpOpb4A0+JhXAhzfVSQ\n" \
"purbmUeGiaOCAlgwggJUMA4GA1UdDwEB/wQEAwIHgDAdBgNVHSUEFjAUBggrBgEF\n" \
"BQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADAdBgNVHQ4EFgQULZAsudxvx82k\n" \
"ou14OQDoL8iSpBswHwYDVR0jBBgwFoAUWvPtK/w2wjd5uVIw6lRvz1XLLqwwVQYI\n" \
"KwYBBQUHAQEESTBHMCEGCCsGAQUFBzABhhVodHRwOi8vZTEuby5sZW5jci5vcmcw\n" \
"IgYIKwYBBQUHMAKGFmh0dHA6Ly9lMS5pLmxlbmNyLm9yZy8wJwYDVR0RBCAwHoIO\n" \
"Ki5lbmNhbGwuc3BhY2WCDGVuY2FsbC5zcGFjZTBMBgNVHSAERTBDMAgGBmeBDAEC\n" \
"ATA3BgsrBgEEAYLfEwEBATAoMCYGCCsGAQUFBwIBFhpodHRwOi8vY3BzLmxldHNl\n" \
"bmNyeXB0Lm9yZzCCAQUGCisGAQQB1nkCBAIEgfYEgfMA8QB2AHoyjFTYty22IOo4\n" \
"4FIe6YQWcDIThU070ivBOlejUutSAAABg6GgqlYAAAQDAEcwRQIhAIJaGEDjzUn8\n" \
"WBBgkZSoTGQIdkc0Zee7tQ4YxWdHKoc4AiBnKT6FwqJOBKqoqZnL5E67eqYYamt+\n" \
"B6kALMNqDGyNfgB3AK33vvp8/xDIi509nB4+GGq0Zyldz7EMJMqFhjTr3IKKAAAB\n" \
"g6Ggql4AAAQDAEgwRgIhAMU7lAH2jMIZN2KGEYAQkqoEjF0OgdkL9Zvx6fY1yL0M\n" \
"AiEAm1X5lbsMilCyqS+CDUhTdT8LePeW9e1PtHeLhcExcbkwCgYIKoZIzj0EAwMD\n" \
"aQAwZgIxAM3DUToZlxgjKVxWr7O6WPQxDy70GPqOFZiBDo2nX3jPXDFng8PJfE47\n" \
"liq60/cR3gIxAL5iq7vTzzTRsRbFhYeyj6crTkJ0wnueFJ0Ox+oP8uFyFy5IWFYL\n" \
"SXdsyu/TPv1W/A==\n" \
"-----END CERTIFICATE-----\n";

void setup() {
    Serial.begin(115200);
    while (!Serial);
    Serial.println("LoRa Receiver");

    LoRa.setPins(ss, rst, dio0);
    
    while (!LoRa.begin(433E6)) {
      Serial.println(".");
      delay(500);
    }
    LoRa.setSyncWord(0xF3);
    Serial.println("LoRa Initializing OK!");
    client.setCACert(root_ca);
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED) { 
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

}

void loop() {
//////////////////////LORA Receive//////////////////////////////////////
    String str = "";
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      while (LoRa.available()) {
        String LoRaData = LoRa.readString();
        str = LoRaData;
//////////////////////Split text//////////////////////////////////////
// String  str = "01#30.240455#-97.817713#3/9/2013#4:52:52";
        String strs[20];
        int StringCount = 0;
        while (str.length() > 0)
        {
          int index = str.indexOf('#');
          if (index == -1)
          {
            strs[StringCount++] = str;
            break;
          }
          else
          {
            strs[StringCount++] = str.substring(0, index);
            str = str.substring(index+1);
          }
        }
//////////////////////Split text//////////////////////////////////////
//////////////////////upload to sql//////////////////////////////////////
  
        if(WiFi.status()== WL_CONNECTED){

          HTTPClient http;
          
          http.begin(serverName);
          
          http.addHeader("Content-Type", "application/x-www-form-urlencoded");
          
          String httpRequestData = "api_key=" + apiKeyValue + "&node=" + strs[0]
                                + "&lat=" + strs[1] + "&lng=" + strs[2]
                                + "&date=" + strs[3] + "&time=" + strs[4] + "";
          Serial.print("httpRequestData: ");
          Serial.println(httpRequestData);
          
          int httpResponseCode = http.POST(httpRequestData);
          
              
          if (httpResponseCode>=-1) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
          }
          else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
          }
          // Free resources
          http.end();
        }
        else {
          Serial.println("WiFi Disconnected");
    } 
    //////////////////////upload to sql//////////////////////////////////////
        }
    }
  //////////////////////LORA Receive//////////////////////////////////////

}
