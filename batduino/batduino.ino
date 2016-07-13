#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>


// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin -- not used

Adafruit_VS1053_FilePlayer musicPlayer = 
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);


const int buttonPin = 5;
const int frequencyPin = 2;
const int errorLED = 13;

char name_str[][20] = {"track001.wav", "track002.wav", "track003.wav", "track004.wav",
    "track005.wav", "track006.wav"};

int freq = 0;

// the volume of left speaker on 40 is as loud as right speaker on 0 (max)
int right_ultra = 0; // right speaker (ultrasounds)
int left_audible = 40; // left speaker (audible)

int analogue[6] = {14, 15, 16, 17, 18, 19};


//------------------------------------------------------------------------------


void setup() { 
  int index = 0;
  Serial.begin(115200);
  pinMode(errorLED, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(frequencyPin, INPUT);
    
  for (index = 0; index < 6; index++)
    pinMode(analogue[index], INPUT);

  digitalWrite(errorLED, LOW);

  while (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     digitalWrite(errorLED, HIGH);
  }
  Serial.println(F("VS1053 found"));

  SD.begin(CARDCS);    // initialise the SD card

  // Set initial volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);
  

}

void loop() {
  int result; //result code from some function as to be tested at later time.
  int index = 0;

  if (digitalRead(frequencyPin) == HIGH) {
    // Audble sounds
    right_ultra = 200;
    left_audible = 40;
  }
  else if (digitalRead(frequencyPin) == LOW) {
    // Ultrasounds
    right_ultra = 0;
    left_audible = 200;
  }

  if (digitalRead(buttonPin) == HIGH) {
    Serial.println("Button_pressed");
    // set the volume accordingly for one speaker to play only
    musicPlayer.setVolume(left_audible,right_ultra);

    // check the analogue inputs
    for (index = 0; index < 6; index++)
      if (digitalRead(analogue[index]) == HIGH) {
        Serial.println(name_str[index]);
        if (musicPlayer.stopped()) {
          result = musicPlayer.playFullFile(name_str[index]);
          Serial.println(result);
        }
        break;
      }
  } // if

} // main loop


