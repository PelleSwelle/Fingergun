#include <Arduino.h>
#include <OPT3101.h>
#include "LIS3DHTR.h"
#include <Wire.h>
LIS3DHTR<TwoWire> LIS; //IIC
#define WIRE Wire

// *************** PINS ***************
const int buttonPin = DD6; // the number of the pushbutton pin
const int ledPin = 4;      // the number of the LED pin
const int buzzPin = 5;
const int lightPin = A6;
//const int sensorPin = 13;
const int sensorPin = 7;

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
    delay(10);
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
    //ammo++;
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
  //Serial.begin(9600);

  // set amount of ammo
  ammo = 10;
  
  // Accelerometer
  Serial.begin(115200);
  while (!Serial)
  {
  };
  LIS.begin(WIRE,0x19); //IIC init 
  //LIS.begin(0x19);
  //LIS.openTemp();  //If ADC3 is used, the temperature detection needs to be turned off.
  LIS.closeTemp();//default
  delay(100);
    LIS.setFullScaleRange(LIS3DHTR_RANGE_2G);
  //  LIS.setFullScaleRange(LIS3DHTR_RANGE_4G);
  //  LIS.setFullScaleRange(LIS3DHTR_RANGE_8G);
  //  LIS.setFullScaleRange(LIS3DHTR_RANGE_16G);
    LIS.setOutputDataRate(LIS3DHTR_DATARATE_1HZ);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_10HZ);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_25HZ);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_100HZ);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_200HZ);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_1_6KHZ);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_5KHZ);
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
  if (!LIS) // Accelerometer starts here
  {
    Serial.println("LIS3DHTR didn't connect.");
    while (1)
      ;
    return;
  }
  //3 axis
    Serial.print("x:"); Serial.print(LIS.getAccelerationX()); Serial.print("  ");
    delay(500);
    Serial.print("y:"); Serial.print(LIS.getAccelerationY()); Serial.print("  ");
    delay(500);
    Serial.print("z:"); Serial.println(LIS.getAccelerationZ());
    delay(500);
  //ADC
  //    Serial.print("adc1:"); Serial.println(LIS.readbitADC1());
  //    Serial.print("adc2:"); Serial.println(LIS.readbitADC2());
  //    Serial.print("adc3:"); Serial.println(LIS.readbitADC3());

  if (LIS3DHTR_REG_ACCEL_OUT_Z_H >= 2) {
    reload();
  }
}
