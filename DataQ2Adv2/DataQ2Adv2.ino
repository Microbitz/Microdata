#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#define DS1307_ADDRESS 0x68
#define I2C_ADDRESS 0x3C
SSD1306AsciiAvrI2c oled;

Adafruit_ADS1115 ads(0x48);
SoftwareSerial mySerial(3, 2); // RX, TX

byte zero = 0x00;
const int chipSelect = 4;
float Voltage0 = 0.0;
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
int counter0 = 0;
int counter1 = 0;
int counter2 = 0;
int counter3 = 0;
String cadena;
String cadena1;
String cadena2;
String cadena3;
String cadena4;
String cadena5;
String cadena6;
String msg;
bool transmit = true;
bool HC = false;
bool HCOK = false;
bool FC = false;
bool FCOK = false;
int second;
int minute;
int hour;
int weekDay;
int monthDay;
int month;
int year;

int setsecond;
int setminute;
int sethour;
int setweekDay;
int setmonthDay;
int setmonth;
int setyear;

byte setsecond0;
byte setminute0;
byte sethour0;
byte setweekDay0;
byte setmonthDay0;
byte setmonth0;
byte setyear0;

File dataFile;

void setup(void)
{
  Wire.begin();
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(Arial_bold_14);
  oled.clear();
  //oled.println("MTDataAq2");
  Serial.begin(115200);
  mySerial.begin(9600);
  ads.begin();
  oled.setCursor(0, 0 );
  oled.println("Signals (mA)");
  oled.print("S1>");
  oled.print(Amp0 / 100, 2);
  oled.print("   ");
  oled.print("S2>");
  oled.print(Amp1 / 100, 2);
  oled.println(" ");
  oled.print("S3>");
  oled.print(Amp2 / 100, 2);
  oled.print("   ");
  oled.print("S4>");
  oled.print(Amp3 / 100, 2);
  oled.println(" ");
  //setDateTime(); //MUST CONFIGURE IN FUNCTION
  pinMode(SS, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("CF");
    while (1) ;
  }
  //Serial.println("card initialized.");
  dataFile = SD.open("MTdl.txt", FILE_WRITE);
  if (! dataFile) {
    Serial.println("EO");
    while (1) ;
  }
}

