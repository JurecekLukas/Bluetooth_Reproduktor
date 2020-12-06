#include "Arduino.h"
#include "WiFi.h"
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
 int rotace;  
 int pom;
 int hlasitost = 0;
 boolean ZlevaDoprava;


//Proměnné pro změnu stanice
 int i=0;
 const int TlacitkoPin = 2;
 int TlacitkoStav = 0; 
 int TlacitkoStav2 = 0;


 
Audio audio;
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
String ssid =     "XXXXXX";   //SSID (název Wifi)
String password = "XXXXXX";    //Heslo


//Funkce na změnu hlasitosti
void zvuk(){
  pom = digitalRead(CLK);
     if (pom != rotace){          //pomocí pinu DT zjišťujeme otočení
       if (digitalRead(DT) != pom) {  
          //otočení doprava
         pozice ++;
         ZlevaDoprava = true;
       } else { 
        //otočení doleva
         ZlevaDoprava = false;
         pozice--;
       }
       
       if (ZlevaDoprava){        // otáčení doprava
        if (hlasitost >20){      //pokud hlasitost bude vyšší než 20 nastavíme na 21 (MAX)
          hlasitost =21;
         }
         else{                  //pokud nebude ještě na hodnotě 20 přičítáme o 1
          hlasitost++;
         }     
         
  
       }else{                   // otáčení doleva 
        if (hlasitost <1){      //pokud hlasitost bude nižší než 1 nastavíme na 0 (MIN)
          hlasitost =0;
         }
         else{                  //pokud nebude ještě na hodnotě 0 odečítáme o 1
          hlasitost--;
         } 
       }
       
     } 
   rotace = pom;
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
      audio.connecttohost("http://vis.media-ice.musicradio.com/CapitalMP3");
      Serial.println("CapitalMP3");
      lcd.clear();
      lcd.print("Stanice: ");
      lcd.setCursor(0,1);
      lcd.print("CapitalMP3");
      delay(500);
      break;
    case 3:
      audio.connecttohost("https://ice.abradio.cz/orion128.mp3");
      Serial.println("Orion");
      lcd.clear();
      lcd.print("Stanice: ");
      lcd.setCursor(0,1);
      lcd.print("Hitradio Orion");
      delay(500);
      break;
    case 4:
      audio.connecttohost("http://stream.rockantenne.de/heavy-metal");
      Serial.println("RockAntenne Heavy metal");
      lcd.clear();
      lcd.print("Stanice: ");
      lcd.setCursor(0,1);
      lcd.print("RockAntenne HM");
      delay(500);
      break;
    case 5:
      audio.connecttohost("http://67.212.238.178:8004/stream");
      Serial.println("Old time USA Radio");
      lcd.clear();
      lcd.print("Stanice: ");
      lcd.setCursor(0,1);
      lcd.print("OldTimeUSA Radio");
      delay(500);
      break;
    case 6:
      i=0;
      break;

  }
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
 // audio.setVolume(hlasitost); // 0...21
 
//    audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");
//    audio.connecttohost("http://macslons-irish-pub-radio.com/media.asx");
//    audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.aac"); //  128k aac
//    audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.mp3"); //  128k mp3
//    audio.connecttohost("http://vis.media-ice.musicradio.com/CapitalMP3"); //  128k mp3
//    audio.connecttohost("http://21283.live.streamtheworld.com/EVROPA2.mp3"); //EVROPA2
//    audio.connecttospeech("Wenn die Hunde schlafen, kann der Wolf gut Schafe stehlen.", "de");
//    audio.connecttohost("http://media.ndr.de/download/podcasts/podcast4161/AU-20190404-0844-1700.mp3");
// podcast
}
 
void loop()
{
   TlacitkoStav2 = digitalRead(TlacitkoPin);
   audio.loop();
   if (TlacitkoStav2 == HIGH) {
     zmenaStanice();
   }
    zvuk();
    audio.setVolume(hlasitost);
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