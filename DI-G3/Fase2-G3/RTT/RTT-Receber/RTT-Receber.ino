#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(17, 16); // CE, CSN
const byte address[6] = "00001";
char dataToSend[32]="";
char dataReceived[32];

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
}

void loop() {
  radio.startListening();//rf comecaa ouvir
  while(radio.available()==false);//espera pela mensagem 
  radio.read(&dataReceived,sizeof(dataReceived));//le a mensagem recebida
  radio.stopListening();//rf para de ler para poder escrever
  radio.write(&dataReceived,sizeof(dataReceived));//envia a mensagem de volta
  Serial.println("------------------------- ");
  Serial.println("");
  Serial.print("Recebido: ");
  Serial.println(dataReceived);
  Serial.println("Mensagem de confirmação enviada");
}