void loop(void)
{

  //----------------------------------Comandos de entrada-----------------------------

  char SerialInByte;
  if (Serial.available())
  {
    if ( msg.indexOf( "X" ) >= 0 ) {
      msg = "";
    }
    SerialInByte = (unsigned char)Serial.read();
    msg += String(SerialInByte);
  }

  //---------------------------------------------------------------
  //----------------------------------Analisis de entrada Hora----------
  if (HC == true) {
    cadena = msg;
    cadena1 = cadena.substring(0, cadena.indexOf(':'));
    cadena2 = cadena.substring(cadena.indexOf(':') + 1, cadena.indexOf('M'));

    if ( cadena.indexOf( "M" ) >= 0 ) {
      Serial.print(cadena1 + ":");
      Serial.println(cadena2);
      Serial.println("OK?");
      cadena = "";
      msg = "";
      HCOK = true;
      HC = false;
      FC = true;
      setsecond = 0;
      setminute = cadena2.toInt();
      sethour = cadena1.toInt();
    }
  }
  if (FC == true) {
    cadena = msg;
    cadena4 = cadena.substring(0, cadena.indexOf('D'));
    cadena5 = cadena.substring(cadena.indexOf('D') + 1, cadena.indexOf('M'));
    cadena6 = cadena.substring(cadena.indexOf('M') + 1, cadena.indexOf('A'));

    if ( cadena.indexOf( "A" ) >= 0 ) {
      Serial.print(cadena1 + ":");
      Serial.print(cadena2 + " ");
      Serial.print(cadena4 + "/");
      Serial.print(cadena5 + "/");
      Serial.println(cadena6);
      Serial.println("OK?");
      cadena = "";
      msg = "";
      FCOK = true;
      FC = true;
      setsecond = 0;
      setmonthDay = cadena4.toInt();
      setmonth = cadena5.toInt();
      setyear = cadena6.toInt();
    }

  }
  //---------------------------------------------------------------
  //----------------------------------Comandos de consola----------

  if ( msg.indexOf( "HC" ) >= 0 ) {
    Serial.println("Calib. Hora");
    msg = "";
    transmit = false;
    HC = true;
    HCOK = false;
  }

  if ( msg.indexOf( "ONT" ) >= 0 ) {
    Serial.println("OK");
    msg = "";
    transmit = true;
    HC = false;
    HCOK = false;
  }

  if ( msg.indexOf( "OFT" ) >= 0 ) {
    Serial.println("OK");
    msg = "";
    transmit = false;
    HC = false;
    HCOK = false;
  }
  if ( msg.indexOf( "OKH" ) >= 0 ) {
    if ( FCOK == true ) {
      setDateTime();
      Serial.println("OK");
      msg = "";
      HC = false;
      HCOK = false;
      FC = false;
      FCOK = false;
      transmit = true;
    }
  }
  //---------------------------------------------------------------
  //----------------------------------Lectura y manejop de señales-
  int16_t adc0;  // we read from the ADC, we have a sixteen bit integer as a result

  adc0 = ads.readADC_SingleEnded(0);
  Voltage0 = ((adc0 * 0.1875) / 1000) * 1000;
  Amp0 = map(Voltage0, 0, 5000, 0, 20000);
  
  adc0 = ads.readADC_SingleEnded(1);
  Voltage0 = ((adc0 * 0.1875) / 1000) * 1000;
  Amp1 = map(Voltage0, 0, 5000, 0, 20000);

  adc0 = ads.readADC_SingleEnded(2);
  Voltage0 = ((adc0 * 0.1875) / 1000) * 1000;
  Amp2 = map(Voltage0, 0, 5000, 0, 20000);

  adc0 = ads.readADC_SingleEnded(3);
  Voltage0 = ((adc0 * 0.1875) / 1000) * 1000;
  Amp3 = map(Voltage0, 0, 5000, 0, 20000);

  //---------------------------------------------------------------
  //----------------------------------Bluetooth--------------------

  mySerial.print("A");
  mySerial.print((Amp0 / 100) + 50, 2);
  mySerial.println();

  mySerial.print("B");
  mySerial.print((Amp1 / 100) + 50, 2);
  mySerial.println();

  mySerial.print("C");
  mySerial.print((Amp2 / 100) + 50, 2);
  mySerial.println();

  mySerial.print("D");
  mySerial.print((Amp3 / 100) + 50, 2);
  mySerial.println();

  mySerial.print(vin);
  mySerial.println("F");

  //---------------------------------------------------------------
  //----------------------------------Simulacion-------------------

  /*counter0 = counter0 + 5;
    counter1 = counter0 / 2;
    counter2 = counter0 * 3;
    counter3 = counter0 / 5 + 1;*/

  //---------------------------------------------------------------
  //----------------------------------Transmision para el software-
  if (transmit == true) {
    char text00[40];
    sprintf(text00, "%d,%d,%d,%d\n", Amp0 , Amp1, Amp2 , Amp3);
    Serial.println(text00);
  }

  if (transmit == false) {
  }
  //---------------------------------------------------------------
  //----------------------------------Pantalla---------------------

  oled.setCursor(0, 0 );
  oled.println("Signals (mA)");
  oled.print("S1>");
  oled.print(Amp0 / 1000, 2);
  oled.print("   ");
  oled.print("S2>");
  oled.println(Amp1 / 1000, 2);
  oled.print("S3>");
  oled.print(Amp2 / 1000, 2);
  oled.print("   ");
  oled.print("S4>");
  oled.println(Amp3 / 1000, 2);
  delay(100);

  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, 7);

  second = bcdToDec(Wire.read());
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  monthDay = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());

  //print the date EG   3/1/11 23:59:59
  oled.print(monthDay);
  oled.print("/");
  oled.print(month);
  oled.print("/");
  oled.print(year);
  oled.print(" ");
  oled.print(hour);
  oled.print(":");
  if(minute < 10){
  oled.print("0"); 
  }
  oled.print(minute);
  oled.print(" ");
  //oled.print(":");
  //oled.println(second);

  //---------------------------------------------------------------
  //----------------------------------MicroSD----------------------

  String dataString = "";
  dataString += String(Amp0 / 100, 7);
  dataString += ",";
  dataString += String(Amp1 / 100, 7);
  dataString += ",";
  dataString += String(Amp2 / 100, 7);
  dataString += ",";
  dataString += String(Amp3 / 100, 7);
  dataString += ",";
  dataString += String(month);
  dataString += "/";
  dataString += String(monthDay);
  dataString += "/";
  dataString += String(year);
  dataString += ",";
  dataString += String(hour);
  dataString += ":";
  dataString += String(minute);
  dataString += ":";
  dataString += String(second);


  dataFile.println(dataString);
  dataFile.flush();
}

//---------------------------------------------------------------
//----------------------------------Reloj---------------------

void setDateTime() {

  byte second =      setsecond; //0-59
  byte minute =      setminute; //0-59
  byte hour =        sethour; //0-23
  byte weekDay =     5; //1-7
  byte monthDay =    setmonthDay; //1-31
  byte month =       setmonth; //1-12
  byte year  =       setyear; //0-99

  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);

  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));

  Wire.write(zero); //start

  Wire.endTransmission();

}

byte decToBcd(byte val) {
  // Convert normal decimal numbers to binary coded decimal
  return ( (val / 10 * 16) + (val % 10) );
}

byte bcdToDec(byte val)  {
  // Convert binary coded decimal to normal decimal numbers
  return ( (val / 16 * 10) + (val % 16) );
}

