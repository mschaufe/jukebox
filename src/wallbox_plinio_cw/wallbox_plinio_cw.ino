#include <Wire.h>
#include <SPI.h>
#include <SPI.h>
#include <SFEMP3Shield.h>
#include <SparkFunSX1509.h>
#include "SparkFun_Qwiic_Relay.h"
#include <SdFat.h>
#include <FreeStack.h>

SdFat sd;
SFEMP3Shield MP3player;

const byte SX1509_ADDRESS_1 = 0x3E;
const byte SX1509_ADDRESS_2 = 0x70;
const byte RELAY_ADDR = 0x18;
SX1509 io1, io2;
Qwiic_Relay relay(RELAY_ADDR); 

int PINS_AK[10];
int PINS_LV[10];
int PINS_10[10];

char pressedLetter;
int pressedNumber;
int pressedSongNr;

uint8_t result; //result code from some function as to be tested at later time.
char trackName[] = "track001.mp3";

const int OUTPUT_PIN = 5;

int playList[10] = {0,0,0,0,0,0,0,0,0,0};


void setup() {

  digitalWrite(OUTPUT_PIN, HIGH);
  pinMode(OUTPUT_PIN, OUTPUT);
  
  Serial.begin(115200);
  Serial.println("Start");

  Wire.begin();

  if (io1.begin(SX1509_ADDRESS_1) == false)
  {
    Serial.println("Failed to communicate. Check wiring and address of SX1509 1.");
    while (1)
      ; // If we fail to communicate, loop forever.
  }
  Serial.println("SX1509 1 detected");

  for(int i=0;i<16;i++)
  {
    io1.pinMode(i, INPUT_PULLUP);
  }

  if (io2.begin(SX1509_ADDRESS_2) == false)
  {
    Serial.println("Failed to communicate. Check wiring and address of SX1509 2.");
    while (1)
      ; // If we fail to communicate, loop forever.
  }
  Serial.println("SX1509 2 detected");

  for(int i=0;i<16;i++)
  {
    io2.pinMode(i, INPUT_PULLUP);
  }

  if(!relay.begin()){
    Serial.println("Check connections to Qwiic Relay.");
  }
  else
  {
    float version = relay.singleRelayVersion();
    Serial.print("Relay ok; firmware version: ");
    Serial.println(version);
    relay.turnRelayOff(); 
  }

  initSD(); 
  MP3player.begin();

}

void loop() {
  if(MP3player.isPlaying()){
    Serial.println("is playing");
    }
  readFromIO();
  //printIoPinsInSerialMonitor();
  processIoInputToPressedButtons();
  printPressedKeysAndSongNumber();
  checkIfNumberAndLetterIsPressed();
  showPlaylist();
  playFirstSongFromPlaylist();
  delay(1000);
}

void readFromIO(){
  // read io1
  for(int i=0;i<10;i++)
  {
    PINS_AK[i] = io1.digitalRead(i);
  }
  for(int i=10;i<16;i++)
  {
    PINS_LV[i-10] = io1.digitalRead(i);
  }
  
  // read io2
  for(int i=10;i<14;i++)
  {
    PINS_LV[i-4] = io2.digitalRead(i);
  }
  for(int i=0;i<10;i++)
  {
    PINS_10[i] = io2.digitalRead(i);
  }
}

void printIoPinsInSerialMonitor() {
  Serial.print("\n\rPINS_AK: ");
  for(int i=0;i<10;i++)
  {
    Serial.print(PINS_AK[i]);
  } 

  Serial.print("\n\rPINS_LV: ");
  for(int i=0;i<10;i++)
  {
    Serial.print(PINS_LV[i]);
  }
  
  Serial.print("\n\rPINS_10: ");
  for(int i=0;i<10;i++)
  {
    Serial.print(PINS_10[i]);
  }
  Serial.println("");
}

