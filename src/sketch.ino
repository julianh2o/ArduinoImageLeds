#include <FastSPI_LED.h>

#define NUM_LEDS 125

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { 
    unsigned char g; 
    unsigned char r; 
    unsigned char b; 
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
    memset(leds, 0, NUM_LEDS * 3); //clear LEDs
    FastSPI_LED.show();

    Serial.begin(115200);
}

void loop() { 
    while(Serial.available() == 0);

    short byteCount = 0;
    byteCount += Serial.read() << 8;
    byteCount += Serial.read();

    char buffer [byteCount];
    Serial.readBytes(buffer,byteCount);

    char ledStart = buffer[0];
    char * startIndex = (char *)(leds) + ledStart*3;
    int i=1;
    while (i<byteCount) {
        startIndex[0] = buffer[i+1];
        startIndex[1] = buffer[i];
        startIndex[2] = buffer[i+2];
        i+=3;
        startIndex+=3;
    }

    //memset(leds, 0, NUM_LEDS * 3);
    FastSPI_LED.show();

    Serial.write(1);
}


