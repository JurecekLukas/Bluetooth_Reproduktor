#include "WiFi.h"
#include "SPIFFS.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Arduino.h"
#include "Audio.h"
#include <LiquidCrystal_I2C.h>
 
 
// Zapojení
// Displej 
//22 SCL
//21 SDA
#define I2S_DOUT      23  // Pin 23 DIN connection
#define I2S_BCLK      26  // Pin 26 Bit clock
#define I2S_LRC       25  // Pin 25 Left Right Clock
int CLK = 14;            // Pin 9 CLK 
int DT = 12;             // Pin 8 DT


//Proměnné pro změnu hlasitosti
int pozice = 0;          
int rotace=0;  
int pom=0;
int hlasitost = 0;
boolean ZlevaDoprava;


//Proměnné pro změnu stanice
int i=0;
const int TlacitkoPin = 34;
int TlacitkoStav = 0; 
int TlacitkoStav2 = 0;


//Proměnné pro posuvník
const int output = 2;
String sliderValue = "0";
int posuvnik = 0;
int pomHlasitost = 0;
int pomPosuvnik = 0;
int pom3 = 0;
// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
const char* PARAM_INPUT = "value";


 
Audio audio;
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
String ssid =     "Test";   //SSID (název Wifi)
String password = "TestTest";    //Heslo


AsyncWebServer server(80);


// Set LED GPIO - tohle by mela byt ledka na esp32
const int ledPin = 34;
const int ledPin2=35;
// Stores LED state
String ledState;
String ledState2;


String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    
    Serial.print(ledState);
    return ledState;
  }

  return String();
}


