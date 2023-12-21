/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <esp_int_wdt.h>
#include <esp_task_wdt.h>
#include <esp_wifi.h>
#include <esp_bt.h>

const char* ssid = "Internet";
const char* password = "244466666";
bool OTA = false;

WebServer server(80);

//const int led = 13;
void handleRoot() {
  server.send(200, "text/html", "G&uumlltige Befehle:<br> <a href=\"/Status\">Status</></a> gibt Messwert und Akkuspannung zur&uumlck <br> <a href=\"/OTA\">OTA</></a> aktiviert OTA");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define PIN        15 // 
#define NUMPIXELS 1 // Popular NeoPixel ring size
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32,

TwoWire I2C_1 = TwoWire(0); 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_1, OLED_RESET);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

#define LOGO2_HEIGHT   32
#define LOGO2_WIDTH    128

// 'theSchaetzler_scale_bot', 128x32px
const unsigned char epd_bitmap_theSchaetzler_scale_bot [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x66, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x66, 0x06, 0x00, 0x3f, 0xc0, 0x00, 0x60, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xe0, 0x66, 0x06, 0x00, 0xe6, 0x70, 0x00, 0xf0, 
	0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xe6, 0x7f, 0xe7, 0xff, 0xff, 0xc6, 0x3f, 0xff, 0xff, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xc6, 0x3f, 0xe7, 0xff, 0xff, 0x86, 0x1f, 0xff, 0xff, 
	0x00, 0x18, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x06, 0x00, 0x66, 0x06, 0x00, 0x06, 0x00, 0x00, 0xf0, 
	0x04, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x06, 0x00, 0x66, 0x06, 0x00, 0x06, 0x00, 0x00, 0x60, 
	0x0c, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x06, 0x00, 0x66, 0x06, 0x00, 0x06, 0x00, 0x03, 0xfc, 
	0x0c, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x06, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x03, 0xfc, 
	0x0c, 0x1b, 0xc0, 0x7c, 0x18, 0x00, 0x0f, 0x06, 0xf0, 0x3f, 0x06, 0x3f, 0xe6, 0x0f, 0x03, 0x44, 
	0x1f, 0x1f, 0xe0, 0xee, 0x1e, 0x00, 0x1d, 0xc7, 0xf8, 0x77, 0x86, 0x3f, 0xc6, 0x3d, 0xc3, 0x04, 
	0x1f, 0x1c, 0x71, 0x83, 0x0f, 0xe0, 0x30, 0xc7, 0x1c, 0xc1, 0x86, 0x01, 0xc6, 0x30, 0xc3, 0x3c, 
	0x0c, 0x18, 0x31, 0x83, 0x03, 0xfc, 0x30, 0x46, 0x0c, 0xc1, 0x86, 0x01, 0x86, 0x60, 0x63, 0x3c, 
	0x0c, 0x18, 0x31, 0x83, 0x00, 0x3e, 0x60, 0x06, 0x0c, 0x07, 0x86, 0x03, 0x06, 0x60, 0x63, 0x3c, 
	0x0c, 0x18, 0x31, 0xff, 0x00, 0x06, 0x60, 0x06, 0x0c, 0x7f, 0x86, 0x06, 0x06, 0x7f, 0xe3, 0x3c, 
	0x0c, 0x18, 0x31, 0x00, 0x00, 0x07, 0x60, 0x06, 0x0c, 0xe1, 0x86, 0x0c, 0x06, 0x60, 0x03, 0x3c, 
	0x0c, 0x18, 0x31, 0x83, 0x00, 0x03, 0x30, 0x66, 0x0c, 0xc1, 0x86, 0x18, 0x06, 0x60, 0x63, 0x3c, 
	0x0c, 0x18, 0x31, 0x83, 0x00, 0x03, 0x30, 0xc6, 0x0c, 0xc1, 0x86, 0x38, 0x06, 0x30, 0xc3, 0xfc, 
	0x0c, 0x18, 0x30, 0xee, 0x00, 0x06, 0x1d, 0xc6, 0x0c, 0xe7, 0x87, 0xbf, 0xe6, 0x3d, 0xc3, 0xfc, 
	0x0c, 0x18, 0x30, 0x7c, 0x00, 0x0e, 0x0f, 0x06, 0x0c, 0x7c, 0xc3, 0xbf, 0xe6, 0x0f, 0x00, 0x60, 
	0x0c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 
	0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf1, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0x01, 0x00, 0x40, 0x10, 0x04, 0x01, 0x00, 0x40, 0x10, 0x04, 0x01, 0x00, 0x40, 0x10, 0x04, 0x01, 
	0x01, 0x00, 0x40, 0x10, 0x04, 0x01, 0x00, 0x40, 0x10, 0x04, 0x01, 0x00, 0x40, 0x10, 0x04, 0x01, 
	0x01, 0x00, 0x40, 0x10, 0x04, 0x01, 0x00, 0x40, 0x10, 0x04, 0x01, 0x00, 0x40, 0x10, 0x04, 0x01, 
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

int i;
int sign;
long value;
float result;
float VoltageBattery;
unsigned long tempmicros;
unsigned long ipmillis;
bool wlan_on = true;
#define ledPin 47
#define clockpin 2
#define datapin 4
#define VCC_DISPLAY 17
#define WLAN true
#define Button 0


void setup() {
  Serial.begin(115200);
  I2C_1.begin(11, 12);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  pinMode(clockpin, INPUT_PULLUP);
  pinMode(datapin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(VCC_DISPLAY,OUTPUT);
  digitalWrite(VCC_DISPLAY, HIGH);
  digitalWrite(ledPin, LOW);
  pinMode(Button, INPUT_PULLUP);
  
  float Voltage = 1.5;
  analogWrite(21,(int)(Voltage*255/3.24));
  //float DisplayVoltage = 2.0;
  //analogWrite(VCC_DISPLAY,(int)(DisplayVoltage*255/3.24));
  delay(500);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  
  display.clearDisplay();
  display.drawBitmap(0,0,epd_bitmap_theSchaetzler_scale_bot,128,32,1);
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(45, 25);
  display.display();
  delay(5000);
  wlan_on = digitalRead(Button);
  
  if(true==wlan_on){
  
  Serial.println("Scan start");

  // WiFi.scanNetworks will return the number of networks found.
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
      Serial.print(n);
      Serial.println(" networks found");
      Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
      for (int i = 0; i < n; ++i) {
          // Print SSID and RSSI for each network found
          Serial.printf("%2d",i + 1);
          Serial.print(" | ");
          Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
          Serial.print(" | ");
          Serial.printf("%4d", WiFi.RSSI(i));
          Serial.print(" | ");
          Serial.printf("%2d", WiFi.channel(i));
          Serial.print(" | ");
          switch (WiFi.encryptionType(i))
          {
          case WIFI_AUTH_OPEN:
              Serial.print("open");
              break;
          case WIFI_AUTH_WEP:
              Serial.print("WEP");
              break;
          case WIFI_AUTH_WPA_PSK:
              Serial.print("WPA");
              break;
          case WIFI_AUTH_WPA2_PSK:
              Serial.print("WPA2");
              break;
          case WIFI_AUTH_WPA_WPA2_PSK:
              Serial.print("WPA+WPA2");
              break;
          case WIFI_AUTH_WPA2_ENTERPRISE:
              Serial.print("WPA2-EAP");
              break;
          case WIFI_AUTH_WPA3_PSK:
              Serial.print("WPA3");
              break;
          case WIFI_AUTH_WPA2_WPA3_PSK:
              Serial.print("WPA2+WPA3");
              break;
          case WIFI_AUTH_WAPI_PSK:
              Serial.print("WAPI");
              break;
          default:
              Serial.print("unknown");
          }
          Serial.println();
          delay(10);
      }
  }
}

#if WLAN
if(true==wlan_on){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setTxPower(WIFI_POWER_5dBm );
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    ESP.restart();
  }

  
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);


  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname("theSchätzler");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.println("Progress:");
      int prozent = progress / (total / 100);
      display.setTextSize(2);
      display.print(prozent);
      display.print(" %");
      display.display();
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();  
  
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
#endif
}

