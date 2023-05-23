/*
 * ESP8266 NodeMCU Automobile Control over WiFi (AP Mode)
 *
 * http://www.ersanalturk.com
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>

// SSID parameters
const char *ssid = "ESP8266_AP_Mode_Arac";
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
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta charset="utf-8">
<title/>Wifi Araç Kontrol Paneli</title>
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
<center>
<h1>Wifi Araç Kontrolü Esp8266 AP Mode V2</h1>
<br>
    <table width="100%">
      <tr>
        <td width="33%"><button class="button" onclick="toggleCheckbox('vites1');" ontouch="toggleCheckbox('vites1');">Vites 1</button></td>
        <td width="33%"><button class="button" onclick="toggleCheckbox('vites2');" ontouch="toggleCheckbox('vites2');">Vites 2</button></td>
        <td width="33%"><button class="button" onclick="toggleCheckbox('vites3');" ontouch="toggleCheckbox('vites3');">Vites 3</button></td>
      </tr>
    </table>
    <hr>
    
    <table width="100%">
      <tr>
        <td width="33%">&nbsp;</td>
        <td width="33%"><button class="button" onclick="toggleCheckbox('ileri');" ontouch="toggleCheckbox('ileri');">İleri</button></td>
        <td width="33%">&nbsp;</td>
      </tr>
      <tr>
        <td><button class="button" onclick="toggleCheckbox('sol');" ontouch="toggleCheckbox('sol');">Sol</button></td>
        <td><button class="button" onclick="toggleCheckbox('dur');" ontouch="toggleCheckbox('dur');">Dur</button></td>
        <td><button class="button" onclick="toggleCheckbox('sag');" ontouch="toggleCheckbox('sag');">Sağ</button></td>
      </tr>
      <tr>
        <td width="33%">&nbsp;</td>
        <td><button class="button" onclick="toggleCheckbox('geri');" ontouch="toggleCheckbox('geri');">Geri</button></td>
        <td width="33%">&nbsp;</td>
      </tr>
    </table>
    <hr>
    
    <p style="font-size:2em">Bas-Çek Butonlar</p>    
     <table width="100%" border="1">
      <tr>
        <td width="33%">&nbsp;</td>
        <td width="33%"><button class="button" ontouchstart="toggleCheckbox('ileri');" ontouchend="toggleCheckbox('dur');">İleri</button></td>
        <td width="33%">&nbsp;</td>
      </tr>
      <tr>
        <td><button class="button"  ontouchstart="toggleCheckbox('sol');"  ontouchend="toggleCheckbox('dur');">Sol</button></td>
        <td><button class="button"  ontouchstart="toggleCheckbox('kornaAcik');"  ontouchend="toggleCheckbox('kornaKapali');">Korna</button></td>
        <td><button class="button"  ontouchstart="toggleCheckbox('sag');"  ontouchend="toggleCheckbox('dur');">Sağ</button></td>
      </tr>
      <tr>
        <td>&nbsp;</td>
        <td><button class="button"  ontouchstart="toggleCheckbox('geri');"  ontouchend="toggleCheckbox('dur');">Geri</button></td>
        <td>&nbsp;</td>
      </tr>
    </table>
    
    <table>
      <tr>
        <td width="50%"><button class="button"  ontouchstart="toggleCheckbox('solSinyal');"  ontouchend="toggleCheckbox('farKapali');">Sol Sinyal</button></td>
        <td width="50%"><button class="button"  ontouchstart="toggleCheckbox('sagSinyal');"  ontouchend="toggleCheckbox('farKapali');">Sağ Sinyal</button></td>
        </tr>
      <tr>
        <td><button class="button" onclick="toggleCheckbox('farAcik');" ontouch="toggleCheckbox('farAcik');">Farları Aç</button></td>
        <td><button class="button" onclick="toggleCheckbox('farKapali');" ontouch="toggleCheckbox('farKapali');">Farları Kapat</button></td>
      </tr>
    <table>
    <hr>
    
</center>

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
#define sagLedPin D0
#define solLedPin D1 
#define buzzerPin D2
#define PWMA D3
#define PWMB D4 
#define inA1 D5
#define inA2 D6
#define inB1 D7
#define inB2 D8
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 Serial.println("You called root page");
 String s = FPSTR(MAIN_page); //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleVites1() { 
 Serial.println("Vites 1");
 analogWrite(PWMA,700);
 analogWrite(PWMB,700);
 server.send(200, "text/html", "Vites 1"); //Send ADC value only to client ajax request
}

void handleVites2() { 
 Serial.println("Vites 2");
 analogWrite(PWMA,850);
 analogWrite(PWMB,850);
 server.send(200, "text/html", "Vites 2"); //Send ADC value only to client ajax request
}

void handleVites3() { 
 Serial.println("Vites 3");
 analogWrite(PWMA,1023);
 analogWrite(PWMB,1023);
 server.send(200, "text/html", "Vites 3"); //Send ADC value only to client ajax request
}

void handleFarAc() { 
 Serial.println("Far Acik");
 digitalWrite(sagLedPin,HIGH);
 digitalWrite(solLedPin,HIGH);
 server.send(200, "text/html", "Far Açık"); //Send ADC value only to client ajax request
}

void handleFarKapat() { 
 Serial.println("Far Kapali");
 digitalWrite(sagLedPin,LOW);
 digitalWrite(solLedPin,LOW);
 server.send(200, "text/html", "Far Kapalı"); //Send ADC value only to client ajax request
}

void handleKornaAc() { 
 Serial.println("Korna Acik");
 digitalWrite(buzzerPin,HIGH);
 server.send(200, "text/html", "Korna Açık"); //Send ADC value only to client ajax request
}

void handleKornaKapat() { 
 Serial.println("Korna Kapali");
 digitalWrite(buzzerPin,LOW);
 server.send(200, "text/html", "Korna Kapalı"); //Send ADC value only to client ajax request
}

void handleIleri() { 
 Serial.println("Ileri");
 digitalWrite(inA1,HIGH);
 digitalWrite(inA2,LOW);
 digitalWrite(inB1,LOW);
 digitalWrite(inB2,HIGH);
 server.send(200, "text/html", "İleri"); //Send ADC value only to client ajax request
}

void handleGeri() { 
 Serial.println("Geri");
 digitalWrite(inA1,LOW);
 digitalWrite(inA2,HIGH);
 digitalWrite(inB1,HIGH);
 digitalWrite(inB2,LOW);
 server.send(200, "text/html", "Geri"); //Send ADC value only to client ajax request
}

void handleSol() { 
 Serial.println("Sol");
 digitalWrite(inA1,HIGH);
 digitalWrite(inA2,LOW);
 digitalWrite(inB1,HIGH);
 digitalWrite(inB2,LOW);
 server.send(200, "text/html", "Sol"); //Send ADC value only to client ajax request
}

void handleSag() { 
 Serial.println("Sag");
 digitalWrite(inA1,LOW);
 digitalWrite(inA2,HIGH);
 digitalWrite(inB1,LOW);
 digitalWrite(inB2,HIGH);
 server.send(200, "text/html", "Sağ"); //Send ADC value only to client ajax request
}

void handleDur() { 
 Serial.println("Dur");
 digitalWrite(inA1,LOW);
 digitalWrite(inA2,LOW);
 digitalWrite(inB1,LOW);
 digitalWrite(inB2,LOW);
 server.send(200, "text/html", "Dur"); //Send ADC value only to client ajax request
}

void handleSolSinyal() { 
 Serial.println("Sol sinyal");
 digitalWrite(solLedPin,HIGH);
 server.send(200, "text/html", "Sol sinyal"); //Send ADC value only to client ajax request
}

void handleSagSinyal() { 
 Serial.println("Sag sinyal");
 digitalWrite(sagLedPin,HIGH);
 server.send(200, "text/html", "Sag sinyal"); //Send ADC value only to client ajax request
}

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
    //Tanımlı pinler output
  pinMode(solLedPin,OUTPUT); 
  pinMode(sagLedPin,OUTPUT); 
  pinMode(buzzerPin,OUTPUT); 
  pinMode(PWMA,OUTPUT); 
  pinMode(PWMB,OUTPUT); 
  pinMode(inA1,OUTPUT); 
  pinMode(inA2,OUTPUT); 
  pinMode(inB1,OUTPUT); 
  pinMode(inB2,OUTPUT);
  //Motorları Durdur
  digitalWrite(solLedPin,LOW);
  digitalWrite(sagLedPin,LOW);
  digitalWrite(buzzerPin,LOW);
  digitalWrite(inA1,LOW);
  digitalWrite(inA2,LOW);
  digitalWrite(inB1,LOW);
  digitalWrite(inB2,LOW);
  analogWrite(PWMA,700);
  analogWrite(PWMB,700);
  
  Serial.begin(115200);
  
  // Setup AP
  WiFi.mode(WIFI_AP); //Only Access point
  WiFi.softAPConfig(ip, ip, netmask);
  WiFi.softAP(ssid, password);

  // Get and print IP Address
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);


 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/vites1", handleVites1);
  server.on("/vites2", handleVites2);
  server.on("/vites3", handleVites3);
  server.on("/ileri", handleIleri);
  server.on("/geri", handleGeri);
  server.on("/sol", handleSol);
  server.on("/sag", handleSag);
  server.on("/dur", handleDur);
  server.on("/solSinyal", handleSolSinyal);
  server.on("/sagSinyal", handleSagSinyal);
  server.on("/farAcik", handleFarAc); 
  server.on("/farKapali", handleFarKapat);
  server.on("/kornaAcik", handleKornaAc);
  server.on("/kornaKapali", handleKornaKapat);  
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
