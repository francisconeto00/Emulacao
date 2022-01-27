#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <CRCx.h>
RF24 radio(17, 16); // CE, CSN
const byte address[6] = "00001";
byte pcrc[30];
char dataReceived[32];
byte ack= 0b10010100;
byte nack=0b10010110;
boolean flagNack=false;
boolean flagAck=false;
void setup() {
  Serial.begin(115200); // ativa a porta
  radio.begin();// ativa o radio 
  radio.setChannel(100);// escolha do canal- canal 2400+100= channel 2500 Mhz
  radio.setPALevel(RF24_PA_MIN);// escolha do nivel do amplificador do transceiver
 radio.openWritingPipe(address); // abre o canal de escrita
  radio.openReadingPipe(0, address); // abre o canal de leitura
  radio.setDataRate(RF24_1MBPS); //velocidade de transferencia do transceiver
  radio.disableCRC();  //desativa controlo de erros
  radio.setAutoAck(0); //desativa Acks automaticos
  radio.startListening();
}

void loop() {
  radio.startListening();//rf comecaa ouvir
  while(radio.available()==false);//espera pela mensagem 
  radio.read(&dataReceived,sizeof(dataReceived));//le a mensagem recebida
  radio.stopListening();//rf para de ler para poder escrever
  for(int i=0; i<30;i++){
    pcrc[i]=dataReceived[i+1];
    }
    byte crc =crcx::crc8(pcrc, sizeof(pcrc));
    Serial.println("CRC:");
    Serial.println(crc,BIN);
    
    if(crc==dataReceived[31]){
     //envia ack
     radio.write(&ack,sizeof(ack));//envia ack
     flagAck=true;
      
     }else {
      //envia nack
       radio.write(&nack,sizeof(nack));//envia nack
       flagNack=true;
      }

 
  Serial.println("------------------------- ");
  Serial.println("Recebido");
  Serial.println(dataReceived);
  if(flagAck){
  Serial.println("Ack enviado"); 
  Serial.println(ack,BIN);
  flagAck=false;
  }else if (flagNack){
    Serial.println("Nack enviado");
    Serial.println(nack,BIN);
    flagNack=false;
    }
  
}


 
