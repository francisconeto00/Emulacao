#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <CRCx.h>
RF24 radio(17, 16); // CE, CSN
const byte address[6] = "00001";
char dataToSend[30];
String dataInput;
float rtt=0;
float tInicial=0;
float tFinal=0;
float t=0;
char dataR[32];
char packet[32];
byte crcm[30];
char dat[00];
byte response;
boolean flagAck=false;
boolean flagNack=false;
int j;
boolean timeout=true;
int flagRetrans=0;
boolean flagLimite=false;
size_t sizet = sizeof(crcm);
byte header= 0b10010000; //ip origem 00 | ip destino 01 |000 tipo trama | 0 identificador da trama 
size_t sizer = sizeof(dataToSend);

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
  dataInput.toCharArray(dataToSend,dataInput.length()+1);;//converte a mensagem recebida do teclado para um array de char 
    Serial.println(dataToSend);
  tInicial=micros();//regista o tempo logo antes do envio
  envia(0,dataToSend); 
  tFinal=micros();// regista o tempo final
  if(flagLimite){
    Serial.println("Falha por timeout");
    }else 
  fim();
  
}
void fim(){
  Serial.println("------------------------- ");
  Serial.println("");
  Serial.println("Enviado  ");
  Serial.println(packet);
  Serial.println("Recebido  ");
  Serial.println(response,BIN);
  rtt= tFinal-tInicial;
  Serial.print("Round-Trip-Time: ");
  Serial.print(rtt);
  Serial.println(" microsegundos" );
  Serial.print("Atraso " );
   Serial.println(rtt/2 );
  }

void buildPacket( int tipotrama, int numtrama, char *dat){
  //cabeçalho
  if(tipotrama== 000){
    header=0b10010000;//trama inicial
    }else if(tipotrama== 001){
       header=0b10010010; //trama normal
       }else if(tipotrama==100){
        header=0b10011000; //010 ack 011 nack
        }
        if(numtrama%2!=0){
          bitWrite(header, 0, 1);
          }
 Serial.println("header");
 Serial.println(header, BIN);
    
   packet[0]=header;
   
  //payload
  
 for(int j=0;j<sizer;j++){
  packet[j+1]= dat[j];
  crcm[j]=dat[j];
  Serial.println(packet[j+1]);
  }
 //crc
 Serial.println("Criando crc :");
 Serial.println("CRC:");
 packet[31]=crcx::crc8(crcm,sizeof(crcm));
  Serial.println(packet[31],BIN);
 
  
  }

void checkAck(){
   Serial.println(bitRead(response,0));
    Serial.println(bitRead(response,1));
     Serial.println(bitRead(response,2));
      Serial.println(bitRead(response,3));
  Serial.println(bitRead(response,4));
   Serial.println(bitRead(response,5));
    Serial.println(bitRead(response,6));
     Serial.println(bitRead(response,7));
  if(bitRead(response,1)==0&&bitRead(response,2)==1&&bitRead(response,3)==0){
    flagAck=true;
    flagRetrans=0;
 Serial.println("Recebi Ack");
    }else if(bitRead(response,1)==1&&bitRead(response,2)==1&&bitRead(response,3)==0){
      flagNack=true;
    
      Serial.println("Recebi Nack"); 
      Serial.println(flagNack);
      }
   }
  
void envia(int i,char *dat){
    radio.stopListening();
    if(i==0){//trama inicial
      j=000;
      }else if(i==999){//trama final
        j=100;
        }else j=001;//se nao envia uma trama normal
    buildPacket(i,j,dat);
    Serial.println("Enviei");
    Serial.println(packet);
    radio.write(packet,sizeof(packet));
    t=micros();
    radio.startListening();
     Serial.println("Is TIME OUT?");
       Serial.println(timeout);
    while(!radio.available()){
      if((micros()-t)>5000){
        timeout=true;
        flagLimite=true;
        break;
        }
      }
      Serial.println("Is TIME OUT?");
       Serial.println(timeout);
      if(timeout==false){
        radio.read(&response,sizeof(response));
        checkAck();
        if(flagAck==true){
        Serial.print("Ack recebido");
        flagRetrans=0;
        flagAck=false;
      } 
      if(flagNack==true){
            flagNack=false;
            if(flagRetrans<5){
                Serial.println("RET----------------------");
                  Serial.println(flagRetrans);
              flagRetrans++;
              delay(100);
              envia(i,dataToSend);
              Serial.print("Nack recebido, a enivar novamente");
            }else {
              Serial.print("5 tentativas de envio sem sucesso");}
              flagLimite=true;
              flagRetrans=0;
            }
          }
      timeout=false;
      
     
    }
