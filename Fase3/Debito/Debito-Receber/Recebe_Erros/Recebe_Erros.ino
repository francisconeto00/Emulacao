#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <CRCx.h>
RF24 radio(17, 16); // CE, CSN
const byte address[6] = "00001";
byte dataR[30];
char eop[32]="Ultimo";
char txt[32]="";
int i=0;
int recebidos=0;
int suc=0;
int cor=0;
int enD = 0; 
boolean flag=false;
float PER=0.0;
int nBits=0;
int lost=1;
int u=0; 
byte pcrc;
int n;
int Last=1;
boolean flagAck=false;
boolean flagNack=false;
byte ack= 0b10110100;
byte nack=0b10110110;
int nacksSeguidos=0;
int uns=0;
int bitp=0;
boolean ip=true;
int sn=0;
int snn=1;


void setup() {
  Serial.begin(115200); // ativa a porta
  radio.begin();// ativa o radio 
  radio.setChannel(100);// escolha do canal- canal 2400+100= channel 2500 Mhz
  radio.setPALevel(RF24_PA_MIN);// escolha do nivel do amplificador do transceiver
  radio.openReadingPipe(0, address); // abre o canal de leitura
    radio.openWritingPipe(address); // abre o canal de escrita
  radio.setDataRate(RF24_1MBPS); //velocidade de transferencia do transceiver
  radio.disableCRC();  //desativa controlo de erros
  radio.setAutoAck(0); //desativa Acks automaticos 
  radio.startListening();//rf começa a ouvir para receber todos os pacotes
 
  }


  
void loop() {
  radio.startListening();//rf comecaa ouvir
 //utilizando o loop do arduino para receber os 1000 pacotes
  while (!radio.available());
  if(radio.available() && recebidos<=1000 && enD==0 ){//se receber 1000 pacotes ou se receber o ultimmo sai do ciclo 
    Serial.println("A Ouvir ...");
    radio.read(&txt,sizeof(txt));//le e escreve na variavel txt
    Serial.println(txt);
    radio.stopListening(); 
     Serial.println("ip    "); 
    Serial.println(bitRead(txt[0],5));
    Serial.println(bitRead(txt[0],4));
     
     if(bitRead(txt[0],5)!=1 && bitRead(txt[0],4)!=0){
          ip=false;
      }
     
    for(int x=0;x<30;x++){
      dataR[x]=txt[x+1];
      }
    byte crc=crcx::crc8(dataR, sizeof(dataR));
 
    
   
     Serial.println("STATE");
       Serial.println(bitRead(txt[0], 0));
        Serial.println("Anterior");
         Serial.println(snn);
        
       Serial.println(bitp);
      Serial.println("CRC");
      Serial.println(crc);
       Serial.println(txt[31]);
       if (recebidos == 48 || recebidos== 59){
        txt[31]=-1;
        }
    if(crc!=txt[31] || ip==false || recebidos==5 || recebidos==12 || recebidos== 25 || recebidos==37 || recebidos==48 || recebidos==59 ||recebidos== 69 
    ||recebidos==79 ||recebidos==89 ||recebidos==99){ 
      //envia nack  
      Serial.println("Enviei Nack");
      radio.write(&nack,sizeof(nack));//envia nack
        cor++;
         
      }else{
      //envia Ack
      sn=bitRead(txt[0], 0);
      Serial.println("Enviei Ack");
      radio.write(&ack,sizeof(ack));//envia ack
      suc++; 
      if(sn==snn){
        Serial.println("Pacote repetido");
      }
      snn=bitRead(txt[0], 0);
      if(bitRead(txt[0],1)==0 &&bitRead(txt[0],2)==0 && bitRead(txt[0],3)==1){
        enD=1;//ultima trama recebida
        }
        ip=true;
          
    Serial.print("RECEBIDOS ");
   Serial.println(recebidos);
    } 
   
    recebidos++;//incrementa o total de pacotes recebidos
      if(enD==1){//verificaçao do ultimo pacote
        fim();//chama a função parra mostrar as estatisticas dos dados recebidos
        
        }
  }     
 
}
void fim(){
  i=0;
  lost=100-suc;//n pacotes perdidos 
  PER=((lost+cor)/100.0)*100.0;//calculo do PER-  ((perdidos+ corrompidos)/total de pacotes)*100.0
  Serial.print("Pacotes recebidos:  ");
  Serial.print(recebidos);
  Serial.println(" de 100");
  
  Serial.print("Pacotes recebidos com sucesso ");
  Serial.println(suc);

  Serial.print("Pacotes recebidos corrompidos ");
  Serial.println(cor);
  
  Serial.print("Pacotes perdidos: ");
  Serial.println(lost);
  
  Serial.print("Packet Error Rate - PER: ");
  Serial.println(PER);
  Serial.print("%");

  
  }
