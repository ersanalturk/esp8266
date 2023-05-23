/*
 * ESP8266 NodeMCU LED - Buzzer Control over WiFi (AP Mode)
 *
 * http://ersanalturk.com/yazilarim.php?yazi=IoTornekleri.php
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>

// SSID parameters
const char *ssid = "ESP8266_AP_Mode";
const char *password = "12345678";

IPAddress ip(192, 168, 4, 1); // IP Address
IPAddress netmask(255, 255, 255, 0); // Netmask
//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80

//---------------------------------------------------------------
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8"/>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta charset="utf-8">
<title/>Wifi Led Buzzer Kontrolu V2</title>
</head>
<body>
<center>
<h1>Wifi Led Buzzer Kontrolu V2</h1><br>
<table width="100%" border="1">
  <tr>
    <td height="46" align="center" bgcolor="lightgreen"><a href="ledAc"  target="myIframe">Led AÃ§</a></td>
    <td height="46" align="center" bgcolor="red"><a href="ledKapat" target="myIframe">Led Kapat</a></td>
  </tr>
  <tr>
    <td height="46" align="center" bgcolor="lightblue"><a href="buzzerAc"  target="myIframe">Buzzer AÃ§</a></td>
    <td height="46" align="center" bgcolor="red"><a href="buzzerKapat"  target="myIframe">Buzzer Kapat</a></td>
  </tr>
</table>
<hr>
<iframe name="myIframe" width="100" height="55" frameBorder="0"><br>
</center>

</body>
</html>
)=====";
//---------------------------------------------------------------
//On board LED Connected to GPIO2
#define ledPin D2 
#define buzzerPin D3

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 Serial.println("You called root page");
 String s = FPSTR(MAIN_page); //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleLedAc() { 
 Serial.println("Led Acik");
 digitalWrite(ledPin,HIGH);
 server.send(200, "text/html", "Led AÃ§Ä±k"); //Send ADC value only to client ajax request
}

void handleLedKapat() { 
 Serial.println("Led Kapali");
 digitalWrite(ledPin,LOW);
 server.send(200, "text/html", "Led KapalÄ±"); //Send ADC value only to client ajax request
}

void handleBuzzerAc() { 
 Serial.println("Led Acik");
 digitalWrite(buzzerPin,HIGH);
 server.send(200, "text/html", "Buzzer AÃ§Ä±k"); //Send ADC value only to client ajax request
}

void handleBuzzerKapat() { 
 Serial.println("Led Kapali");
 digitalWrite(buzzerPin,LOW);
 server.send(200, "text/html", "Buzzer KapalÄ±"); //Send ADC value only to client ajax request
}

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);
  
  // Setup AP
  WiFi.mode(WIFI_AP); //Only Access point
  WiFi.softAPConfig(ip, ip, netmask);
  WiFi.softAP(ssid, password);

  // Get and print IP Address
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  //Onboard LED port Direction output
  pinMode(ledPin,OUTPUT); 
  pinMode(buzzerPin,OUTPUT); 
  
  //MotorlarÄ± Durdur
  digitalWrite(ledPin,LOW);
  digitalWrite(buzzerPin,LOW);
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/ledAc", handleLedAc); //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/ledKapat", handleLedKapat);
  server.on("/buzzerAc", handleBuzzerAc);
  server.on("/buzzerKapat", handleBuzzerKapat);
  
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
