#include <Bounce2.h>

#define LED_PIN 2

#define NUM_KEYS_NUMB 10
#define NUM_KEYS_ALPH_AK 10
#define NUM_KEYS_ALPH_LV 10
#define NUM_KEYS_COIN 3
#define BUTTON_STOP_MOTOR 2

const uint8_t PINS_10[NUM_KEYS_NUMB] = {34,36,38,40,42,44,46,48,50,52};
const uint8_t PINS_AK[NUM_KEYS_ALPH_AK] = {32,30,28,26,24,33,31,29,27,25};
const uint8_t PINS_LV[NUM_KEYS_ALPH_LV] = {35,37,39,41,43,45,47,49,51,53};
const uint8_t PINS_COIN[NUM_KEYS_COIN] = {14,15,16};


int relay_another_coin_req = 4; 
int relay_make_any_selection = 7;
int relay_only_10c_selection = 8;
int relay_motor = 12;

int ledState = LOW;


Bounce * button_PINS_10 = new Bounce[NUM_KEYS_NUMB];
Bounce * button_PINS_AK = new Bounce[NUM_KEYS_ALPH_AK];
Bounce * button_PINS_LV = new Bounce[NUM_KEYS_ALPH_LV];
Bounce * button_PINS_COIN = new Bounce[NUM_KEYS_COIN];

Bounce button_STOP_MOTOR = Bounce(); 

int balance = 100;

int keyNumber;
int keyLetter;

int MOTOR_ON_FLAG = 0;

void setup() {
  
  // ------------------ Key 1 - 0 -------------------------- //
 
  for (int i = 0; i < NUM_KEYS_NUMB; i++) {
    button_PINS_10[i].attach( PINS_10[i] , INPUT_PULLUP  );
    button_PINS_10[i].interval(50);
  }
  
  // ------------------ Key A - K -------------------------- //

  for (int i = 0; i < NUM_KEYS_ALPH_AK; i++) {
    button_PINS_AK[i].attach( PINS_AK[i] , INPUT_PULLUP  );
    button_PINS_AK[i].interval(50);
  }
  // ------------------ Key L - V -------------------------- //

  for (int i = 0; i < NUM_KEYS_ALPH_LV; i++) {
    button_PINS_LV[i].attach( PINS_LV[i] , INPUT_PULLUP  );
    button_PINS_LV[i].interval(50);
  }

  // ------------------ SLUG REJECTOR  --------------------- //

  for (int i = 0; i < NUM_KEYS_COIN; i++) {
    button_PINS_COIN[i].attach( PINS_COIN[i] , INPUT_PULLUP  );
    button_PINS_COIN[i].interval(10);
  }

  // ------------------------------------------------------- //

  
  // Setup the LED :
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);
  
  // Setup the RELAYS :
  pinMode(relay_another_coin_req, OUTPUT);
  pinMode(relay_make_any_selection, OUTPUT);
  pinMode(relay_only_10c_selection, OUTPUT);
  pinMode(relay_motor, OUTPUT);

  pinMode(BUTTON_STOP_MOTOR,INPUT_PULLUP);
  button_STOP_MOTOR.attach(BUTTON_STOP_MOTOR);
  button_STOP_MOTOR.interval(4);
  
  Serial.begin(115200); 
  

  if (balance >= 100){
    Serial.print("so0");Serial.print(";");
  }
  Serial.print("ba");Serial.println(balance);
  
}



