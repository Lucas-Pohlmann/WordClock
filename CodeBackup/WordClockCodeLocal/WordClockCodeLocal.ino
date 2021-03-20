#include <Adafruit_NeoPixel.h>
#include <DS3231.h>
#include <SoftwareSerial.h>

Adafruit_NeoPixel L1 = Adafruit_NeoPixel(22, 9, NEO_GRB + NEO_KHZ800);				//Die einzelnen Neopixel Lines
Adafruit_NeoPixel L2 = Adafruit_NeoPixel(22, 8, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel L3 = Adafruit_NeoPixel(22, 7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel L4 = Adafruit_NeoPixel(22, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel L5 = Adafruit_NeoPixel(22, 5, NEO_GRB + NEO_KHZ800);

//RealTimeClock Variables
DS3231 RTC(A4, A5);
Time RTCtime;
//RTC Functions
void ResetTime(bool Reset, int Hour, int Minute, int Second, int Day, int Month, int Year, int DayName /* MONDAY == 1, TUESDAY == 2, usw...*/ ) {
	if (Reset) {
		RTC.setDOW(DayName);
		RTC.setTime(Hour, Minute, Second);
		RTC.setDate(Day, Month, Year);
		Serial.println("resetted the RTC");
	}
}
void ResetHour(bool Reset, int Hour) { 
	
	if (Reset) {
		RTC.setTime(Hour, RTCtime.min, RTCtime.sec);
		Serial.println("resetted the RTC Hours");
	}
}
void OutputTime(SoftwareSerial output){ //Outputs the Time to the HC-06 Module
//Wiedergabe der Zeit im Serial Output
  RTCtime = RTC.getTime();
//Stunden ausgeben
    if (RTCtime.hour < 10){
      output.print("0");
    }
    output.print(RTCtime.hour);
//Minuten ausgeben
    if (RTCtime.min < 10){
      output.print(":0");
    }else{
      output.print(":");
    }
    output.print(RTCtime.min);
//Sekunden ausgeben
  if (RTCtime.sec < 10){
    output.print(".0");
  }else{
    output.print(".");
  }
  output.print(RTCtime.sec);
  output.print("  ");
//Wochentag ausgeben
  switch(RTCtime.dow){
    case 1:
      output.print("Monday");
      break;
    case 2:
      output.print("Tuesday");
      break;
    case 3:
      output.print("Wednesday");
      break;
    case 4:
      output.print("Thursday");
      break;
    case 5:
      output.print("Friday");
      break;
    case 6:
      output.print("Saturday");
      break;
    case 7:
      output.print("Sunday");
      break;
  }
  output.print(" ");
//Datum ausgegeben
  if (RTCtime.date < 10){
      output.print("0");
    }
    output.print(RTCtime.date);
//Monat ausgeben
  if (RTCtime.mon < 10){
      output.print(".0");
    }else{
      output.print(".");
    }
    output.print(RTCtime.mon);
//Jahr ausgeben
  output.print(".");
  output.println(RTCtime.year);
}

//HC06 Bluetooth Module
SoftwareSerial HC06(10, 11);	//TX RX (BEIM HC-06 MODUL)
bool HC06active = false;
char string[32];
char byteRead;
String oldstringInput;
String stringInput;
//HC-06 Functions
void getInput() {
    //if (Serial.available()) {
    oldstringInput = stringInput;
        while (HC06.available()) {
            int availableBytes = HC06.available();
            for (int i = 0; i < availableBytes; i++)
            {
                string[i] = HC06.read();
                string[i + 1] = '\0'; // Append a null
                stringInput = string;
            }
        }
        
    //}
}



void setup()
{
//SerialMonitor Setup
	Serial.begin(9600);

//HC-06 Setup
	HC06.begin(9600);

//RTC Setup
	RTC.begin();
	RTCtime = RTC.getTime();
	ResetTime(false, 17, 05, 20, 17, 12, 2020, 4);			//Daten Eingeben und auf true stellen um die Uhr zu stellen
	ResetHour(false, 17);									              //Stunden eingeben und fertig!

}


void loop()
{
  getInput();
  if (oldstringInput != stringInput) {
    //getting Time on mobile
        if (stringInput == "getTime") {
          OutputTime(HC06);
        }
    //changing Time via mobile
        if(stringInput == "setTime"){
          HC06.println("Hour:");
          int i = 0;
          oldstringInput = stringInput;
          while(oldstringInput == stringInput && i < 100){
            Serial.println(i);
            getInput();
            delay(100);
            i++;
          }
          Serial.println(stringInput.toInt());
          if(i == 100){
            HC06.println("took to long");
            i = 0;
            return;
          }else if(stringInput.toInt() == NULL){
            HC06.println("not a Number!");
            return;
          }else if(stringInput.toInt() > 24 || stringInput.toInt() < 0){
            HC06.println("Number invalid");
            return;
          }
        }
    }
  delay(200);
}
