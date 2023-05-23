/*
 * ESP8266 NodeMCU Araç Kontrol Omni tekerlek
 * Tekerlek yönleri:
 * Kuzeye ve doğuya doğru ileri
 * Güneye ve Batıya doğru geri
 * Kuzey motor M1
 * Batı Motor M2
 * Doğu Motor M3
 * Güney Motor M4
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>

// SSID parameters
const char *ssid = "ESP8266_Arac_AP_Mode";
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
    <title>WiFi Araba Kontrol Sistemi</title>
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
  <h1>WiFi Araba Kontrol Sistemi</h1>

  <table border="1">
      <tr>
        <th colspan="3">Mobil Bas-Bırak Tuşlar</th>
      </tr>
      <tr>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('kuzeyBati');" ontouchend="toggleCheckbox('dur');">KuzeyBatı</button>
        </td>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('kuzey');" ontouchend="toggleCheckbox('dur');">Kuzey</button>
        </td>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('kuzeyDogu');" ontouchend="toggleCheckbox('dur');">KuzeyDoğu</button>
        </td>
      </tr>
      <tr>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('bati');" ontouchend="toggleCheckbox('dur');">Batı</button>
        </td>
        <td>
          <button class="button" ontouch="toggleCheckbox('dur');">Dur</button>
        </td>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('dogu');" ontouchend="toggleCheckbox('dur');">Doğu</button>
        </td>
      </tr>
      <tr>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('guneyBati');" ontouchend="toggleCheckbox('dur');">GüneyBatı</button>
        </td>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('guney');" ontouchend="toggleCheckbox('dur');">Güney</button>
        </td>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('guneyDogu');" ontouchend="toggleCheckbox('dur');">GüneyDoğu</button>
        </td>
      </tr>
      <tr>
        <td>
          <button class="button" ontouch="toggleCheckbox('vites1');">Vites 1</button>
        </td>
        <td>
          <button class="button" ontouch="toggleCheckbox('vites2');">Vites 2</button>
        </td>
        <td>
          <button class="button" ontouch="toggleCheckbox('vites3');">Vites 3</button>
        </td>
      </tr> 
  </table>

  <table border="1">
      <tr>
        <th colspan="3">Bilgisayar Kontrol Tuşları</th>
      </tr>
      <tr>
        <td>
          <button class="button" onclick="toggleCheckbox('bkuzeyBati');" accesskey="t">KuzeyBatı - t</button>
        </td>
        <td>
          <button class="button" onclick="toggleCheckbox('bkuzey');" accesskey="y">Kuzey - y</button>
        </td>
        <td>
          <button class="button" onclick="toggleCheckbox('bkuzeyDogu');" accesskey="u">KuzeyDoğu - u</button>
        </td>
      </tr>
      <tr>
        <td>
          <button class="button" onclick="toggleCheckbox('bbati');" accesskey="g">Batı - g</button>
        </td>
        <td>
          <button class="button" onclick="toggleCheckbox('dur');" accesskey="h">Dur - h</button>
        </td>
        <td>
          <button class="button" onclick="toggleCheckbox('bdogu');" accesskey="j">Doğu - j</button>
        </td>
      </tr>
      <tr>
        <td>
          <button class="button" onclick="toggleCheckbox('bguneyBati');" accesskey="b">GüneyBatı - b</button>
        </td>
        <td>
          <button class="button" onclick="toggleCheckbox('bguney');" accesskey="n">Güney - n</button>
        </td>
        <td>
          <button class="button" onclick="toggleCheckbox('bguneyDogu');" accesskey="m">GüneyDoğu - m</button>
        </td>
      </tr>
      <tr>
        <td>
          <button class="button" onclick="toggleCheckbox('vites1');" accesskey="1">Vites 1 - 1</button>
        </td>
        <td>
          <button class="button" onclick="toggleCheckbox('vites2');" accesskey="2">Vites 2 -2</button>
        </td>
        <td>
          <button class="button" onclick="toggleCheckbox('vites3');" accesskey="3">Vites 3 - 3</button>
        </td>
      </tr> 
  </table>

  <table border="1">
      <tr>
        <th colspan="2">Mobil Bas-Bırak Tuşlar (Motor Test)</th>
      </tr>
      <tr>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('m1ileri');" ontouchend="toggleCheckbox('dur');">M1 ileri</button>
        </td>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('m1geri');" ontouchend="toggleCheckbox('dur');">M1 geri</button>
        </td>
      </tr>
      <tr>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('m2ileri');" ontouchend="toggleCheckbox('dur');">M2 ileri</button>
        </td>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('m2geri');" ontouchend="toggleCheckbox('dur');">M2 geri</button>
        </td>
      </tr>
      <tr>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('m3ileri');" ontouchend="toggleCheckbox('dur');">M3 ileri</button>
        </td>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('m3geri');" ontouchend="toggleCheckbox('dur');">M3 geri</button>
        </td>
      </tr>
      <tr>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('m4ileri');" ontouchend="toggleCheckbox('dur');">M4 ileri</button>
        </td>
        <td>
          <button class="button" ontouchstart="toggleCheckbox('m4geri');" ontouchend="toggleCheckbox('dur');">M4 geri</button>
        </td>
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
#define pwm D0  //vites hiz ayari
#define m11 D1  //kuzey
#define m12 D2
#define m21 D3  //batı
#define m22 D4 
#define m31 D5  //doğu
#define m32 D6 
#define m41 D7  //güney
#define m42 D8


//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 Serial.println("You called root page");
 String s = FPSTR(MAIN_page); //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void m1ileri(){ //sol ön motor
  digitalWrite(m11,1);
  digitalWrite(m12,0);
}

void m1geri(){
  digitalWrite(m11,0);
  digitalWrite(m12,1);
}

void handleM1ileri() { 
 Serial.println("M1 ileri");
 m1ileri();
 server.send(200, "text/html", "M1 ileri"); //Send A/D value only to client ajax request
}

void handleM1geri() { 
 Serial.println("M1 geri");
 m1geri();
 server.send(200, "text/html", "M1 geri"); 
}

void m2ileri(){ 
  digitalWrite(m21,1);
  digitalWrite(m22,0);
}

void m2geri(){
  digitalWrite(m21,0);
  digitalWrite(m22,1);
}

void handleM2ileri() { 
 Serial.println("M2 ileri");
 m2ileri();
 server.send(200, "text/html", "M2 ileri"); 
}

void handleM2geri() { 
 Serial.println("M2 geri");
 m2geri();
 server.send(200, "text/html", "M2 geri"); 
}

void m3ileri(){
  digitalWrite(m31,1);
  digitalWrite(m32,0);
}

void m3geri(){
  digitalWrite(m31,0);
  digitalWrite(m32,1);
}

void handleM3ileri() { 
 Serial.println("M3 ileri");
 m3ileri();
 server.send(200, "text/html", "M3 ileri"); 
}

void handleM3geri() { 
 Serial.println("M3 geri");
 m3geri();
 server.send(200, "text/html", "M3 geri"); 
}

void m4ileri(){
  digitalWrite(m41,1);
  digitalWrite(m42,0);
}

void m4geri(){
  digitalWrite(m41,0);
  digitalWrite(m42,1);
}

void handleM4ileri() { 
 Serial.println("M4 ileri");
 m4ileri();
 server.send(200, "text/html", "M4 ileri"); 
}

void handleM4geri() { 
 Serial.println("M4 geri");
 m4geri();
 server.send(200, "text/html", "M4 geri"); 
}

void handleDur() { 
 Serial.println("Motorlar durdu");
  digitalWrite(m11,LOW);
  digitalWrite(m12,LOW);
  digitalWrite(m21,LOW);
  digitalWrite(m22,LOW);
  digitalWrite(m31,LOW);
  digitalWrite(m32,LOW);
  digitalWrite(m41,LOW);
  digitalWrite(m42,LOW);
 server.send(200, "text/html", "Motorlar durdu"); 
}

void dur(){
  digitalWrite(m11,LOW);
  digitalWrite(m12,LOW);
  digitalWrite(m21,LOW);
  digitalWrite(m22,LOW);
  digitalWrite(m31,LOW);
  digitalWrite(m32,LOW);
  digitalWrite(m41,LOW);
  digitalWrite(m42,LOW);
  }

void handleVites1() { 
 Serial.println("Vites1");
 analogWrite(pwm,700);
 server.send(200, "text/html", "Vites1"); 
}

void handleVites2() { 
 Serial.println("Vites2");
 analogWrite(pwm,850);
 server.send(200, "text/html", "Vites2"); 
}

void handleVites3() { 
 Serial.println("Vites3");
 analogWrite(pwm,1023);
 server.send(200, "text/html", "Vites3"); 
}

//mobil kontrol için
void handlek() { 
 Serial.println("Kuzey");
 m2ileri();
 m3ileri();
 server.send(200, "text/html", "Kuzey"); 
}

void handleg() { 
 Serial.println("Guney");
 m2geri();
 m3geri();
 server.send(200, "text/html", "Gumey"); 
}

void handleb() { 
 Serial.println("Bati");
 m1geri();
 m4geri();
 server.send(200, "text/html", "Bati"); 
}

void handled() { 
 Serial.println("Dogu");
 m1ileri();
 m4ileri();
 server.send(200, "text/html", "Dogu"); 
}

void handlegb() { 
 Serial.println("Guney bati");
   m1geri();
   m2geri();
   m3geri();
   m4geri();
 server.send(200, "text/html", "Guney bati"); 
}


void handlekd() { 
 Serial.println("Kuzey dogu");
   m1ileri();
   m2ileri();
   m3ileri();
   m4ileri();
 server.send(200, "text/html", "Kuzey dogu"); //Send ADC value only to client ajax request
}

void handlekb() { 
 Serial.println("Kuzey bati");
   m1geri();
   m2ileri();
   m3ileri();
   m4geri();
 server.send(200, "text/html", "Kuzey bati"); 
}


void handlegd() { 
 Serial.println("Guney dogu");
   m1ileri();
   m2geri();
   m3geri();
   m4ileri();
 server.send(200, "text/html", "Guney dogu"); //Send ADC value only to client ajax request
}

//bilgisayardan kontrol için
void handlebk() { 
 Serial.println("Kuzey");
 dur();
 m2ileri();
 m3ileri();
 server.send(200, "text/html", "Kuzey"); 
}

void handlebg() { 
 Serial.println("Guney");
  dur();
 m2geri();
 m3geri();
 server.send(200, "text/html", "Gumey"); 
}

void handlebb() { 
 Serial.println("Bati");
  dur();
 m1geri();
 m4geri();
 server.send(200, "text/html", "Bati"); 
}

void handlebd() { 
 Serial.println("Dogu");
  dur();
 m1ileri();
 m4ileri();
 server.send(200, "text/html", "Dogu"); 
}

void handlebgb() { 
 Serial.println("Guney bati");
  dur();
   m1geri();
   m2geri();
   m3geri();
   m4geri();
 server.send(200, "text/html", "Guney bati"); 
}


void handlebkd() { 
 Serial.println("Kuzey dogu");
  dur();
   m1ileri();
   m2ileri();
   m3ileri();
   m4ileri();
 server.send(200, "text/html", "Kuzey dogu"); //Send ADC value only to client ajax request
}

void handlebkb() { 
 Serial.println("Kuzey bati");
  dur();
   m1geri();
   m2ileri();
   m3ileri();
   m4geri();
 server.send(200, "text/html", "Kuzey bati"); 
}


void handlebgd() { 
 Serial.println("Guney dogu");
  dur();
   m1ileri();
   m2geri();
   m3geri();
   m4ileri();
 server.send(200, "text/html", "Guney dogu"); //Send ADC value only to client ajax request
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
  pinMode(pwm,OUTPUT); 
  pinMode(m11,OUTPUT); 
  pinMode(m12,OUTPUT);
  pinMode(m21,OUTPUT); 
  pinMode(m22,OUTPUT); 
  pinMode(m31,OUTPUT); 
  pinMode(m32,OUTPUT); 
  pinMode(m41,OUTPUT); 
  pinMode(m42,OUTPUT); 
  
  //Motorları Durdur
  digitalWrite(m11,LOW);
  digitalWrite(m12,LOW);
  digitalWrite(m21,LOW);
  digitalWrite(m22,LOW);
  digitalWrite(m31,LOW);
  digitalWrite(m32,LOW);
  digitalWrite(m41,LOW);
  digitalWrite(m42,LOW);
  analogWrite(pwm,700);
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/dur", handleDur); 
  server.on("/m1ileri", handleM1ileri);
  server.on("/m2ileri", handleM2ileri);
  server.on("/m3ileri", handleM3ileri);
  server.on("/m4ileri", handleM4ileri);
  server.on("/m1geri", handleM1geri);
  server.on("/m2geri", handleM2geri);
  server.on("/m3geri", handleM3geri);
  server.on("/m4geri", handleM4geri);
  
  //bas-bırak tuşlar için
  server.on("/kuzey", handlek);
  server.on("/guney", handleg);
  server.on("/dogu", handled);
  server.on("/bati", handleb);
  server.on("/kuzeyBati", handlekb);
  server.on("/guneyBati", handlegb);
  server.on("/kuzeyDogu", handlekd);
  server.on("/guneyDogu", handlegd);

  //bilgisayar kontrolü için
  server.on("/bkuzey", handlebk);
  server.on("/bguney", handlebg);
  server.on("/bdogu", handlebd);
  server.on("/bbati", handlebb);
  server.on("/bkuzeyBati", handlebkb);
  server.on("/bguneyBati", handlebgb);
  server.on("/bkuzeyDogu", handlebkd);
  server.on("/bguneyDogu", handlebgd);
  
  server.on("/vites1", handleVites1);
  server.on("/vites2", handleVites2);
  server.on("/vites3", handleVites3);
  
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
