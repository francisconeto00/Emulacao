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
boolean timeout=true;
int flagRetrans=0;
byte response=0b0000000;
boolean nadaRecebido=false;
boolean flagLimite=false;
float t=0;
int bitp=-1;
boolean iperror=false;
boolean ttt=false;
int tou=0;
boolean flagNext=true;
int sn=0;
byte ack = 0b00000000;
byte nack = 0b00001111;


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
  radio.startListening(); //rf começa a ouvir

  //"Metodo" para receber mensagens
  if ( radio.available() ) {
    receber();
  }
  
  //"Metodo" para enviar mensagens
  if( Serial.available() ) {
    radio.stopListening(); //o rf para de ouvir para conseguir escrever
   if (Serial.available() > 0) {
        dataInput[m] = Serial.read();
        m++;
      }
      if(m>0){
        envia(dataInput);
      }
      Serial.print(dataInput);
       memset(dataInput, '\0', sizeof(dataInput));
       m=0;
  } 
}


void buildPacket(char *dat){
Serial.println("Entrei em buildpacket()");
  //cabeçalho
       header=0b11100010; //trama normal
            bitWrite(header, 0, sn);
            Serial.print("STATE  ");
            Serial.print(sn);
 Serial.print("header  ");
 Serial.println(header, BIN);
    
   packet[0]=header;
  //payload
  
 for(int j=0;j<30;j++){
   packet[j+1]= dat[j];
  crcm[j]=dat[j];

  }
 //crc
  Serial.println("Criando crc :");
 Serial.println("CRC:");
 packet[31]=crcx::crc8(crcm,sizeof(crcm)); 
 Serial.print("CRC-------");
 Serial.print(packet[31]);
}

void checkAck(){    
  if(bitRead(response,1)==0&&bitRead(response,2)==1&&bitRead(response,3)==0){
    flagAck=true;
    flagRetrans=0;
    Serial.println("Recebi Ack"); 
    }else{ if(bitRead(response,1)==1&&bitRead(response,2)==1&&bitRead(response,3)==0){
           flagNack=true;
            Serial.println("Recebi Nack"); 
           }else
           nadaRecebido=true;
    }
    if(sn==0){
      sn=1;
      
      }else if(sn==1){
       sn=0; 
        } 
   }
void envia(char *dat){
  delay(100);
  radio.stopListening();
   buildPacket(dat);
   //pacote ja criado cabeçalho payload e crc
   radio.write(packet,sizeof(packet));
   Serial.print("enviei");
  //enviou o pacote
  //falta agora aguardar pela confirmação
 t=micros();//se nao receber nada quer dizer que o recetor esta fora do alcance 
 radio.startListening();
Serial.println(" startlistening.");
  Serial.println("Radio   ");
  Serial.println(radio.available());
   while(!radio.available()){
      if((micros()-t)>5000){
        timeout=true;
        break;
        Serial.println("  a espera....");
        }
      }
      if(!timeout){ //caso tenha recebido alguma coisa antes do timeout
         Serial.println("Entrei  ");
        radio.read(&response,sizeof(response)); 
        if(bitRead(response,5)==1 && bitRead(response,4)==1){//ip check
        checkAck();
        Serial.println(bitRead(response,5));
        Serial.println(bitRead(response,4)); 
        }else iperror=true;
        }
        
       
        if(flagAck){//detetou Ack tudo ok
            Serial.print("Ack recebido-Mensagem enviada");//reset a tudo
            flagRetrans=0;
            flagAck=false;
            
       
            
          }
          
         if(flagNack || iperror){ //nova transmissao caso ja nao tenha sido time out ou 5 retransmissoes
          flagNack=false;//reset da variavel
          nadaRecebido=false;
          ttt=true;
          if(flagRetrans<5){
            Serial.print("------------------Retransmissao nº ");
            Serial.println(flagRetrans);
            flagRetrans++;
            //vamos voltar a tentar enviar 
            envia(dat);
            
             
            }else {
               Serial.print("-- 5 tentativas sem sucesso --");
               flagLimite=true;
              flagRetrans=0;
                    
        
              }
          
          }
           timeout=false;
  }
  
  void receber(){
    byte payload[30];
    char msg[30];
     radio.startListening();
     while (!radio.available()){
  radio.read(dataReceived, sizeof(dataReceived));
  }
   header=dataReceived[0];
  if(bitRead(response,5)==1 && bitRead(response,4)==1){//ip check
       for (int i = 0; i < 30; i++) {
        payload[i] = dataReceived[i + 1];
        msg[i]=dataReceived[i + 1];
      }
      byte crc = crcx::crc8(payload, sizeof(payload));
      if (crc == dataReceived[31]) {
        radio.stopListening();
        radio.write(&ack, sizeof(ack)); 
        Serial.println(msg);
      }else{
         radio.stopListening();
        radio.write(&nack, sizeof(nack));
        
        }
  }
  
     
    
    }
  
  
