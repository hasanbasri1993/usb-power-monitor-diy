// Nokia 5110 LCD-Display (84x48 Bildpunkte)

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SimpleTimer.h>

// D7 - Serial clock out (CLK oder SCLK)
// D6 - Serial data out (DIN)
// D5 - Data/Command select (DC oder D/C)
// D4 - LCD chip select (CE oder CS)
// D3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

int currentinput = A1;
int voltageinput = A0;
float sample2;
float actualval ;
float val;
float amps;
float vout = 0.0;
float vin ;
float R1 = 10000.0; //
float R2 = 10000.0; //
int value = 0;
// the timer object
SimpleTimer timer;

void setup()   {

  // Display initialisieren
  display.begin();
  // Kontrast setzen
  display.setContrast(55);
  display.clearDisplay();   // clears the screen and buffer
  timer.setInterval(1200, textscroll);
  timer.setInterval(1200, powermonitor);

}


void loop() {
  timer.run();

  display.clearDisplay();      // Display wieder löschen
}

void set_text(int x, int y, String text, int color) {

  display.setTextColor(color); // Textfarbe setzen, also Schwarz oder Weiss
  display.setCursor(x, y);     // Startpunkt-Position des Textes
  display.println(text);       // Textzeile ausgeben
  display.display();           // Display aktualisieren
}

void textscroll() {

  display.setTextSize(1);
  set_text(11, 0, "USB Monitor", BLACK);
  display.setTextSize(1);
  set_text(2, 40, "hasanbasri.co", BLACK);
  display.setTextSize(1);
  set_text(35, 12, "V", BLACK);
  display.setTextSize(1);
  set_text(80, 12, "V", BLACK);
  display.drawLine(7, 23, 77, 23, BLACK);
  display.display();
  display.drawLine(42, 10, 42, 36, BLACK);
  display.display();

  /*/ Ein kleines bisschen Scroll-Text-Magie
    int x=0;
    for(int i=0;i<(5.6*8);i++){
    set_text(x,40,"hasanbasri.co",BLACK);
    delay(i==0?1000:100);
    if(i<(5.6*8)-1)set_text(x,40,"hasanbasri.co",WHITE);
    if((i)<(2.74*8))x-=1;else x+=1;
    }*/
}
void powermonitor () {
  value = analogRead(voltageinput);
  vout = (value * 5.0) / 1024.0; // see text
  vin = vout / (R2 / (R1 + R2));

  for (int i = 0; i < 150; i++)
  {
    sample2 += analogRead(currentinput); //read the current from sensor
    delay(2);
  }
  long milisec = millis(); // calculate time in milliseconds
  long time = milisec / 1000; // convert milliseconds to seconds
  sample2 = sample2 / 150;
  val = (5.0 * sample2) / 1024.0;
  actualval = val - 2.5; // offset voltage is 2.5v
  amps = actualval * 10;
  float mA = amps * 1000 ;
  float totamps = totamps + mA; // calculate total amps
  float avgamps = totamps / time; // average amps
  float amphr = (avgamps * time) / 3600; // amp-hour


  display.setTextColor(BLACK); // Textfarbe setzen, also Schwarz oder Weiss
  display.setCursor(9, 12);     // Startpunkt-Position des Textes
  display.println(vin);       // Textzeile ausgeben
  display.display();           // Display aktualisieren

  display.setTextColor(BLACK); // Textfarbe setzen, also Schwarz oder Weiss
  display.setCursor(45, 12);     // Startpunkt-Position des Textes
  display.println(mA);       // Textzeile ausgeben
  display.display();           // Display aktualisieren
}
