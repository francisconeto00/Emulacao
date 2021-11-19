#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(17, 16); // CE, CSN
const byte address[6] = "00001";
char dataToSend[32]="";
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
  radio.startListening();
  while(radio.available()==false);
  radio.read(&dataReceived,sizeof(dataReceived));
  radio.stopListening();
  radio.write(&dataReceived,sizeof(dataReceived));
  Serial.println("------------------------- ");
  Serial.println("");
  Serial.print("Recebido: ");
  Serial.println(dataReceived);
  Serial.println("Mensagem de confirmação enviada");
}
