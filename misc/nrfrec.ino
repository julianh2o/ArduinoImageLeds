// vim: syntax=cpp
/**
 * A Mirf example to test the latency between two Ardunio.
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 * Note: To see best case latency comment out all Serial.println
 * statements not displaying the result and load 
 * 'ping_server_interupt' on the server.
 */

#include <nRF24L01.h>
#include <SPI.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>

void setup() {
    Serial.begin(9600);

    Mirf.cePin = 8;
    Mirf.csnPin = 7;
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();

    uint8_t rx_addr[] = {0x34,0x43,0x10,0x10,0x00};
    uint8_t tx_addr[] = {0x34,0x00,0x10,0x10,0x00};

    Mirf.setRADDR((byte *)&rx_addr);
    Mirf.setTADDR((byte *)&tx_addr);

    Mirf.payload = 32;
    Mirf.channel = 40;

    Mirf.config();

    delay(3000);
}

void loop(){
    uint8_t data [] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

    Serial.println("loop");
    Serial.println(Mirf.getStatus());

    uint8_t val;
    for (int i=0; i<=0x1d; i++) {
        Mirf.readRegister(i,&val,1);
        Serial.print("0x");
        Serial.print(i,HEX);
        Serial.print("\t");
        Serial.print("0x");
        Serial.println(val,HEX);
    }

    if(!Mirf.isSending() && Mirf.dataReady()) {
        Serial.println("Got packet!");

        uint8_t rec [32];
        Mirf.getData((byte *) &rec);
        for (int i=0; i<32; i++) {
            Serial.print("0x");
            Serial.print(rec[i],HEX);
        }
        Serial.println();
    }

/*
    while(Mirf.isSending()){
    }
    Serial.println("Finished sending");
    delay(10);
    while(!Mirf.dataReady()){
        //Serial.println("Waiting");
        if ( ( millis() - time ) > 1000 ) {
            Serial.println("Timeout on response from server!");
            return;
        }
    }


    Serial.print("Ping: ");
    Serial.println((millis() - time));
*/

    delay(100);
} 
