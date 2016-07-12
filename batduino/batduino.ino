// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);


const int buttonPin = 5;
const int frequencyPin = 2;
const int errorLED = 13;

char name_str[12][30] = {"Noctula.wav", "Brown_long_eared.wav", "Common_pip.wav", "Barbastelle.wav",
    "Serotine.wav", "Soprano_pip.wav", "track007.wav", "track008.wav", "track009.wav",
    "track010.wav", "track011.wav", "track012.wav"};
char wav_str[20] = ".mp3";

int buttonState = 0;
int freq = 0;
int right_ultra = 40;
int left_audible = 0;

int analogue[6] = {14, 15, 16, 17, 18, 19};





//------------------------------------------------------------------------------


void setup() { 
  int index = 0;
  
  Serial.begin(115200);
  pinMode(errorLED, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(frequencyPin, INPUT);
  
  digitalWrite(errorLED, LOW);
  delay(1000);

  while (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     //while (1);
     digitalWrite(errorLED, HIGH);
  }
  Serial.println(F("VS1053 found"));

  SD.begin(CARDCS);    // initialise the SD card

  for (index = 0; index < 6; index++)
    pinMode(analogue[index], INPUT);

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(40,100);
  

}

void loop() {
  int result; //result code from some function as to be tested at later time.
  int index = 0;
  // delay(100); // small delay to allow the circuitry to "wake up"
  buttonState = digitalRead(buttonPin);

  
  if (digitalRead(frequencyPin) == HIGH) {
    //Serial.println("Ultrasounds");
    //freq = 6;
    right_ultra = 0;
    left_audible = 200;
  }
  else if (digitalRead(frequencyPin) == LOW) {
    //Serial.println("Audible sounds");
    //freq = 0;
    right_ultra = 200;
    left_audible = 40;
  }
    
  musicPlayer.setVolume(left_audible,right_ultra);

  if (buttonState == HIGH) {
    Serial.println("Button_pressed");
    // check the analogue inputs
    for (index = 0; index < 6; index++) {
      if (digitalRead(analogue[index]) == HIGH) {
           Serial.println(name_str[index]);
           if (musicPlayer.stopped()) {
             result = musicPlayer.playFullFile(name_str[index]);
              Serial.println(result);
            // File is playing in the background
        }
        break;
      }
    }
  }
}


