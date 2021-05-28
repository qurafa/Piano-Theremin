// AUTHOR/LICENSE:
// Created by Faruq Afolabi
// Student Number: 217219619
// Lassonde School of Engineering, York University

#include "pitches.h"
#include "NewPing.h"
#include <LiquidCrystal.h>
#include <string.h>

/*
Sources:
NewPing.h: https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home
pitches.h: https://www.arduino.cc/en/Tutorial/BuiltInExamples/toneMelody
*/

//2D array containig the frequencies to play through the speaker, defined in pitches.h
const int NOTES[6][12] =
{{NOTE_C1, NOTE_CS1, NOTE_D1, NOTE_DS1, NOTE_E1, NOTE_F1, NOTE_FS1, NOTE_G1, NOTE_GS1, NOTE_A1, NOTE_AS1, NOTE_B1},
{NOTE_C2, NOTE_CS2, NOTE_D2, NOTE_DS2, NOTE_E2, NOTE_F2, NOTE_FS2, NOTE_G2, NOTE_GS2, NOTE_A2, NOTE_AS2, NOTE_B2},
{NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3},
{NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4},
{NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5},
{NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6}};

//The maximum and minimum ranges for the sensor
const int MIN_RANGE = 2;
const int MAX_RANGE = 36+MIN_RANGE;

//The octave value, used to access the NOTES array, set later in the code
int octave = 0;

//The i value, set based on the distance and used to access the NOTES array
int i;

//The period at which we play through the speaker
int halfT;

//The duration it takes for the pulse to go to the object and back to the sensor
float duration;

//The distance between the object and the sensor, calculated using duration
float distance;

//The interval between each note, in terms of distance
float interval;

//other variables
String note = "";
String row1 = "";
String row2 = "";
int octaveCheck1;
int octaveCheck2;
int octaveCheck3;
int octaveCheck4;
int octaveCheck5;
int octaveCheck6;

//defining the pins used
LiquidCrystal lcd(49,47,43,41,39,37);
#define Trig 25
#define Echo 23
#define Speaker 13
#define octave1 2
#define octave2 5
#define octave3 7
#define octave4 10
#define octave5 11
#define octave6 12

//trig array used to access the buttons pressed
int trig[6] = {octave1,octave2,octave3,octave4,octave5,octave6};

//ping, of type NewPing used to get distance from the ultrasonic sensor
NewPing ping(Trig, Echo, MAX_RANGE);

void setup()
{
//assigning input and output
pinMode(Trig, OUTPUT);
pinMode(Echo, INPUT);
pinMode(Speaker, OUTPUT);
pinMode(octave1, INPUT_PULLUP);
pinMode(octave2, INPUT_PULLUP);
pinMode(octave3, INPUT_PULLUP);
pinMode(octave4, INPUT_PULLUP);
pinMode(octave5, INPUT_PULLUP);
pinMode(octave6, INPUT_PULLUP);

//setting up the display
lcd.begin(16,2);

//calculating the interval incm between each note
interval = (MAX_RANGE - MIN_RANGE)/12;
}

void loop()
{
  //Resetting the values
  duration = 0;
  distance = 0;
  octave = 0;
  halfT = 0;
  i = 0;
  
  //setting the octave based on the button pressed
  if(digitalRead(octave1) == LOW){
    octave = 1;
    digitalWrite(octave1, HIGH);
    }
    else if(digitalRead(octave2) == LOW){
    octave = 2;
    digitalWrite(octave2, HIGH);
    }
    else if(digitalRead(octave3) == LOW){
    octave = 3;
    digitalWrite(octave3, HIGH);
    }
    else if(digitalRead(octave4) == LOW){
    octave = 4;
    digitalWrite(octave4, HIGH);
    }
    else if(digitalRead(octave5) == LOW){
    octave  = 5;
    digitalWrite(octave5, HIGH);
    }
    else if(digitalRead(octave6) == LOW){
    octave = 6;
    digitalWrite(octave6, HIGH);
    }
    
  //Setting the distance and note
  if(octave != 0){
  setDistance();
  setNote();
  }
  
  //Setting the rows od the lcd display
  setDisp();
  
  //Displaying to the lcd
  disp();
  
  //Playing through the speaker if the i and octave have been set
  if(octave != 0 && i != 0)
    play(Speaker, NOTES[octave-1][i-1]);
}

//This function displays to the 16x2 LCD
void disp(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(row1);
  lcd.setCursor(0,1);
  lcd.print(row2);
  }

//This function sets the note and i values based on the distance from the sensor
void setNote(){
  if(MIN_RANGE < distance && distance <= MAX_RANGE && octave != 0){
    if(distance <= interval+MIN_RANGE){note = "C"; i = 1;}
    else if(distance <= (interval*2)+MIN_RANGE){note = "C#"; i = 2;}
    else if(distance <= (interval*3)+MIN_RANGE){note = "D"; i = 3;}
    else if(distance <= (interval*4)+MIN_RANGE){note = "D#"; i = 4;}
    else if(distance <= (interval*5)+MIN_RANGE){note = "E"; i = 5;}
    else if(distance <= (interval*6)+MIN_RANGE){note = "F"; i = 6;}
    else if(distance <= (interval*7)+MIN_RANGE){note = "F#"; i = 7;}
    else if(distance <= (interval*8)+MIN_RANGE){note = "G";  i = 8;}
    else if(distance <= (interval*9)+MIN_RANGE){note = "G#";  i = 9;}
    else if(distance <= (interval*10)+MIN_RANGE){note = "A"; i = 10;}
    else if(distance <= (interval*11)+MIN_RANGE){note = "A#"; i = 11;}
    else if(distance <= (interval*12)+MIN_RANGE){note = "B"; i = 12;}
    }else{
      i = 0;
      note = "";
      }
  }

//This function sets the rows of the lcd if we are in range, otherwise it sets it "--"
void setDisp(){
  if(MIN_RANGE < distance && distance <= (MAX_RANGE+MIN_RANGE) && octave != 0){
    row1 = "Note:";
    row1.concat(note);
    row1.concat(" Octave:");
    row1.concat(octave);
    row2 = "Distance:";
    row2.concat(distance);
    row2.concat("cm");
  }else{
    row1 = "--";
    row2 = "--";
  }
}

//This function plays through the speaker, as long as we're in range and a button is being pressed, taking in the pin and frequency as input
void play(int pin, int freq){
  //setting the period at which to play through the speaker withthe given frequency
  halfT = (int)(float(1000000) / float(2 * freq) + 0.5);
  
  //while the octave button is still pressed and the we're still in range
  while((digitalRead(trig[octave-1]) == LOW) && ((MIN_RANGE <= distance) && (distance <= MAX_RANGE))){
    if((((interval*(i-1)+MIN_RANGE) <= distance) && (distance <= (interval*i)+MIN_RANGE))){
        digitalWrite(pin, 0);
        delayMicroseconds(halfT);
        digitalWrite(pin, 1);
        delayMicroseconds(halfT);
        }else{break;}
        //update the distance for the if-statement check
        setDistance();
    }
  }

//This function sets the distance from the ultrasonic sensor, using the NewPing library
void setDistance(){
  duration = ping.ping();
  distance = duration*0.034/2;
}
