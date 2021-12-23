 /*
   IERG4230 IoT Testing Program
   MFRC522 RFID
   Matter Needing attention:
   On Boot/Reset/wakeup,
   GPIO15(D8) keep LOW and GPIO2(D4)keep HIGH
   GPIO0(D3) HIGH = Run mode / LOW = Flash mode
   On Board LEDs (active LOW):
    GPIO16(D0)
    GPIO2(D4)
  
  Connections:
  MFRC522      ESP8266-12E
  ---------    -----------
  IRQ          Not connected
  SDA_NSS_RX   GPIO 2(D4)
  SCK          Not connected
  MOSI         Not connected
  SCL_MISO_TX  GPIO 0(D3)
  PCTRL        Not connected
  GND          GND
  nRESET       Not connected
  VIN(+5)      VIN
*/

//for web auto update *******************************************************************
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
 
String html_1 = R"=====(
<!DOCTYPE html>
<html>
 <head>
 <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
 <meta charset='utf-8'>
 <meta http-equiv="refresh" content="2">
 <style>
   body {font-size:100%;} 
   #main {display: table; margin: auto;  padding: 0 10px 0 10px; } 
   h2 {text-align:center; } 
   p { text-align:center; }
 </style>
   <title>Auto Update Example Using HTML</title>
 </head>
 
 <body>
   <div id='main'>
     <h2>Auto Update Example Using HTML</h2>
     <div id='count'> 
       <p>Count = %count%</p>
     </div>
   </div> 
 </body>
</html>
)====="; 
//END for web auto update *******************************************************************


#include "MFRC522_I2C.h"
#include <NTPClient.h>;
#include <WiFiUdp.h>;
#include <ESP8266WiFi.h>;
#include <WiFiClient.h>
#include <ESP8266WebServer.h>;
#include <ESP8266mDNS.h>
#include <Wire.h>;
#include "IERG4230_ESP8266.h" //website
#include <Adafruit_GFX.h>     //OLED
#include <Adafruit_SSD1306.h> //OLED

// ==== define program information, it will be shown after reset
#define programMark "IERG4230 Exp2.5 Given program"      //-- You can modify this line. 
#define versionMark 11244                    //-- Any number, You can modify this line. 

#ifndef STASSID
#define STASSID "IERG123"
#define STAPSK  "12345678"
#endif

#define ESP8266I2CAddress 0x66  //-- ESP8266 I2C Address

//OLED colour defination******
/*
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
*/
//****************************

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels




int mailnum = 0;
String str[]={"","","","","","","","","","",""};
String datetime[]={"","","","","","","","","","",""};
boolean duplicate =false; 
#define cardno = "43489250116106128";
int cardaccess = 1;
int accessnum = 0;
String cardaccesstime[]= {"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",};


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};



// OLED
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, i2c Addr, Reset share with 8266 reset);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define RST_PIN 14 // GPIO-14(D5) Pin on ESP8266

// 0x3F is I2C address on SDA.
//MFRC522 mfrc522(0x3F, RST_PIN);   // Create MFRC522 instance.
MFRC522 mfrc522(0x28, RST_PIN);   // Create MFRC522 instance.


//new//
  char *ssid_ap = "IERG123";
  char *password_ap = "12345678";
  
  IPAddress ip(192,168,1,1);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);
  //ESP8266WebServer server(80);
  WiFiServer server(80);
//new for website******************************
// Variable to store the HTTP request
//String header;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

//*******************************************

  
void setup() {

  
   //new//
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_ap, password_ap);
  delay(100);
  server.begin();
  Serial.println("HTTP server started");

  
  ESP.wdtDisable();
  ESP.wdtFeed();


  Serial.begin(115200);           // Initialize serial communications with the PC
  Serial.println("ESP8266-12E/F MFRC5522 RFID test program\n");
  Serial.println("Build-in LED at GPIO-16(D0)");
  Serial.println("I2C SDA Pin at GPIO-2(D4)");
  Serial.println("I2C SCL Pin at GPIO-0(D3)");
//  Wire.begin();                   // Initialize I2C
  Wire.begin(2,0);                  // Initialize I2C
  mfrc522.PCD_Init();             // Init MFRC522
  ShowReaderDetails();            // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, type, and data blocks..."));

  // OLED Setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(1000);
  display.clearDisplay();

  ///OLED diplay 1st line
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  
  // Display static text
  display.println("Smart MailBox");
  display.println("Room:18H");
  display.println("Status:Closed");
  display.display();
  char fd[]={};

//new for server
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid_ap);
  WiFi.begin(ssid_ap, password_ap);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();


// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
   // GMT +8 = 28800;
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(28800);
  
}

