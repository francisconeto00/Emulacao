#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <CRCx.h>
RF24 radio(17, 16); // CE, CSN
const byte address[6] = "00001";
char dat[30]="Grupo3";
char eop[32]="Ultimo";
int i=0;
long ti=0;
long tf=0;
long total;
double tt=0;
float debito;
int nBits =32*8*1000;
int j;
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
size_t sizer = sizeof(dat);
int uns=0;
int bitp=-1;
boolean iperror=false;
boolean ttt=false;
int tou=0;
boolean flagNext=true;
int sn=0;



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
  radio.stopListening();//desativa o modo de "leitura" do rf para assim conseguir enviar
}
void loop() { 
  
      if(i<=100){ 
    //utilizando o loop infinito do arduino vamos enviar 1000pacotes  "Grupo3"
  
    ti=micros();//regista o tempo inicial de cada envio
    envia(i,dat);
    tf=micros();//regista o tempo final de cada envio
    total+=(tf-ti);//regista e vai acumolando o tempo total de cada envio
   i++;
  }else{
      Serial.print("numero de bits ");
      Serial.println(nBits);  
      Serial.print("total t ");
      tt=total*pow(10,-6);//converte o tempo total para segundos
      debito=(nBits/(tt)); //nBits/Tempototal
       Serial.println("Debito  ");
      Serial.println(debito);
      tf=micros();//regista o tempo final de cada envio
      total+=(tf-ti);//regista e vai acumolando o tempo total de cada 
      while(1);//quando enviar os 1000 pacotes fica parado
      }
    
 }
  


void buildPacket( int tipotrama, int numtrama, char *dat){
  //cabeçalho
  if(tipotrama== 000){
    header=0b11100000;//trama inicial
    }else if(tipotrama== 001){
       header=0b11100010; //trama normal
       }else if(tipotrama==100){
        header=0b11101000; //010 ack 011 nack
       }
            bitWrite(header, 0, sn);
 Serial.print("header  ");
 Serial.println(header, BIN);   
   packet[0]=header;
  //payload
  
 for(int j=0;j<sizer;j++){
   packet[j+1]= dat[j];
  crcm[j]=dat[j];

  }
 //crc
 Serial.print("CRC:");
 packet[31]=crcx::crc8(crcm,sizeof(crcm)); 
 Serial.println(packet[31]);
}
void checkAck(){
  Serial.println("Recebido");  
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


void envia(int i, char *dat){
  delay(10);
  Serial.print("PACOTE  n");
  Serial.println(i);
  radio.stopListening();
   if(i==0){//trama inicial
      j=000;
      }else if(i==99){//trama final
        j=100;
        }else j=001;//se nao envia uma trama normal
   buildPacket(j,i,dat);
   //pacote ja criado cabeçalho payload e crc
   radio.write(packet,sizeof(packet));
  //enviou o pacote
  //falta agora aguardar pela confirmação
 t=micros();//se nao receber nada quer dizer que o recetor esta fora do alcance 
 radio.startListening();
   while(!radio.available()){
      if((micros()-t)>5000){
        timeout=true;
        Serial.println("  a espera....");
        break;
        }
      } 
     
      if(!timeout){ //caso tenha recebido alguma coisa antes do timeout
        radio.read(&response,sizeof(response)); 
        if(bitRead(response,5)==1 && bitRead(response,4)==1){
         
        checkAck();
        
        }else iperror=true;
        }
        
       
        if(flagAck){//detetou Ack tudo ok
           //reset a tudo
            flagRetrans=0;
            flagAck=false;
       
            
          }
          
         if(flagNack || iperror){ //nova transmissao caso ja nao tenha sido time out ou 5 retransmissoes
          flagNack=false;//reset da variavel
          nadaRecebido=false;
          ttt=true;
          if(flagRetrans<5){
            Serial.print("------Retransmissao nº ");
            Serial.println(flagRetrans);
            flagRetrans++;
            //vamos voltar a tentar enviar 
            envia(i,dat);
            
             
            }else {
               Serial.print("-- 5 tentativas sem sucesso --");
               flagLimite=true;
              flagRetrans=0;
                    
        
              }
          
          }
           timeout=false;
  }
   
