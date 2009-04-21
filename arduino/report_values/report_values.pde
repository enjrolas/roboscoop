/*

 This program lies in an infinite loop.
 when it receives a newline ('\n') character, it will return a total of 11 bytes.  The first 9 bytes are, in order
 the high byte of the compass value
 the low byte ''    ''      '' 
 the battery level coming off the a2d
 255 if the front bumper is pressed, 0 if it's not
 255 if the rear bumper is pressed, 0 if it's not
 the poop sensor level coming off the a2d
 the front rangefinder level coming off the a2d
 the rear rangefinder level coming off the a2d
 whether or not there's power going to the motors (255 if there is, 0 if there's not)
 
 
 
 
 The function readCompass() is based off of CMPS03 with arduino I2C example
 
 This will display a value of 0 - 359 for a full rotation of the compass.
 
 The SDA line is on analog pin 4 of the arduino and is connected to pin 3 of the CMPS03.
 The SCL line is on analog pin 5 of the arduino and is conected to pin 2 of the CMPS03.
 Both SDA and SCL are also connected to the +5v via a couple of 1k8 resistors.
 A switch to callibrate the CMPS03 can be connected between pin 6 of the CMPS03 and the ground.
 */
#include <Wire.h>

#define address 0x60 //defines address of compass
#define numValues 9  //how many bytes we're sending back

#define compassHighByte 0
#define compassLowByte 1
#define batteryLevel 2
#define frontBumper 3
#define backBumper 4
#define poopLevel 5
#define frontRange 6
#define backRange 7
#define motorPower 8

#define frontBumperPin 3
#define backBumperPin 4
#define motorPowerPin 5

#define batteryPin 0  //the battery voltage is on analog pin 0
#define poopPin 1
#define frontRangePin 2
#define backRangePin 3

byte values[10];
byte checksum;

void setup(){
  Wire.begin(); //conects I2C
  pinMode(frontBumperPin,INPUT);  //front bumper
  pinMode(backBumperPin,INPUT);  //rear bumper
  pinMode(motorPowerPin,INPUT);  //motor power
  //  digitalWrite(frontBumperPin,HIGH);  //front bumper pullup
  //  digitalWrite(backBumperPin,HIGH);  //rear bumper pullup
  //  digitalWrite(motorPowerPin,HIGH);  //motor power pullup
  Serial.begin(19200);
}

void readCompass()
{
  Wire.beginTransmission(address);      //starts communication with cmps03
  Wire.send(2);                         //Sends the register we wish to read
  Wire.endTransmission();

  Wire.requestFrom(address, 2);        //requests high byte
  while(Wire.available() < 2);         //while there is a byte to receive
  values[compassHighByte] = Wire.receive();           //reads the byte as an integer
  values[compassLowByte] = Wire.receive();
}

void loop(){
  char in;
  if(Serial.available()>0)
  {
    in=Serial.read();
    readValues(in);
    computeChecksum();
    reportValues();
  }
}

void readValues(char in)
{
  if(in=='c')
    readCompass();  //'readcompass' is blocking, so if the compass is not connected, send a character other than 'c'
  values[batteryLevel]=analogRead(batteryPin)>>2;
  values[poopLevel]=analogRead(poopPin)>>2;


  if(digitalRead(frontBumperPin)==HIGH)
    values[frontBumper]=255;
  else
    values[frontBumper]=0;

  if(digitalRead(backBumperPin)==HIGH)
    values[backBumper]=255;
  else
    values[backBumper]=0;

  if(digitalRead(motorPowerPin)==HIGH)
    values[motorPower]=255;
  else
    values[motorPower]=0;

}

void computeChecksum()
{
  int i;
  checksum=0;
  for(i=0;i<numValues;i++)
    checksum+=values[i];
}

void reportValues()
{
  int i;
  for(i=0;i<numValues;i++)
  {
    Serial.print(values[i]);
  }
  Serial.print(checksum);
  Serial.println();
}
