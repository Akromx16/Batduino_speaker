
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

int analogue[6] = {14, 15, 16, 17, 18, 19};

SFEMP3Shield MP3player;

//------------------------------------------------------------------------------

void play()
{
  int index = 0;
  digitalWrite(transPin, HIGH);
  delay(100); // small delay to allow the circuitry to "wake up"
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    // check the analogue inputs
    for (index = 0; index < 6; index++) {
      if (digitalRead(analogue[index]) == HIGH) {
        MP3player.setVolume(2, 2); // commit new volume
        MP3player.playTrack(index + 1);
        while (MP3player.isPlaying() != 0)
          delay(50);
        break;
      }
    }
  }
}

void setup() { 
  int index = 0;
  //Serial.begin(115200);

  digitalWrite(transPin, LOW);
  pinMode(buttonPin, INPUT);
  
  for (index = 0; index < 6; index++)
    pinMode(analogue[index], INPUT);

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

