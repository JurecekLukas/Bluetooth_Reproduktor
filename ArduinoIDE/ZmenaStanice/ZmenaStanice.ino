#include <Arduino.h>

int i=0;
const int buttonPin = 2; 
int buttonState = 0;


void zmenaStanice(){
    buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
   if (buttonState == HIGH) {
    i++;
   }


 
  switch(i){
    case 0:
     // audio.connecttohost("http://vis.media-ice.musicradio.com/CapitalMP3");
      Serial.println("Capitalmp3");
      delay(500);
      break;
    case 1:
      Serial.println("kjhglghognfsonh");
      delay(500);
      break;
    case 2:
      Serial.println("Cgggggggggp3");
      delay(500);
      break;
    case 3:
      Serial.println("dsdsdsds3");
      delay(500);
      break;
    case 4:
      Serial.println("Evropa2");
      delay(500);
      break;
    case 5:
      i=0;
      break;

  }
}

void setup() {
  Serial.begin(115200);
    pinMode(buttonPin, INPUT);



}

void loop() {
  zmenaStanice();


}



//    audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");
//    audio.connecttohost("http://macslons-irish-pub-radio.com/media.asx");
//    audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.aac"); //  128k aac
//    audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.mp3"); //  128k mp3
//    audio.connecttohost("http://vis.media-ice.musicradio.com/CapitalMP3"); //  128k mp3
//    audio.connecttospeech("Wenn die Hunde schlafen, kann der Wolf gut Schafe stehlen.", "de");
//    audio.connecttohost("http://media.ndr.de/download/podcasts/podcast4161/AU-20190404-0844-1700.mp3");
