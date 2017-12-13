#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include "LCD.h"
#include "LiquidCrystal_I2C.h"
#include <SoftwareSerial.h>


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);
Adafruit_ADS1115 ads(0x48);
SoftwareSerial mySerial(3, 2); // RX, TX

float Voltage0 = 0.0;
float Voltage1 = 0.0;
float Voltage2 = 0.0;
float Voltage3 = 0.0;

float Amp0 = 0.0;
float Amp1 = 0.0;
float Amp2 = 0.0;
float Amp3 = 0.0;

int analogInput = 0;
float vout = 0.0;
float vin = 0.0;
float R1 = 100000.0; // resistance of R1 (100K) -see text!
float R2 = 10000.0; // resistance of R2 (10K) - see text!
int value = 0;

void setup(void) 
{
  lcd.begin (16, 2); // 16 x 2 LCD module
  lcd.setBacklightPin(3, POSITIVE); // BL, BL_POL
  lcd.setBacklight(HIGH);
  
  Serial.begin(9600);
  mySerial.begin(9600);
    
  ads.begin();
}

void loop(void) 
{
  int16_t adc0;  // we read from the ADC, we have a sixteen bit integer as a result
  int16_t adc1; 
  int16_t adc2; 
  int16_t adc3;
   
  adc0 = ads.readADC_SingleEnded(0);
  Voltage0 = (adc0 * 0.1875)/1000;
  Voltage0 = Voltage0*100;
  Amp0 = map(Voltage0, 0, 500, 0, 2000);
  
  adc1 = ads.readADC_SingleEnded(1);
  Voltage1 = (adc1 * 0.1875)/1000;
  Voltage1 = Voltage1*100;
  Amp1 = map(Voltage1, 0, 500, 0, 2000);

  adc2 = ads.readADC_SingleEnded(2);
  Voltage2 = (adc2 * 0.1875)/1000;
  Voltage2 = Voltage2*100;
  Amp2 = map(Voltage2, 0, 500, 0, 2000);

  adc3 = ads.readADC_SingleEnded(3);
  Voltage3 = (adc3 * 0.1875)/1000;
  Voltage3 = Voltage3*100;
  Amp3 = map(Voltage3, 0, 500, 0, 2000);
  
   value = analogRead(analogInput);
   vout = (value * 5.0) / 1024.0; // see text
   vin = vout / (R2/(R1+R2)); 
   if (vin<0.09) {
   vin=0.0;
   }

  //Serial.print("AIN0: "); 
  //Serial.print(Voltage0);
  //Serial.print(adc0);
  //Serial.print("\tVoltage0: ");
  Serial.print(Amp0/100, 7);  
  Serial.print("A");
  mySerial.print(Amp0/100, 7);  
  mySerial.print("A");
  
  //Serial.print("AIN1: "); 
  //Serial.print(Voltage1);
  //Serial.print(adc1);
  //Serial.print("\tVoltage1: ");
  Serial.print(Amp1/100, 7);  
  Serial.print("B");
  mySerial.print(Amp1/100, 7);
  mySerial.print("B");
  
  //Serial.print("AIN2: "); 
  //Serial.print(Voltage2);
  //Serial.print(adc2);
  //Serial.print("\tVoltage2: ");
  Serial.print(Amp2/100, 7);  
  Serial.print("C");
  mySerial.print(Amp2/100, 7);  
  mySerial.print("C");
  
  //Serial.print("AIN3: "); 
  //Serial.print(Voltage3);
  //Serial.print(adc3);
  //Serial.print("\tVoltage3: ");
  Serial.print(Amp3/100, 7);  
  Serial.print("D");
  mySerial.print(Amp3/100, 7); 
  mySerial.print("D");

  Serial.print(vin);  
  Serial.println("F");
  mySerial.print(vin); 
  mySerial.println("F");
    
  lcd.home (); // Set cursor to 0,0
  lcd.print("I1:");
  lcd.print(Amp0/100,2); // Custom text
  lcd.setCursor (8, 0); // Go to home of 2nd line
  lcd.print("I2:");
  lcd.print(Amp1/100,2); // Custom text
  lcd.setCursor (0, 1); // Go to home of 2nd line
  lcd.print("I3:");
  lcd.print(Amp2/100,2); // Custom text
  lcd.setCursor (8, 1); // Go to home of 2nd line
  lcd.print("I4:");
  lcd.print(Amp3/100,2); // Custom text
  
  delay(1000);
}

