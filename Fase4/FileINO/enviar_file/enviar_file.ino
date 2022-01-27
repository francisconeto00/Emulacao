#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <CRCx.h>
RF24 radio(17, 16); // CE, CSN
const byte address[6] = "00001";
String username = "1";
char dataInput[31];
char dataReceived[32];
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
int sn=0;
boolean iperror=false;
boolean flagNext=true;
char res[32];
byte ack= 0b10110100;
byte nack=0b10110110;
boolean enviar=true;
boolean recebe=false;
char A='A';



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
  //"Metodo" para enviar mensagens
  if( Serial.available() ) {
    radio.stopListening(); //o rf para de ouvir para conseguir escrever
    if (Serial.available() > 0) {  
       Serial.readBytes(dataInput, 30);
       envia(dataInput);
       memset(dataInput, 0, sizeof(dataInput));
    }
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
void envia(char *dat){
  //delay(100);
  buildPacket(dat); 
  radio.stopListening();
  radio.write(packet,sizeof(packet));
  radio.startListening(); 
  memset(packet, 0, sizeof(packet));
  t=micros();//se nao receber nada quer dizer que o recetor esta fora do alcance 
  //radio.startListening();
      if(!timeout){ //caso tenha recebido alguma coisa antes do timeout
        while(!radio.available()){ 
        }
        radio.read(&response,sizeof(response));  
        radio.stopListening();
        checkAck();
        if(flagAck){//detetou Ack tudo ok
            flagRetrans=0;
            flagAck=false;
            enviar=false;
            recebe=true;
            Serial.write(&A,sizeof(A));
          }
          
         if(flagNack || iperror){ //nova transmissao caso ja nao tenha sido time out ou 5 retransmissoes
          flagNack=false;//reset da variavel
          nadaRecebido=false;
        
          if(flagRetrans<5){
            flagRetrans++;
            //vamos voltar a tentar enviar
            envia(dat);
            
          }else {
            flagLimite=true;
            flagRetrans=0;
          }
          
        }
        timeout=false;
        //radio.startListening();
     }
}
  
