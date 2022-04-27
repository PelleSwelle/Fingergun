#include <Arduino.h>
#include <OPT3101.h>

// *************** PINS ***************
const int buttonPin = DD6; // the number of the pushbutton pin
const int ledPin = 4;      // the number of the LED pin
const int buzzPin = 5;
const int lightPin = A6;
const int sensorPin = 13;

// *************** PITCHES ***************
int gunPitch = 1000;
int noAmmoPitch = 400;

// *************** GUN ***************
const int magSize = 2;
int ammo;

// *************** UTIL ***************
int buttonState = 0; // variable for reading the pushbutton status
int sensorValue;

void playShootSound()
{
  int startPitch = gunPitch;
  for (int i = startPitch; i > 0; i -= 10)
  {
    analogWrite(ledPin, i);
    tone(buzzPin, i);
    delay(1);
    Serial.println(i);
  }
  noTone(buzzPin);
}

void playNoAmmoSound()
{
  tone(buzzPin, noAmmoPitch);
  delay(100);
  tone(buzzPin, noAmmoPitch);
  delay(100);
  noTone(buzzPin);
}

void reload()
{
  const int reloadTime = 2000;
  ammo += magSize;

  delay(reloadTime);
}

// TODO make this abrubtable
void pressTrigger()
{
  // if out of ammo
  if (ammo <= 0)
  {
    playNoAmmoSound();
    Serial.println("no Ammo");
    // TODO DELETE THIS
    ammo++;
  }
  // if ammo is held
  else
  {
    playShootSound();
    Serial.print("pew!");
    ammo--;
  }
}

void setup()
{
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input and clickable instead of togglable
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(sensorPin, INPUT);

  pinMode(buzzPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  Serial.begin(9600);

  // set amount of ammo
  ammo = 1;
}

void loop()
{
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH)
  {
    pressTrigger();
  }
  else
  {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    analogWrite(buzzPin, 0);
  }
}
