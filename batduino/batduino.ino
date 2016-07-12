
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
const int frequencyPin = 4;
const int errorLED = 13;
int buttonState = 0;
int freq = 0;

int analogue[6] = {14, 15, 16, 17, 18, 19};

SFEMP3Shield MP3player;

//------------------------------------------------------------------------------


void setup() { 
  int index = 0;
  uint8_t result; //result code from some function as to be tested at later time.
  
  Serial.begin(115200);
  //digitalWrite(frequencyPin, LOW);
  pinMode(errorLED, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(frequencyPin, INPUT);
  
  digitalWrite(errorLED, HIGH);
  delay(3000);

  Serial.println("Begin");
  //digitalWrite(errorLED, HIGH);

  for (index = 0; index < 6; index++)
    pinMode(analogue[index], INPUT);


  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) {
    sd.initErrorHalt();
    Serial.println("Error_1");
    //digitalWrite(errorLED, HIGH);
    
  }
    
  if(!sd.chdir("/")) {
    sd.errorHalt("sd.chdir");
    Serial.println("Error_2");
    //digitalWrite(errorLED, HIGH);
  }
  delay(100);
  result = MP3player.begin();

  if(result != 0) {
    Serial.print(F("Error code: "));
    Serial.println(result);
  }
}

void loop() {
  int index = 0;
  // delay(100); // small delay to allow the circuitry to "wake up"
  buttonState = digitalRead(buttonPin);

  
  if (digitalRead(frequencyPin) == HIGH)
    freq = 4;
  else
    freq = 0;


  if (buttonState == HIGH) {
    Serial.println("Button_pressed");
    // check the analogue inputs
    for (index = 0; index < 6; index++) {
      if (digitalRead(analogue[index]) == HIGH) {
        MP3player.setVolume(2, 2); // commit new volume
        uint8_t result = MP3player.playTrack(index + 1 + freq);
        if(result != 0) {
          Serial.print(F("Error code: "));
          Serial.print(result);
          Serial.println(F(" when trying to play track"));
        }
        
        while (MP3player.isPlaying() != 0)
          delay(50);
        break;
      }
    }
  }
}