void processIoInputToPressedButtons(){
  int highIOs_AK = 0;
  for(int i=0;i<10;i++)
  {
    highIOs_AK = highIOs_AK + PINS_AK[i];
  }
  
  int highIOs_LV = 0;
  for(int i=0;i<10;i++)
  {
    highIOs_LV = highIOs_LV + PINS_LV[i];
  }

  if(highIOs_AK != 0 and highIOs_LV != 0)
  {
    delay(500);
    // flush key
  }
  
  int highIOs_10 = 0;
  for(int i=0;i<10;i++)
  {
    highIOs_10 = highIOs_10 + PINS_10[i];
  }
  
  pressedSongNr = 0;
  pressedLetter = 0;
  pressedNumber = 0;
  
  switch (highIOs_10) {
    case 0:
      break;
    case 1:
      pressedSongNr = 100;
      pressedNumber = 1;
      break;
    case 2:
      pressedSongNr = 80;
      pressedNumber = 2;
      break;
    case 3:
      pressedSongNr = 60;
      pressedNumber = 3;
      break;
    case 4:
      pressedSongNr = 40;
      pressedNumber = 4;
      break;
    case 5:
      pressedSongNr = 20;
      pressedNumber = 5;
      break;
    case 6:
      pressedSongNr = 0;
      pressedNumber = 6;
      break;
    case 7:
      pressedSongNr = 120;
      pressedNumber = 7;
      break;
    case 8:
      pressedSongNr = 140;
      pressedNumber = 8;
      break;
    case 9:
      pressedSongNr = 160;
      pressedNumber = 9;
      break;
    case 10:
      pressedSongNr = 180;
      pressedNumber = 10;
      break;
  }

  switch (highIOs_AK) {
    case 0:
      break;
    case 1:
      pressedSongNr = pressedSongNr + 1;
      pressedLetter = 'A';
      break;
    case 2:
      pressedSongNr = pressedSongNr + 2;
      pressedLetter = 'B';
      break;
    case 3:
      pressedSongNr = pressedSongNr + 3;
      pressedLetter = 'C';
      break;
    case 4:
      pressedSongNr = pressedSongNr + 4;
      pressedLetter = 'D';
      break;
    case 5:
      pressedSongNr = pressedSongNr + 5;
      pressedLetter = 'E';
      break;
    case 6:
      pressedSongNr = pressedSongNr + 6;
      pressedLetter = 'F';
      break;
    case 7:
      pressedSongNr = pressedSongNr + 7;
      pressedLetter = 'G';
      break;
    case 8:
      pressedSongNr = pressedSongNr + 8;
      pressedLetter = 'H';
      break;
    case 9:
      pressedSongNr = pressedSongNr + 9;
      pressedLetter = 'J';
      break;
    case 10:
      pressedSongNr = pressedSongNr + 10;
      pressedLetter = 'K';
      break;
  }


  switch (highIOs_LV) {
    case 0:
      break;
    case 1:
      pressedSongNr = pressedSongNr + 11;
      pressedLetter = 'L';
      break;
    case 2:
      pressedSongNr = pressedSongNr + 12;
      pressedLetter = 'M';
      break;
    case 3:
      pressedSongNr = pressedSongNr + 13;
      pressedLetter = 'N';
      break;
    case 4:
      pressedSongNr = pressedSongNr + 14;
      pressedLetter = 'P';
      break;
    case 5:
      pressedSongNr = pressedSongNr + 15;
      pressedLetter = 'Q';
      break;
    case 6:
      pressedSongNr = pressedSongNr + 16;
      pressedLetter = 'R';
      break;
    case 7:
      pressedSongNr = pressedSongNr + 17;
      pressedLetter = 'S';
      break;
    case 8:
      pressedSongNr = pressedSongNr + 18;
      pressedLetter = 'T';
      break;
    case 9:
      pressedSongNr = pressedSongNr + 19;
      pressedLetter = 'U';
      break;
    case 10:
      pressedSongNr = pressedSongNr + 20;
      pressedLetter = 'V';
      break;
  }
}

void checkIfNumberAndLetterIsPressed(){
  if(pressedLetter == 0 or pressedNumber == 0)
  {
    
  }
  else
  {
    releasePressedKeys();
    sprintf(trackName, "track%03d.mp3", pressedSongNr); //Splice the new file number into this file name
    add_song_to_playlist(pressedSongNr);
  }
}

void add_song_to_playlist(int song_nr){
  for(int i = 0; i < 10; i++){
    if(playList[i] == 0){
      playList[i] = song_nr;
      break;
    }
  }
  if(playList[9] != 0){
    Serial.println("Playlist is full");
  }
}
void remove_played_song(){
  for(int i = 0; i<10-1; i++){
    playList[i] = playList[i+1];
  }
  playList[9] = 0;
}

void showPlaylist(){
  for(int i = 0; i < 10; i++){
   Serial.print(playList[i]);Serial.print(", ");
  }
  Serial.println("");
}

void playFirstSongFromPlaylist(){
  if (playList[0] == 0){
    Serial.println("playlist is empty");
  }
  else
  {
    Serial.print("Start playing song nr:");Serial.println(playList[0]);
    remove_played_song();
  }  
}
    
void printPressedKeysAndSongNumber(){
  if(pressedLetter == 0 or pressedNumber == 0)
  {
    //Serial.println("No selection made!");
  }
  else
  {
    Serial.print("Pressed keys:");Serial.print(pressedLetter);Serial.println(pressedNumber);
    Serial.print("Pressed song nummer: ");Serial.println(pressedSongNr);
    Serial.println("");
  }
}
void releasePressedKeys(){
  delay(1200);
  relay.turnRelayOn(); 
  delay(500);
  relay.turnRelayOff(); 
}

void initSD()
{
  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) digitalWrite(OUTPUT_PIN, LOW);
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  //if(!sd.chdir("/")) sd.errorHalt("sd.chdir");
}