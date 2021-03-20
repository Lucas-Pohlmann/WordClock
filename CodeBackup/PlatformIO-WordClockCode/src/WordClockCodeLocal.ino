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
      //change hours
          HC06.println("hour:");
        //wait for input
          int h = 0;
          oldstringInput = stringInput;
          while(oldstringInput == stringInput && h < 100){
            Serial.println(h);
            getInput();
            delay(100);
            h++;
          }
          Serial.println(stringInput.toInt());
        //check if a valid number or no number
          if(h == 100){
            HC06.println("took to long");
            h = 0;
            return;
          }else if(stringInput.toInt() == NULL){
            HC06.println("not a number!");
            return;
          }else if(stringInput.toInt() > 24 || stringInput.toInt() < 0){
            HC06.println("number invalid");
            return;
        //if valid number set the hour
          }else{
            int hour = stringInput.toInt();
            HC06.print("setting hour to: ");
            HC06.println(hour);
            RTC.setTime(hour, RTCtime.min, RTCtime.sec);
            RTCtime = RTC.getTime();
          }
      //change minutes
          delay(500);
          HC06.println("minutes:");
        //wait for input
          int m = 0;
          oldstringInput = stringInput;
          while(oldstringInput == stringInput && m < 100){
            Serial.println(m);
            getInput();
            delay(100);
            m++;
          }
          Serial.println(stringInput.toInt());
        //check if a valid number or no number
          if(m == 100){
            HC06.println("took to long");
            m = 0;
            return;
          }else if(stringInput.toInt() == NULL){
            HC06.println("not a number!");
            return;
          }else if(stringInput.toInt() > 60 || stringInput.toInt() < 0){
            HC06.println("number invalid");
            return;
        //if valid number set the hour
          }else{
            int minutes = stringInput.toInt();
            HC06.print("Setting minutes to: ");
            HC06.println(minutes);
            RTC.setTime(RTCtime.hour, minutes, RTCtime.sec);
            RTCtime = RTC.getTime();
          }
      //change seconds
          delay(500);
          HC06.println("seconds:");
        //wait for input
          int s = 0;
          oldstringInput = stringInput;
          while(oldstringInput == stringInput && s < 100){
            Serial.println(s);
            getInput();
            delay(100);
            s++;
          }
          Serial.println(stringInput.toInt());
        //check if a valid number or no number
          if(s == 100){
            HC06.println("took to long");
            s = 0;
            return;
          }else if(stringInput.toInt() == NULL){
            HC06.println("not a number!");
            return;
          }else if(stringInput.toInt() > 60 || stringInput.toInt() < 0){
            HC06.println("number invalid");
            return;
        //if valid number set the hour
          }else{
            int seconds = stringInput.toInt();
            HC06.print("Setting seconds to: ");
            HC06.println(seconds);
            RTC.setTime(RTCtime.hour, RTCtime.min, seconds);
            RTCtime = RTC.getTime();
          }
      //show time you set
          delay(500);
          HC06.println("Changed Time to: ");
          OutputTime(HC06);
        }
    //change DOW via mobile
        if(stringInput == "setDOW"){
          stringInput = "Marek stinkt";
          HC06.println("dow (MONDAY = 1 etc.):");
        //wait for input
          int dow = 0;
          oldstringInput = stringInput;
          while(oldstringInput == stringInput && dow < 100){
            Serial.println(dow);
            getInput();
            delay(100);
            dow++;
          }
          Serial.println(stringInput.toInt());
        //check if a valid number or no number
          if(dow == 100){
            HC06.println("took to long");
            dow = 0;
            return;
          }else if(stringInput.toInt() == NULL){
            HC06.println("not a number!");
            return;
          }else if(stringInput.toInt() > 7 || stringInput.toInt() < 0){
            HC06.println("number invalid");
            return;
          }
        //if valid number set the dow
          else{
            int date = stringInput.toInt();
            HC06.print("setting date to: ");
            RTC.setDOW(date);
            RTCtime = RTC.getTime();
          //convert day of week number to text
            switch (RTCtime.dow)
            {
            case 1:
              HC06.println("Monday");
              break;
            case 2:
              HC06.println("Tuesday");
              break;
            case 3:
              HC06.println("Wednesday");
              break;
            case 4:
              HC06.println("Thursday");
              break;
            case 5:
              HC06.println("Friday");
              break;
            case 6:
              HC06.println("Saturday");
              break;
            case 7:
              HC06.println("Sunday");
              break;
            }
          }
        }
    //change date via mobile
        if(stringInput == "setDate"){
      //change day
          HC06.println("day:");
        //wait for input
          int d = 0;
          oldstringInput = stringInput;
          while(oldstringInput == stringInput && d < 100){
            Serial.println(d);
            getInput();
            delay(100);
            d++;
          }
          Serial.println(stringInput.toInt());
        //check if a valid number or no number
          if(d == 100){
            HC06.println("took to long");
            d = 0;
            return;
          }else if(stringInput.toInt() == NULL){
            HC06.println("not a number!");
            return;
          }else if(stringInput.toInt() > 31 || stringInput.toInt() < 0){
            HC06.println("number invalid");
            return;
          }
        //if valid number set the day
          else{
            int day = stringInput.toInt();
            HC06.print("setting date to: ");
            HC06.println(day);
            RTC.setDate(day, RTCtime.mon, RTCtime.year);
            RTCtime = RTC.getTime();
          }
      //change month
          delay(500);
          HC06.println("month:");
        //wait for input
          int mon = 0;
          oldstringInput = stringInput;
          while(oldstringInput == stringInput && mon < 100){
            Serial.println(mon);
            getInput();
            delay(100);
            mon++;
          }
          Serial.println(stringInput.toInt());
        //check if a valid number or no number
          if(mon == 100){
            HC06.println("took to long");
            mon = 0;
            return;
          }else if(stringInput.toInt() == NULL){
            HC06.println("not a number!");
            return;
          }else if(stringInput.toInt() > 12 || stringInput.toInt() < 0){
            HC06.println("number invalid");
            return;
          }
        //if valid number set the month
          else{
            int month = stringInput.toInt();
            HC06.print("Setting month to: ");
            HC06.println(month);
            RTC.setDate(RTCtime.date, month, RTCtime.year);
            RTCtime = RTC.getTime();
          }
      //change year
          delay(500);
          HC06.println("year:");
        //wait for input
          int y = 0;
          oldstringInput = stringInput;
          while(oldstringInput == stringInput && y < 100){
            Serial.println(y);
            getInput();
            delay(100);
            y++;
          }
          Serial.println(stringInput.toInt());
        //check if a valid number or no number
          if(y == 100){
            HC06.println("took to long");
            y = 0;
            return;
          }else if(stringInput.toInt() == NULL){
            HC06.println("not a number!");
            return;
          }
        //if valid number set the hour
          else{
            int year = stringInput.toInt();
            HC06.print("Setting year to: ");
            HC06.println(year);
            RTC.setTime(RTCtime.hour, RTCtime.mon, year);
            RTCtime = RTC.getTime();
          }
      //show time you set
          delay(500);
          HC06.println("Changed Date to: ");
          OutputTime(HC06);
      }
    }//Ende of HC06 Commands loop
  delay(200);
}