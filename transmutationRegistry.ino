/*

  Project Title: transmutationRegistry
  Description: Software companion to “Neogenesis Scanner”. NHH, “New Human Health” 2022.

  ©Leandro Niero 2022
  leandroniero@mail.com
  https://leandroniero.myportfolio.com/transmutationregistry


  Fire2012 by Mark Kriegsman, July 2012.
  As part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY

*/


#include <CapacitiveSensor.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 100

#define DATA_PIN 3
#define CLOCK_PIN 13
#define COLOR_ORDER GRB
#define BRIGHTNESS  255


#define SPARKING 100
#define COOLING  60
bool gReverseDirection = true;

CRGB leds[NUM_LEDS];


CapacitiveSensor   cs_4_2 = CapacitiveSensor(4, 2);
CapacitiveSensor   cs_7_8 = CapacitiveSensor(7, 8);
CapacitiveSensor   cs_12_13 = CapacitiveSensor(12, 13);

void setup() {

  Serial.begin(9600);
  delay(5000);

  Serial.write("0");


  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

}

//LED
void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(random(200, 250));
  }
}


void loop() {



  //long start = millis();
  long total1 =  cs_4_2.capacitiveSensor(30);
  long total2 =  cs_7_8.capacitiveSensor(30);
  long total3 =  cs_12_13.capacitiveSensor(30);


  Serial.write(total1);
  //
  // Serial.println(total1);
  //  Serial.print(total1);
  //  Serial.print("\t");
  //  Serial.print(total2);
  //  Serial.print("\t");
  //  Serial.println(total3);

  delay(10);  // arbitrary delay to limit data to serial port



  //LED
  static uint8_t hue = 0;


  if  ( total2 > 15 || total3 > 15 ) {


    for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
      // Set the led colour
      leds[i] = CRGB(120, 255, 20);
      // Show the leds
      FastLED.show();
      // reset to black
      leds[i] = CRGB(255, 0, 0);
      fadeall();

    }

    for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
      // Set the led colour
      leds[i] = CRGB(random(255), random(255), 0);
      // Show the leds
      FastLED.show();
      // reset to black
      leds[i] = CRGB(random(255), random(255), 0);
      fadeall();

    }

    for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
      leds[i] = CRGB(255, 0, 0);
      // Show the leds
      FastLED.show();
      // reset to black
      leds[i] = CRGB(120, 255, 20);
      fadeall();

    }
  } else {
    random16_add_entropy( random());
    Fire2012();
    FastLED.show(); // display this frame

  }
}

void Fire2012()
{
  // Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    CRGB color = HeatColor( heat[j]);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}
