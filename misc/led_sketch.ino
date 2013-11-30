#include <FastSPI_LED.h>

#define NUM_LEDS 125

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { 
  unsigned char b; 
  unsigned char r; 
  unsigned char g; 
};
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;

#define PIN 3

void setup() {
  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_WS2811);

  FastSPI_LED.setPin(PIN);

  FastSPI_LED.init();
  FastSPI_LED.start();

  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
}

void loop() { 
  memset(leds, 0, NUM_LEDS * 3);

  // one at a time
  for(int j = 0; j < 3; j++) { 
    for (int l = 0; l < 80; l++) {
      int v = l;
      if (v > 40) v = 40-(v-40);
      for(int i = 0 ; i < NUM_LEDS; i++ ) {
        switch(j) { 
        case 0: 
          leds[i].r = v; 
          break;
        case 1: 
          leds[i].g = v; 
          break;
        case 2: 
          leds[i].b = v; 
          break;
        }
      }
      FastSPI_LED.show();
      delay(50);
    }
  }
}


