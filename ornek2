/*
 * ESP8266 NodeMCU LED - Buzzer Control over WiFi (AP Mode)
 *
 * www.ersanalturk.com
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
<!DOCTYPE HTML><html lang="tr">
  <head>
    <title>Akıllı Ev V2</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;background-color:#eee}
      .button {
    margin-top:30px;
    margin-bottom:30px;
        padding: 10px 20px;
        font-size: 24px;
        text-align: center;
        outline: none;
        color: #fff;
        background-color: #2f4468;
        border: none;
        border-radius: 5px;
        box-shadow: 0 6px #999;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }  
      .button:hover {background-color: #1f2e45}
      .button:active {
        background-color: #1f2e45;
        box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
    </style>
  </head>
  <body>
  <h1>Akıllı Ev Kontrol Sistemi v2</h1>
  
 <table border="1">
   <tr>
    <th colspan="2">
      Mobil Ekran Kontrol
    </th>
   </tr>
   <tr>
      <td><button class="button" ontouch="toggleCheckbox('ledAc');" onclick="toggleCheckbox('ledAc');">Led Aç</button></td>
      <td><button class="button" ontouch="toggleCheckbox('ledKapat');" onclick="toggleCheckbox('ledKapat');">Led Kapat</button></td>
  </tr>
   <tr>
      <td colspan="2"><button class="button" ontouchstart="toggleCheckbox('buzzerAc');" ontouchend="toggleCheckbox('buzzerKapat');">Zile Bas</button></td>
   </tr>
 </table> 
 
 <table border="1">
   <tr>
    <th colspan="2">
      Bilgisayar Ekranından Kontrol
    </th>
   </tr>
   <tr>
      <td><button class="button" onclick="toggleCheckbox('ledAc');" accesskey="g">Led Aç - g</button></td>
      <td><button class="button" onclick="toggleCheckbox('ledKapat');" accesskey="h">Led Kapat - h</button></td>
  </tr>
   <tr>
      <td colspan="2"><button class="button" onmousedown="toggleCheckbox('buzzerAc');" onmouseup="toggleCheckbox('buzzerKapat');">Zile Bas</button></td>
   </tr>
 </table> 

 <script>
   function toggleCheckbox(x) {
   var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }
  </script>
  </body>
</html>
)=====";
//---------------------------------------------------------------
//On board LED Connected to GPIO2
#define ledPin D0 
#define buzzerPin D1

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
 server.send(200, "text/html", "Led Acik"); //Send ADC value only to client ajax request
}

void handleLedKapat() { 
 Serial.println("Led Kapali");
 digitalWrite(ledPin,LOW);
 server.send(200, "text/html", "Led Kapali"); 
}

void handleBuzzerAc() { 
 Serial.println("Led Acik");
 digitalWrite(buzzerPin,HIGH);
 server.send(200, "text/html", "Buzzer Acik"); 
}

void handleBuzzerKapat() { 
 Serial.println("Led Kapali");
 digitalWrite(buzzerPin,LOW);
 server.send(200, "text/html", "Buzzer Kapali"); 
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
  
  //Motorları Durdur
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
