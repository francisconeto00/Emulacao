#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(17, 16); // CE, CSN
const byte address[6] = "00001";
String username = "1";
String dataInput;
char dataToSend[32];
char dataReceived[32];

void setup() {
   Serial.begin(9600);
  radio.begin();
  radio.setChannel(100);
  radio.setAutoAck(0);
  radio.openWritingPipe(address);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_1MBPS);  
  radio.disableCRC();
}
void loop() {


  // listen for radio data
  radio.startListening();
  
  if ( radio.available() ) {
    // read data from radio
    radio.read( &dataReceived, sizeof(dataReceived) );
    Serial.println(dataReceived);   
    
  }

  if( Serial.available() ) {
    // stop listening on radio
    radio.stopListening();
    // get serial input
   
    dataInput = "[" + username + "] " + Serial.readStringUntil('\n');
    Serial.println(dataInput);
    
    dataInput.toCharArray(dataToSend,dataInput.length()+1);
     
    // send data
    Serial.println(dataInput.length());
    radio.write( &dataToSend, dataInput.length());  
   
  }
  
}
