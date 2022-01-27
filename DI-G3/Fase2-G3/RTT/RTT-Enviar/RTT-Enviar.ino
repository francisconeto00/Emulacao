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
  Serial.begin(115200); // ativa a porta
  radio.begin();// ativa o radio 
  radio.setChannel(100);// escolha do canal- canal 2400+100= channel 2500 Mhz
  radio.setPALevel(RF24_PA_MIN);// escolha do nivel do amplificador do transceiver
  radio.openWritingPipe(address); // abre o canal de escrita
  radio.openReadingPipe(0, address); // abre o canal de leitura
  radio.setDataRate(RF24_1MBPS); //velocidade de transferencia do transceiver
  radio.disableCRC();  //desativa controlo de erros
  radio.setAutoAck(0); //desativa Acks automaticos 
  radio.stopListening();//rf para de ler para enviar a mensagem
}

void loop() {
  
  while(Serial.available()==false); //espera pelo uso do teclado 
  dataInput=Serial.readStringUntil('\n');//lê ate ao Enter
  dataInput.toCharArray(dataToSend,dataInput.length()+1);//converte a mensagem recebida do teclado para um array de char 
  
  tInicial=micros();//regista o tempo logo antes do envio
  while(radio.write( &dataToSend, dataInput.length())==false);  //envia e espera pelo envio total  
  radio.startListening();//rf modo rx 
  while(radio.available()==false);//espera para receber a mensagens de confirmação
  tFinal=micros();// regista o tempo final
  
  radio.read(&dataRecived, sizeof(dataRecived));//lê a mensagem recebida 
  radio.stopListening();//rf modo tx
  Serial.println("------------------------- ");
  Serial.println("");
  Serial.println("Enviado  ");
  Serial.println(dataToSend);
  Serial.println("Recebido  ");
  Serial.println(dataRecived);
  rtt= tFinal-tInicial;
  Serial.print("Round-Trip-Time: ");
  Serial.print(rtt);
  Serial.println(" microsegundos" );
  Serial.print("Atraso " );
   Serial.println(rtt/2 );
  
}
