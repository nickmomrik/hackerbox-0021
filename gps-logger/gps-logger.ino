/*
   Log GPS NMEA data to an SD card every second
   
   This code was adapted from the MAKE: GPS Cat Tracker Project 
   http://makezine.com/projects/make-37/gps-cat-tracker-2/

   Adapted for http://www.instructables.com/id/HackerBoxes-0021-Hacker-Tracker/

   Further modified by Nick Momrik.
*/
 
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
 
// Arduino pins used by the GPS module
static const int GPS_RXPin = A1;
static const int GPS_TXPin = A0;
static const int GPS_Baud = 9600;

// Arduino pin for SD Card
static const int SD_ChipSelect = 4;

// Filename for GPS logging
char fileName[] = "gps.txt";
File dataFile;
 
// The GPS connection is attached with a software serial port
SoftwareSerial Gps_serial( GPS_RXPin, GPS_TXPin );
 

void setup() {      
  // Open the debug serial port at 9600
  Serial.begin( 9600 );
 
  // Open the GPS serial port 
  Gps_serial.begin( GPS_Baud );
   
  Serial.print( "Initializing SD card..." );
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode( SD_ChipSelect, OUTPUT );
  
  // see if the card is present and can be initialized:
  if ( ! SD.begin( SD_ChipSelect ) ) {
    Serial.println( "Card failed, or not present" );

    return;
  }

  Serial.println( "Card initialized." );

  if ( ! SD.exists( fileName ) ) {
    dataFile = SD.open( fileName, FILE_WRITE );
    dataFile.close();

    if ( SD.exists( fileName ) ) {
      Serial.print( "Created " );
    } else {
      Serial.print( "Error creating " );
    }
    Serial.println( fileName );
  }
}

int inByte = 0;         // incoming serial byte
byte pbyGpsBuffer[100];
int byBufferIndex = 0;
 
void loop() {
  byte byDataByte;
  
  if ( Gps_serial.available() ) {
    byDataByte = Gps_serial.read();

    Serial.write( byDataByte );
    pbyGpsBuffer[ byBufferIndex++ ] = byDataByte;
     
    if ( byBufferIndex >= 100 ) {
      byBufferIndex = 0;     

      if ( SD.exists( fileName ) ) {
        dataFile = SD.open( fileName, FILE_WRITE );
        // if the file is available, write to it:
        if ( dataFile ) {
          dataFile.write( pbyGpsBuffer, 100 );
          dataFile.close();
        } else {
          Serial.print( "Error opening " );
          Serial.println( fileName );
        }
      }
    }
  }
}