void loop() {

  // Look for new cards, and select one if present
  if (! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() )
  {
    ESP.wdtFeed();  // keep FEED the watchdog!
    delay(200);
    return;
  }
  display.clearDisplay();
  // Now a card is selected. The UID and SAK is in mfrc522.uid.
  display.setCursor(0, 0);
  // Display static text
  display.println("Smart MailBox");
  display.println("Room:18H");
  display.display();

/*
  // Dump UID
  Serial.println();
  Serial.print(F("Stamp RFID Length: "));
  Serial.println(mfrc522.uid.size, HEX);
  //OLED 3rd line
  display.setCursor(0, 30);
  display.print("Stamp RFID Length: ");
  display.println(mfrc522.uid.size, HEX);
  display.display();
*/

 


  
  Serial.print(F("Stamp ID:"));
  //OLED 5rd line
  //display.setCursor(0, 30);
  display.print("Stamp ID:");

 // display.display();
 //display.setCursor(55, 50);
  
    
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    // OLED 5rd line
    // display.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    if (mfrc522.uid.uidByte[i] < 0x10)
    {
      display.print(" 0");
    }
    else
    {
      display.print(" ");
    }
    display.print(mfrc522.uid.uidByte[i], HEX);
    display.display();
  }
  Serial.print("\n");//new add
  ESP.wdtFeed();
  
  //new//
  duplicate = false;
  String id = "";
  for (byte i = 0; i < mfrc522.uid.size; i++){
    id += String(mfrc522.uid.uidByte[i]);
    }


  for(int k =0; k<= mailnum;k++){
  if (id == str[k] ){ 
    duplicate = true;
    }
  }

  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
 // Serial.print("Epoch Time: ");
 // Serial.println(epochTime);
  
  String formattedTime = timeClient.getFormattedTime();
 // Serial.print("Formatted Time: ");
 // Serial.println(formattedTime);  

  int currentHour = timeClient.getHours();
  //Serial.print("Hour: ");
  //Serial.println(currentHour);  

  int currentMinute = timeClient.getMinutes();
  //Serial.print("Minutes: ");
  //Serial.println(currentMinute); 

  int currentSecond = timeClient.getSeconds();
  //Serial.print("Seconds: ");
  //Serial.println(currentSecond);  

  String weekDay = weekDays[timeClient.getDay()];
  //Serial.print("Week Day: ");
  //Serial.println(weekDay);    

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  //Serial.print("Month day: ");
  //Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  //Serial.print("Month: ");
  //Serial.println(currentMonth);

  String currentMonthName = months[currentMonth-1];
  //Serial.print("Month name: ");
  //Serial.println(currentMonthName);

  int currentYear = ptm->tm_year+1900;
  //Serial.print("Year: ");
  //Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay)+"  "+ String(weekDay)+"  "+String(currentHour)+":"+String(currentMinute)+":"+String(currentSecond);
  //Serial.print("Current date: ");
  //Serial.println(currentDate);

  
  if(!duplicate){
    if(id != "43489250116106128"){
       str[mailnum] = id;
       datetime[mailnum] = currentDate;
       mailnum++;}
    }
   
   if(id == "43489250116106128"){
        cardaccesstime[accessnum] = currentDate;
        accessnum++;
        cardaccess = cardaccess+1 ;
      }
      
    

  
  
  if(id == "43489250116106128"){
  display.clearDisplay();
  // Now a card is selected. The UID and SAK is in mfrc522.uid.
  display.setCursor(0, 0);
  // Display static text
  display.println("Smart MailBox");
  display.println("Room:18H");
  if(cardaccess%2 == 1){ display.println("Status:Closed");} else {display.println("Status:Opened");}
  display.display();}

     
  for(int j = 0; j<= mailnum;j++){
       if(str[j] != ""){
       Serial.println(str[j]);}
     } 

  Serial.println();
  Serial.print("number of letter(s):");
  Serial.println(mailnum);
  display.print("number of letter(s):");
  display.println(mailnum);
  delay(100);

//inside the loop for webserver*******************************************************************************************
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //meta-refresh page every 2 seconds
            //x=x+1; //page upload counter
            client.println("<HTML>");
            client.print("<HEAD>");
            client.print("<meta http-equiv=\"refresh\" content=\"0.2\">");
            client.print("<TITLE />Smart Mailbox ROOM 18H TEST</title>");
            client.print("</head>");
            client.println("<BODY>");
            //client.print("Zoomkat's meta-refresh test IDE 1.0");
            client.println(" ");
          
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
         
            // Web Page Heading
            client.println("<style> table, th, td { border:1px solid black;}</style>");
            client.println("<body><h1>Smart mailbox</h1>");
            client.println("<h2>Address: Wah Fu (II) Estate Block A Room 18H </h2>");
            client.print("<h1>Total number of mail:");
            client.print(mailnum);
            client.print("</h1>");
            client.println("<h2>CARD no:43489250116106128</h2>");
            client.println("<table style=\"width:100%\">");
            client.println("<tr><td>Received Mail Time</td><td>Mail</td><td>Mail Tag No.</td></tr>");
                         
              for(int j = 0; j<= mailnum;j++){
                if(str[j] != ""){
                  client.print("<tr>");
                  client.print("<td>");
                  client.print(datetime[j]);
                  client.print("</td>");
                  client.print("<td>[");
                  client.print(j+1);
                  client.print("]</td>");
                  client.print("<td>");
                  client.print(str[j]);
                  client.print("</td>");
                  client.print("</tr>");
                }
              }
              client.print("</table>");
              client.print("</body>");


             
            // The HTTP response ends with another blank line
            //client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    //header = "";
    // Close the connection
    //client.stop();
    //Serial.println("Client disconnected.");
    //Serial.println("");
      server.begin();
  }
//**********************************end of webserver
  
}

void ShowReaderDetails() {
  // Get the MFRC522 software version 
  ESP.wdtFeed();
  Serial.println("");
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.print(F("MFRC522 Software Version: 0x"));
  Serial.print(v, HEX);
  if (v == 0x91)
    Serial.print(F(" = v1.0"));
  else if (v == 0x92)
    Serial.print(F(" = v2.0"));
  else
    Serial.print(F(" (unknown)"));
  Serial.println("");
  // When 0x00 or 0xFF is returned, communication probably failed
  if ((v == 0x00) || (v == 0xFF)) {
    Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
  }
}