String processor2(const String& var2){
  //Serial.println(var);
  if (var2 == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}


//Funkce na změnu hlasitosti
void zvuk(){
   pom = digitalRead(CLK);
     if (pom != rotace){ //pomocí pinu DT zjišťujeme otočení
       if (digitalRead(DT) != pom) {  
          //otočení doprava
         pozice ++;
         ZlevaDoprava = true;
       } else { 
        //otočení doleva
         ZlevaDoprava = false;
         pozice--;
       }
       
       if (ZlevaDoprava){ // otáčení doprava
        if (hlasitost >20){ //pokud hlasitost bude vyšší než 20 nastavíme na 21 (MAX)
          hlasitost =21;

         }
         else{ //pokud nebude ještě na hodnotě 20 přičítáme o 1
          hlasitost++;

         }     
         
 
       }else{        // otáčení doleva 
        if (hlasitost <1){ //pokud hlasitost bude nižší než 1 nastavíme na 0 (MIN)
          hlasitost =0;
         }
         else{//pokud nebude ještě na hodnotě 0 odečítáme o 1
          hlasitost--;

         } 
       }
       //Výpis pozice encodéru

       
     } 
   rotace = pom;
}


void zvuk2(){
  pomHlasitost = hlasitost;
  pomPosuvnik = posuvnik;
  if(pomPosuvnik != 0){
    pom3=posuvnik;
    Serial.println(pom3);
    pomHlasitost = 0;
  }
  else if(pomHlasitost != 0){
    pom3=hlasitost;
    Serial.println(pom3);
    pomPosuvnik = 0; 
  }
}

//Funkce na změnu stanice/zapnutí rádia + výpis na diplej
void zmenaStanice(){
  TlacitkoStav = digitalRead(TlacitkoPin);
  Serial.println(TlacitkoStav);
  if (TlacitkoStav == HIGH) { //Pokud zmáčknu tlačítko tak se zvedne hodnota i a díky tomu se přepne switch
    i++;
   }


 
  switch(i){
    case 0:   //Stav 0 nedělá nic čeká na signál
      break;
    case 1:
      audio.connecttohost("http://21283.live.streamtheworld.com/EVROPA2.mp3"); //Připoj se na adresu 
      Serial.println("Evropa2");  //Výpis v sériovém monitoru
      lcd.clear();                //Vymaž co je na displeji
      lcd.print("Stanice: ");     //Napiš na displej (začne v místě 0,0)
      lcd.setCursor(0,1);         //Běž na místo 0,1 (začátek 2.řádku)
      lcd.print("Evropa2");       //Napiš na displej
      delay(500);
      break;
    case 2:
      audio.connecttohost("https://ice.abradio.cz/orion128.mp3");
      Serial.println("Orion");
      lcd.clear();
      lcd.print("Stanice: ");
      lcd.setCursor(0,1);
      lcd.print("Hitradio Orion");
      delay(500);
      break;
    case 3:
      audio.connecttohost("http://icecast4.play.cz/kiss128.mp3");
      Serial.println("Rádio Kiss");
      lcd.clear();
      lcd.print("Stanice: ");
      lcd.setCursor(0,1);
      lcd.print("Kiss");
      delay(500);
      break;
    case 4:
      audio.connecttohost("http://icecast6.play.cz/casrock128.mp3");
      Serial.println("Čas Rock");
      lcd.clear();
      lcd.print("Stanice: ");
      lcd.setCursor(0,1);
      lcd.print("Čas Rock");
      delay(500);
      break;
    case 5:
      audio.connecttohost("http://vis.media-ice.musicradio.com/CapitalMP3");
      Serial.println("CapitalMP3");
      lcd.clear();
      lcd.print("Stanice: ");
      lcd.setCursor(0,1);
      lcd.print("CapitalMP3");
      delay(500);
      break;
    case 6:
      audio.connecttohost("http://stream.rockantenne.de/heavy-metal");
      Serial.println("RockAntenne Heavy metal");
      lcd.clear();
      lcd.print("Stanice: ");
      lcd.setCursor(0,1);
      lcd.print("RockAntenne HM");
      delay(500);
      break;
     case 7:
      audio.connecttohost("http://67.212.238.178:8004/stream");
      Serial.println("Old time USA Radio");
      lcd.clear();
      lcd.print("Stanice: ");
      lcd.setCursor(0,1);
      lcd.print("OldTimeUSA Radio");
      delay(500);
      break;
     case 8:
      audio.connecttohost("http://playerservices.streamtheworld.com/api/livestream-redirect/LOS40_ARGENTINA.mp3");
      Serial.println("Los40 Argentina");
      lcd.clear();
      lcd.print("Stanice: ");
      lcd.setCursor(0,1);
      lcd.print("Los40 Argentina");
      delay(500);
      break;
    case 9:
      i=0;
      break;

  }
}

void zmenaStanice2(){
   //Evropa2 tlačítko
  server.on("/1", HTTP_GET, [](AsyncWebServerRequest *request){
      i=0;
      audio.connecttohost("http://21283.live.streamtheworld.com/EVROPA2.mp3"); //Připoj se na adresu 
      Serial.println("Evropa2");  //Výpis v sériovém monitoru
      lcd.clear();                //Vymaž co je na displeji
      lcd.print("Stanice: ");     //Napiš na displej (začne v místě 0,0)
      lcd.setCursor(0,1);         //Běž na místo 0,1 (začátek 2.řádku)
      lcd.print("Evropa2");       //Napiš na displej
      delay(500);
      request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  

  server.on("/2", HTTP_GET, [](AsyncWebServerRequest *request){
     i=0;
     audio.connecttohost("https://ice.abradio.cz/orion128.mp3");
     Serial.println("Orion");
     lcd.clear();
     lcd.print("Stanice: ");
     lcd.setCursor(0,1);
     lcd.print("Hitradio Orion");
     delay(500);
     request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
    server.on("/3", HTTP_GET, [](AsyncWebServerRequest *request){
     i=0;
     audio.connecttohost("http://icecast4.play.cz/kiss128.mp3");
     Serial.println("Rádio Kiss");
     lcd.clear();
     lcd.print("Stanice: ");
     lcd.setCursor(0,1);
     lcd.print("Kiss");
     delay(500);
     request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
    server.on("/4", HTTP_GET, [](AsyncWebServerRequest *request){
     i=0;
     audio.connecttohost("http://icecast6.play.cz/casrock128.mp3");
     Serial.println("Čas Rock");
     lcd.clear();
     lcd.print("Stanice: ");
     lcd.setCursor(0,1);
     lcd.print("Čas Rock");
     delay(500);
     request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
    server.on("/5", HTTP_GET, [](AsyncWebServerRequest *request){
     i=0;
     audio.connecttohost("http://vis.media-ice.musicradio.com/CapitalMP3");
     Serial.println("CapitalMP3");
     lcd.clear();
     lcd.print("Stanice: ");
     lcd.setCursor(0,1);
     lcd.print("CapitalMP3");
     delay(500);
     request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
    server.on("/6", HTTP_GET, [](AsyncWebServerRequest *request){
     i=0;
     audio.connecttohost("http://stream.rockantenne.de/heavy-metal");
     Serial.println("RockAntenne Heavy metal");
     lcd.clear();
     lcd.print("Stanice: ");
     lcd.setCursor(0,1);
     lcd.print("RockAntenne HM");
     delay(500);
     request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
    server.on("/7", HTTP_GET, [](AsyncWebServerRequest *request){
     i=0;
     audio.connecttohost("http://67.212.238.178:8004/stream");
     Serial.println("Old time USA Radio");
     lcd.clear();
     lcd.print("Stanice: ");
     lcd.setCursor(0,1);
     lcd.print("OldTimeUSA Radio");
     delay(500);
     request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
    server.on("/8", HTTP_GET, [](AsyncWebServerRequest *request){
     i=0;
     audio.connecttohost("http://playerservices.streamtheworld.com/api/livestream-redirect/LOS40_ARGENTINA.mp3");
     Serial.println("Los40 Argentina");
     lcd.clear();
     lcd.print("Stanice: ");
     lcd.setCursor(0,1);
     lcd.print("Los40 Argentina");
     delay(500);
     request->send(SPIFFS, "/index.html", String(), false, processor);
  });
}

void setup() {
    Serial.begin(115200);
    pinMode (CLK,INPUT);
    pinMode (DT,INPUT);
    pinMode(TlacitkoPin, INPUT);
    rotace = digitalRead(CLK); 
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) delay(1500);
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    lcd.begin();  
    lcd.backlight();
    lcd.print("WifiRadioJurecek");
    lcd.setCursor(0,1);
    lcd.print("Stiskni tlacitko"); 
    ledcSetup(ledChannel, freq, resolution); // configure LED PWM functionalitites
    ledcAttachPin(output, ledChannel); // attach the channel to the GPIO to be controlled  
    ledcWrite(ledChannel, sliderValue.toInt());
     
    // Initialize SPIFFS
    if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }

    // Connect to Wi-Fi
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }

    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());
  
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });

   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor2);
  });
    
    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/style.css", "text/css");
    });


      // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
      server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam("value")) {
            message = request->getParam("value")->value();
            sliderValue = message;
            ledcWrite(ledChannel, sliderValue.toInt());
            posuvnik = sliderValue.toInt();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });
  
    zmenaStanice2();
  
    // Start server
    server.begin();
}
 
void loop()
{
   TlacitkoStav2 = digitalRead(TlacitkoPin);
   audio.loop();
   if (TlacitkoStav2 == HIGH) {
     zmenaStanice();   
   }
   zvuk();
   zvuk2();
   audio.setVolume(pom3);
}
 
// optional
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreaminfo(const char *info){
    Serial.print("streaminfo  ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}
void audio_eof_speech(const char *info){
    Serial.print("eof_speech  ");Serial.println(info);
}
