#include <FastLED.h>                                          
#define LED_DT 3                                             
                                         
#define COLOR_ORDER GRB                                       
#define LED_TYPE WS2812                                       
#define NUM_LEDS 15                                       

#define MIC_PIN   A0
#define ref A2
uint8_t squelch = 340;                                          
int sample;                                                   
float sampleAvg = 0;                                          
float micLev = 0;                                             
uint8_t maxVol = 400;                                          
bool samplePeak = 0;                                          

struct CRGB leds[NUM_LEDS];                                   

int max_bright = 255;
                                           

#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (7, 8);
byte addresses[][6] = {"0"};

struct package
{
  int sample_peak = sample;
};

typedef struct package Package;
Package data;

void setup() {
 
  Serial.begin(115200);

  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  

  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  delay(1000);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  delay(1000);
} 

void loop() {

  getSample();

  EVERY_N_MILLISECONDS(25) {
   ripple();
  }

   FastLED.show();
} 

void getSample() {
  
  int16_t micIn;                                              
  static long peakTime;
   
  micIn = analogRead(MIC_PIN);
  micIn = micIn - 420;                                          
  if (micIn < 375)
  sample = 0;
  else
  sample = (sample + micIn) / 2;
  sampleAvg = ((sampleAvg * 31) + sample) / 32;               
  if (sample > (sampleAvg+200) && millis() > (peakTime + 50)) {    
    samplePeak = 1;
    data.sample_peak = sample;
    peakTime=millis();                
  }                                                           

}  

void ripple() {

  if (samplePeak == 1) {step = -1; samplePeak = 0; }          

  myRadio.write(&data, sizeof(data)); 

  Serial.print("\nPackage:");
  Serial.print("\n");
  Serial.println(data.sample_peak); 
  } 
  