void loop() {    
  float SupplyCalip = analogRead(10)/4096.0*3.3;
  VoltageBattery = analogRead(9)*2.0/4096.0*3.3;
  int not_sampled = 1;
  while(not_sampled){
    tempmicros=micros();
    while ((digitalRead(clockpin)==HIGH)) {}
    //if clock is LOW wait until it turns to HIGH
    tempmicros=micros();
    while ((digitalRead(clockpin)==LOW)) {} //wait for the end of the HIGH pulse
    if ((micros()-tempmicros)>500) { //if the HIGH pulse was longer than 500 micros we are at the start of a new bit sequence
      decode(); //decode the bit sequence
      not_sampled = 0;
    }
  }
  if(false == digitalRead(0)){
    if((millis()-ipmillis)>4000){
      showip();
    }
  }
  else{
    ipmillis=millis();
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("CLP:");
    display.print(SupplyCalip);
    display.print(" BAT:");
    display.println(VoltageBattery);
    display.setTextSize(2); // Draw 2X-scale text
    display.print(result);
    display.println("mm");
    display.display();
  }
  
  #if WLAN
  if(true==wlan_on){
  server.on("/Status", []() {  
    float RSSI = WiFi.RSSI();
    server.send(200, "text/html", "Messwert:"+ String(result)+ "<br>Akku: "+ String(VoltageBattery)+"<br>RSSI: "+ String(RSSI));
  });
  server.on("/OTA", []() {
    server.send(200, "text/html", "OTA Activ");
    OTA = true;
  });
  delay(200);
  //DelayLightSleep(2000);
  
  server.handleClient();
  if(OTA==true){
    ArduinoOTA.handle();
  }
  }
  #endif
}

void decode() {

  sign=1;
  value=0;
  for (i=0;i<23;i++) {
    //esp_sleep_enable_ext1_wakeup(GPIO_NUM_12,HIGH) 
    //immer nur Aufwachen wenn GPIO_NUM_12 High ist while 
    //(digitalRead(clockpin)==LOW) {esp_light_sleep_start() 
    //print "ESP GEHT SCHLAFEN"} while (digitalRead(clockpin)==HIGH) { 
    //Automatisch wach und hier Werte messen (durch "esp_sleep_enable_ext1_wakeup" }
    while (digitalRead(clockpin)==HIGH) {} //wait until clock returns to HIGH- the first bit is not needed
    while (digitalRead(clockpin)==LOW) {} //wait until clock returns to LOW
    if (digitalRead(datapin)==LOW) {
      if (i<20) {
        value|= 1<<i;
      }
      if (i==20) {
        sign=-1;
      }
    }
  }
  result=(value*sign)/100.00;    
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}

void showip(void){
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(2); // Draw 2X-scale text
      display.println("IP:");
      display.setTextSize(1); // Draw 2X-scale text
      display.print(WiFi.localIP().toString());
      display.display();
}

void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}