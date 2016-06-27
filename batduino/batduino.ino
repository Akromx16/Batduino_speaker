
#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <SdFatUtil.h>

//and the MP3 Shield Library
#include <SFEMP3Shield.h>

//sleep libraries
#include <PinChangeInt.h>
#include <avr/sleep.h>

SdFat sd;
const int buttonPin = 5;
const int transPin = 10;
int buttonState = 0;

SFEMP3Shield MP3player;

//------------------------------------------------------------------------------

void play()
{
  digitalWrite(transPin, HIGH);
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    MP3player.setVolume(2, 2); // commit new volume
    MP3player.playTrack(1);
    while (MP3player.isPlaying() != 0)
      delay(50);
  }
}

void setup() { 
  //Serial.begin(115200);

  digitalWrite(transPin, LOW);
  pinMode(buttonPin, INPUT);
  PCintPort::attachInterrupt(buttonPin, play, RISING);

  if(!sd.begin(SD_SEL, SPI_FULL_SPEED))
    sd.initErrorHalt();
    
  if(!sd.chdir("/")) 
    sd.errorHalt("sd.chdir");

  MP3player.begin();

}

void loop() {
  digitalWrite(transPin, LOW);
  sleep();
}

void sleep(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();
  sleep_disable();
}

