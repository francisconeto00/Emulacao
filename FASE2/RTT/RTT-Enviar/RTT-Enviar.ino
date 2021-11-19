#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(17, 16); // CE, CSN
const byte address[6] = "00001";
char dataToSend[9]="Mensagem";
char dataRecived[32];
String dataInput;
float rtt=0;
float tInicial=0;
float tFinal=0;
char dataR[32];

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
  radio.stopListening();
}

void loop() {
  
  while(Serial.available()==false);
  dataInput=Serial.readStringUntil('\n');
  dataInput.toCharArray(dataToSend,dataInput.length()+1);
  tInicial=micros();
  while(radio.write( &dataToSend, dataInput.length())==false);  
  radio.startListening();
  while(radio.available()==false);
  tFinal=micros();
  radio.read(&dataRecived, sizeof(dataRecived));
  radio.stopListening();
  Serial.println("------------------------- ");
  Serial.println("");
  Serial.println("Enviado  ");
  Serial.println(dataToSend);
  Serial.println("Recebido  ");
  Serial.println(dataRecived);
  rtt= tFinal-tInicial;
  Serial.println("Round-Trip-Time: ");
  Serial.print(rtt);
  Serial.println(" microsegundos" );
  Serial.println("Atraso " );
   Serial.println(rtt/2 );
  
}
