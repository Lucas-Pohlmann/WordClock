#include <Adafruit_NeoPixel.h>
#include <DS3231.h>

Adafruit_NeoPixel reihe1 = Adafruit_NeoPixel(22, 9, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel reihe2 = Adafruit_NeoPixel(22, 8, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel reihe3 = Adafruit_NeoPixel(22, 7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel reihe4 = Adafruit_NeoPixel(22, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel reihe5 = Adafruit_NeoPixel(22, 5, NEO_GRB + NEO_KHZ800);


DS3231 rtc(A4, A5);
Time t;
int z = 0;
int h = 0;
int a = 0;
int d = 0;
int r = 0;
int g = 0;
int b = 0;
int c = 0;
int reset = 0;
int EINER = 0;
bool inUHR = false;
int poti = analogRead(A0);
long white = 0;
long nichts = 0;
bool sommer = true;


void setup() {
  r = 255;
  g = 255;
  b = 255;
  

 nichts = reihe1.Color(0, 0, 0);
 white = reihe1.Color(r, g, b);
 //blau = reihe2.Color(0, 0, 255);
 rtc.begin();
 reihe1.begin();
 reihe2.begin();
 reihe3.begin();
 reihe4.begin();
 reihe5.begin();
 Serial.begin(9600);

}
void(* resetFunc) (void) = 0;


void ALLE(){
  reihe1.fill(white, 0, 22);
  reihe2.fill(white, 0, 22);
  reihe3.fill(white, 0, 22);
  reihe4.fill(white, 0, 22);
  reihe5.fill(white, 0, 22);
  
}

void ES(){
  reihe1.fill(white, 20, 2);
}

void IST(){
  reihe1.fill(white, 16, 3);
}

void FUENF(){
  reihe1.fill(white, 11, 4);
}

void ZEHN(){
  reihe1.fill(white, 0, 4);
}

void ZWANZIG(){
  reihe1.fill(white, 4, 7);
}

void DREI(){
  reihe2.fill(white, 18, 4);
}

void VIERTEL(){
  reihe2.fill(white, 11, 7);
}

void NACH(){
  reihe2.fill(white, 2, 4);
}

void VOR(){
  reihe2.fill(white, 6, 3);
}

void HALB(){
  reihe3.fill(white, 18, 4);
}

void zwoelf(){
  reihe3.fill(white, 12, 5);
}

void zwei(){
  reihe3.fill(white, 0, 4);
}

void eins(){
  if(EINER == 2){
    reihe3.fill(white, 2, 3);
  }else{
    reihe3.fill(white, 2, 4);
  }
}

void sieben(){
  reihe3.fill(white, 5, 6);
}


void drei(){
  reihe4.fill(white, 17, 4);
}


void fuenf(){
  reihe4.fill(white, 11, 4);
}


void elf(){
  reihe4.fill(white, 0, 3);
}


void neun(){
  reihe4.fill(white, 3, 4);
}


void vier(){
  reihe4.fill(white, 7, 4);
}


void acht(){
  reihe5.fill(white, 17, 4);
}


void zehn(){
  reihe5.fill(white, 13, 4);
}



void sechs(){
  reihe5.fill(white, 1, 5);
}


void uhr(){
  reihe5.fill(white, 8, 3);
}




void CLEAR(){
  reihe1.clear();
  reihe2.clear();
  reihe3.clear();
  reihe4.clear();
  reihe5.clear();
}

void SHOW(){
  reihe1.show();
  reihe2.show();
  reihe3.show();
  reihe4.show();
  reihe5.show();
}

void BRIGHTNESS(){
  reihe1.setBrightness(poti);
  reihe2.setBrightness(poti);
  reihe3.setBrightness(poti);
  reihe4.setBrightness(poti);
  reihe5.setBrightness(poti);
  
}









void loop() {

  CLEAR();
  EINER = 0;
  t = rtc.getTime();
  z = t.min;
  if(sommer == true){
    h = t.hour;
  }else{
    h = t.hour + 1;
  }
  if(h >= 12){
    h = h - 12;
  }
  a = h;

  Serial.print(r);
  Serial.print("  ");
  Serial.print(g);
  Serial.print("  ");
  Serial.print(b);
  Serial.print("  ");
  Serial.print(c);
  Serial.print("  ");
  Serial.print("MIN: ");
  Serial.print(z);
  Serial.print("/");
  d = z / 5;
  Serial.print(d);
  Serial.print("    HOUR:");
  Serial.print(h);
  Serial.print("   Poti:");
  poti = analogRead (A0);
  poti = poti/4;
  Serial.print(poti);
  Serial.print("  Farbe:");
  Serial.print(white);
  delay(50);
  ES();
  IST();

  
switch (d) {
  case 0:
    EINER = EINER + 1;
    inUHR = true;
    break;

  case 1:
    FUENF();
    NACH();
    break;

  case 2:
    ZEHN();
    NACH();
    break;

  case 3:
    VIERTEL();
    NACH();
    break;

  case 4:
    ZWANZIG();
    NACH();
    break;

  case 5:
    FUENF();
    VOR();
    HALB();
    a = a + 1;
    break;

  case 6:
    HALB();
    a = a + 1;
    break;

  case 7:
    FUENF();
    NACH();
    HALB();
    a = a + 1;
    break;

  case 8:
    ZWANZIG();
    VOR();
    a = a + 1;
    break;

  case 9:
    VIERTEL();
    VOR();
    a = a + 1;
    break;

  case 10:
    ZEHN();
    VOR();
    a = a + 1;
    break;

  case 11:
    FUENF();
    VOR();
    a = a + 1;
    break;

  case 12:
    EINER = EINER + 1;
    inUHR = true;
    break;
}

if(a >= 12){
    a = a - 12;
  }

switch (a) {
  case 0:
    zwoelf();
    break;

  case 1:
    EINER = EINER + 1;
    eins();
    break;

  case 2:
    zwei();
    break;

  case 3:
    drei();
    break;

  case 4:
    vier();
    break;

  case 5:
    fuenf();
    break;

  case 6:
    sechs();
    break;

  case 7:
    sieben();
    break;
    
  case 8:
    acht();
    break;

  case 9:
    neun();
    break;

  case 10:
    zehn();
    break;

  case 11:
    elf();
    break;

  case 12:
    zwoelf();
    break;


}

  if(inUHR == true){
    uhr();
  }
  inUHR = false;
  Serial.print("  a = ");
  Serial.println(a);
  reset = reset + 1;
  Serial.print(reset);
  Serial.print("   EINS?:  ");
  Serial.println(EINER);
  poti = 255;
  BRIGHTNESS();
  SHOW();
  delay(100);
  //reset = reset + 1;
  /*if (reset >= 100){
    reset = 0;
    Serial.println("resetting");
    Serial.println("resetting");
    Serial.println("resetting");
    Serial.println("resetting");
    resetFunc();
    
  }else{
  
  }
  */


  

}
