#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <CRCx.h>
RF24 radio(17, 16); // CE, CSN
const byte address[6] = "00001";
String username = "1";
char dataInput[31];
char dataReceived[32];
int m;
byte header;
char packet[32];
byte crcm[30];
boolean flagAck=false;
boolean flagNack=false;
boolean timeout=false;
int flagRetrans=0;
byte response=0b00000000;
boolean nadaRecebido=false;
boolean flagLimite=false;
float t=0;
int bitp=-1;
boolean iperror=false;
boolean ttt=false;
int tou=0;
boolean flagNext=true;
int sn=0;
byte ack= 0b10110100;
byte nack=0b10110110;
char res[32];
int y=0;
boolean enviar=false;
boolean recebe=true;


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
 
  radio.startListening(); //rf começa a ouvir
  //"Metodo" para receber mensagens
  if ( radio.available()) {
    while (!radio.available());
    radio.read(res,sizeof(res));
    radio.stopListening();
    receber(res);
    memset(res, 0, sizeof(res));
   } 
}


void buildPacket(char *dat){
  //cabeçalho
  header=0b11100010; //trama normal
  bitWrite(header, 0, sn);
  packet[0]=header;
  //payload
  for(int j=0;j<30;j++){
    packet[j+1]= dat[j];
    crcm[j]=dat[j];
  }
 //crc
 packet[31]=crcx::crc8(crcm,sizeof(crcm));
}

void checkAck(){    
  if(bitRead(response,1)==0&&bitRead(response,2)==1&&bitRead(response,3)==0){
    flagAck=true;
    flagRetrans=0;
  }else{ 
      if(bitRead(response,1)==1&&bitRead(response,2)==1&&bitRead(response,3)==0){
        flagNack=true;
      }else{
        nadaRecebido=true;
      }    
    }
    if(sn==0){
      sn=1;  
    }else if(sn==1){
      sn=0; 
    } 
}

 void receber(char *dataReceived){
    byte payload[30];
    char msg[30];
    memset(msg, 0, sizeof(msg));
    //radio.stopListening();
    header=dataReceived[0];
    for (int i = 0; i < 30; i++) {
        payload[i] = dataReceived[i + 1];
        msg[i]=dataReceived[i + 1];
    }
     byte crc = crcx::crc8(payload, sizeof(payload));
      if (crc == dataReceived[31]) {
        Serial.write(msg,30);
        //Serial.println(msg);
        //radio.startListening();
        radio.write(&ack, sizeof(ack));;
        enviar=true;
        recebe=false;
        //radio.stopListening(); 
      }else{
        //radio.stopListening();
        radio.write(&nack, sizeof(nack));  
      }
  }
