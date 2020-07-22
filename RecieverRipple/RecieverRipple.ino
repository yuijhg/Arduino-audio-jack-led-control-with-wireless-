#include <SPI.h>  
#include "RF24.h" 
#include <FastLED.h>
#define NUM_LEDS 15
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

  
int  sensorValue =  0;  
int check = 0; 
uint8_t myfade = 255;
RF24 myRadio (10, 9);
uint8_t colour;                                               
int center = 7;                                               
int step = -1; 
#define maxsteps 16 
#define LED_TYPE WS2812


struct package
{
  int sample_peak=0;
};

byte addresses[][6] = {"0"}; 

typedef struct package Package;
Package data;

void setup() 
{
  Serial.begin(115200);
  delay(1000);

  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();

  LEDS.addLeds<LED_TYPE, DATA_PIN, RGB>(leds, NUM_LEDS);  
  FastLED.setBrightness(255);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
}


void loop()  
{

  if ( myRadio.available()) 
  {
    while (myRadio.available())
    {
      myRadio.read( &data, sizeof(data) );
    }
    Serial.print("\nPackage:");
    Serial.print("\n");
    Serial.print("Data:");
    Serial.print(data.sample_peak);
    Serial.print("\n"); 
    Serial.print("\n");
    
   ripple();
   FastLED.show();
} 
  }

void ripple() {

  if (check != data.sample_peak) {step = -1;}
  fadeToBlackBy(leds, NUM_LEDS, 64);                          // 8 bit, 1 = slow, 255 = fast
  
  switch (step) {

    case -1:                                                  
      center = 7;
      colour = random8();                                     
      step = 0;
      Serial.print("\n-1");
      break;

    case 0:
        for (int i = 8; i > 14; i++) {       
        leds[i] = leds[i - 1];
        }

       for (int i = 6; i > 0; i--) {                  
       leds[i] = leds[i + 1];
       }                  
      step ++;
      Serial.print("\0");
      break;

    case maxsteps:                                            
      // step = -1;
      Serial.print("\nMAX");
      break;

    default:                                                  
      leds[(center + step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade/step*2);       
      leds[(center - step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade/step*2);
      step ++;                                                
      break;  
  } 
  check = data.sample_peak;
} 