void loop() {

  // ------------------ Motor ------------------------- //
  
  button_STOP_MOTOR.update();
  if ( button_STOP_MOTOR.rose() ) {
    Serial.print("Motor stop");
    MOTOR_ON_FLAG = 0;
    digitalWrite(relay_motor, LOW);
  }

  // ------------------ Coin -------------------------- //

  button_PINS_COIN[0].update();
  if ( button_PINS_COIN[0].fell() ) {
    balance = balance + 100;
    Serial.print("ba");Serial.println(balance);
  }
  button_PINS_COIN[1].update();
  if ( button_PINS_COIN[1].fell() ) {
    balance = balance + 50;
    Serial.print("ba");Serial.println(balance);
  }
  button_PINS_COIN[2].update();
  if ( button_PINS_COIN[2].fell() ) {
    balance = balance + 20;
    Serial.print("ba");Serial.println(balance);
  }

  if (balance >= 100){
    digitalWrite(relay_make_any_selection, HIGH);
  }else{
    digitalWrite(relay_make_any_selection, LOW);
  }

  if (balance < 100 && balance >= 50){
    digitalWrite(relay_only_10c_selection, HIGH);
  }else{
    digitalWrite(relay_only_10c_selection, LOW);
  }
  
 if (balance < 50){
    digitalWrite(relay_another_coin_req, HIGH);
  }else{
    digitalWrite(relay_another_coin_req, LOW);
  }
  
  // ------------------ Key -------------------------- //

  button_STOP_MOTOR.update();
  if ( button_STOP_MOTOR.rose() ) {
    Serial.print("Motor stop");
    MOTOR_ON_FLAG = 0;
    digitalWrite(relay_motor, LOW);
  }

  // Update the Bounce instance :
  button_PINS_10[0].update();
  button_PINS_AK[0].update();
  button_PINS_LV[0].update();

  if ( button_PINS_10[0].rose() ) {
    check_keyboard_10();
  }
  
  if ( button_PINS_10[0].fell() ) {
    check_keyboard_10();
  }
  button_STOP_MOTOR.update();
  if ( button_STOP_MOTOR.rose() ) {
    Serial.print("Motor stop");
    MOTOR_ON_FLAG = 0;
    digitalWrite(relay_motor, LOW);
  }
  if ( button_PINS_AK[0].rose() ) {
    check_keyboard_AK();
  }
  
  if ( button_PINS_AK[0].fell() ) {
    check_keyboard_AK();
  }
  button_STOP_MOTOR.update();
  if ( button_STOP_MOTOR.rose() ) {
    Serial.println("Motor stop");
    MOTOR_ON_FLAG = 0;
    digitalWrite(relay_motor, LOW);
  }

  if ( button_PINS_LV[0].rose() ) {
    check_keyboard_LV();
  }
  
  if ( button_PINS_LV[0].fell() ) {
    check_keyboard_LV();
  }
  

 // ------------------ Play -------------------------- //

 if(keyNumber!=NULL && keyLetter!=NULL && balance >= 50){
  
  if (keyNumber == 1){
    keyNumber = 0;
  }

  int title_nr = keyLetter + keyNumber;
  Serial.print("so");Serial.print(title_nr);

  balance = balance - 50;
  Serial.print(";");Serial.print("ba");Serial.println(balance);

  digitalWrite(relay_motor, HIGH);
  
  unsigned long starttime = millis();
  unsigned long endtime = starttime;
  while((endtime - starttime) <=7000){ // do this loop for up to 1000mS

    endtime = millis();
  
    button_STOP_MOTOR.update();
    if ( button_STOP_MOTOR.rose() ) {
      //Serial.print("Without");
      MOTOR_ON_FLAG = 0;
      delay(1000);
      digitalWrite(relay_motor, LOW);
      break;
      
    }
  
  }
  //Serial.println("Watchdog");
  keyNumber=NULL;
  keyLetter=NULL;
 }

 // ------------------ Motor ------------------------ //
 if (MOTOR_ON_FLAG == 1){
  delay(500);
  digitalWrite(relay_motor, HIGH);
  delay(6200);
  digitalWrite(relay_motor, LOW);
  MOTOR_ON_FLAG = 0;
  }
}


