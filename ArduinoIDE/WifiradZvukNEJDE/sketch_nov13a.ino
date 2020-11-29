#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
 
// Digital I/O used
#define I2S_DOUT      22  // DIN connection
#define I2S_BCLK      26  // Bit clock
#define I2S_LRC       25  // Left Right Clock
 int CLK = 14;  // Pin 9 na CLK 
 int DT = 12;  // Pin 8 na DT

 int pozice = 0; 
 int rotace;  
 int pom;
 int hlasitost = 0;
 boolean ZlevaDoprava;
 
Audio audio;
 
String ssid =     "Insanenet";
String password = "insane68";

int zvuk(){
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
          Serial.println ("Vase hlasitost je na urovni = ");
          Serial.println (hlasitost);
          Serial.println ("   Maximum");
          return hlasitost;
         }
         else{ //pokud nebude ještě na hodnotě 20 přičítáme o 1
          hlasitost++;
          Serial.println ("Vase hlasitost je na urovni = ");
          Serial.println (hlasitost);
          return hlasitost;
         }     
         
  
       }else{        // otáčení doleva 
        if (hlasitost <1){ //pokud hlasitost bude nižší než 1 nastavíme na 0 (MIN)
          hlasitost =0;
          Serial.println ("Vase hlasitost je na urovni = ");
          Serial.println (hlasitost);
          Serial.println ("   Minimum");
          return hlasitost;
         }
         else{//pokud nebude ještě na hodnotě 0 odečítáme o 1
          hlasitost--;
          Serial.println ("Vase hlasitost je na urovni = ");
          Serial.println (hlasitost);
          return hlasitost;
         } 
       }
       //Výpis pozice encodéru
       Serial.print("Pozice encoderu: ");
       Serial.println(pozice);
       
     } 
   rotace = pom;
}
 
void setup() {
    Serial.begin(115200);
    pinMode (CLK,INPUT);
    pinMode (DT,INPUT);
    rotace = digitalRead(CLK); 
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) delay(1500);
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(zvuk()); // 0...21
 
//    audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");
//    audio.connecttohost("http://macslons-irish-pub-radio.com/media.asx");
//    audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.aac"); //  128k aac
//     audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.mp3"); //  128k mp3
     audio.connecttohost("http://vis.media-ice.musicradio.com/CapitalMP3"); //  128k mp3
//    audio.connecttospeech("Wenn die Hunde schlafen, kann der Wolf gut Schafe stehlen.", "de");
//    audio.connecttohost("http://media.ndr.de/download/podcasts/podcast4161/AU-20190404-0844-1700.mp3");
// podcast
}
 
void loop()
{
    audio.loop();
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
