#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

// Replace with your network credentials
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

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  client.setCACert(root_ca);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // (you can also pass in a Wire library object like &Wire2)
}
int node=02;
float lat=65.023;
float lng=5.434;
int date = 110722;
int timee = 220033;

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;
    
    http.begin(serverName);
    
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
     String httpRequestData = "api_key=" + apiKeyValue + "&node=" + node
                          + "&lat=" + lat + "&lng=" + lng
                          + "&date=" + date + "&time=" + timee + "";
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
  //Send an HTTP POST request every 30 seconds
  delay(30000);  
}