void check_keyboard_10() {
  int decode_number = 0;
  for (int i = 0; i < NUM_KEYS_NUMB; i++){
    if (!digitalRead(PINS_10[i]) == 0){
      decode_number++;
    }
    //Serial.print(!digitalRead(PINS_10[i]));
    
  }
  //Serial.println(" ;1-0");
  switch (decode_number) {
    case 0:
      //Serial.print("keine Eingabe");
      break;
    case 1:
      keyNumber = 100; // 1
      //Serial.println(keyNumber);
      break;
    case 2:
      keyNumber = 80; // 2
      //Serial.println(keyNumber);
      break;
    case 3:
      keyNumber = 60;
      //Serial.println(keyNumber);
      break;
    case 4:
      keyNumber = 40;
      //Serial.println(keyNumber);
      break;
    case 5:
      keyNumber = 20;
      //Serial.println(keyNumber);
      break;
    case 6:
      keyNumber = 1;
      //Serial.println("6");
      break;
    case 7:
      keyNumber = 120;
      //Serial.println(keyNumber);
      break;
    case 8:
      keyNumber = 140;
      //Serial.println(keyNumber);
      break;
    case 9:
      keyNumber = 160;
      //Serial.println(keyNumber);
      break;
    case 10:
      keyNumber = 180;
      //Serial.println(keyNumber);
      break;
  }
}

void check_keyboard_AK() {
  int decode_char = 0;
  for (int i = 0; i < NUM_KEYS_ALPH_AK; i++){
    if (!digitalRead(PINS_AK[i]) == 0){
      decode_char++;
    }
    //Serial.print(!digitalRead(PINS_AK[i]));
    
  }
  //Serial.println(" ;a-k");
  switch (decode_char) {
    case 0:
      //Serial.print("keine Eingabe");
      break;
    case 1:
      keyLetter = 1; //A
      //Serial.print("A");
      break;
    case 2:
      keyLetter = 2; //B
      //Serial.print(keyLetter);
      break;
    case 3:
      keyLetter = 3; //C
      //Serial.print(keyLetter);
      break;
    case 4:
      keyLetter = 4;
      //Serial.print(keyLetter);
      break;
    case 5:
      keyLetter = 5;
      //Serial.print(keyLetter);
      break;
    case 6:
      keyLetter = 6;
      //Serial.print(keyLetter);
      break;
    case 7:
      keyLetter = 7;
      //Serial.print(keyLetter);
      break;
    case 8:
      keyLetter = 8;
      //Serial.print(keyLetter);
      break;
    case 9:
      keyLetter = 9;
      //Serial.print(keyLetter);
      break;
    case 10:
      keyLetter = 10;
      //Serial.print(keyLetter);
      break;
  }
  //Serial.println();
}

void check_keyboard_LV() {
  int decode_char = 0;
  for (int i = 0; i < NUM_KEYS_ALPH_LV; i++){
    if (!digitalRead(PINS_LV[i]) == 0){
      decode_char++;
    }
    //Serial.print(!digitalRead(PINS_LV[i]));
    
  }
  //Serial.println(" ;l-v");
  switch (decode_char) {
    case 0:
      //Serial.print("keine Eingabe");
      break;
    case 1:
      keyLetter = 11;
      //Serial.print(keyLetter);
      break;
    case 2:
      keyLetter = 12;
      //Serial.print(keyLetter);
      break;
    case 3:
      keyLetter = 13;
      //Serial.print(keyLetter);
      break;
    case 4:
      keyLetter = 14;
      //Serial.print(keyLetter);
      break;
    case 5:
      keyLetter = 15;
      //Serial.print(keyLetter);
      break;
    case 6:
      keyLetter = 16;
      //Serial.print(keyLetter);
      break;
    case 7:
      keyLetter = 17;
      //Serial.print(keyLetter);
      break;
    case 8:
      keyLetter = 18;
      //Serial.print(keyLetter);
      break;
    case 9:
      keyLetter = 19;
      //Serial.print(keyLetter);
      break;
    case 10:
      keyLetter = 20;
      //Serial.print(keyLetter);
      break;
  }
  //Serial.println();
}
