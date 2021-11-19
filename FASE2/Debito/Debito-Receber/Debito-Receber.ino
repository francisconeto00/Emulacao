#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(17, 16); // CE, CSN
const byte address[6] = "00001";
char dataR[32]="Grupo3";
char eop[32]="Ultimo";
char txt[32]="";
int i=0;
int recebidos=0;
int sus=0;
int cor=0;
int enD = 0; 
boolean flag=false;
float PER=0.0;
int nBits=0;
int lost=0;
int u=0; 
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(100);
  radio.openWritingPipe(address);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_1MBPS);  
  radio.disableCRC();
  radio.setAutoAck(0);
  radio.startListening();
 
  }


  
void loop() {
  
 
  if(radio.available() && recebidos<=999 && enD==0 ){
    radio.startListening();
    radio.read(&txt,sizeof(txt));
    Serial.println(txt);
    
    for(i;i<sizeof(txt);i++){
      if(txt[i] != dataR[i]){        
          flag==true;   
       }
    }
        recebidos++;
      if(strcmp(eop,txt)==0){
        fim();
        enD=1;
        flag=false;
        }
        
     if(flag){
        cor++;
        flag=false;
        }
   Serial.println(recebidos);
    }
  
}
void fim(){
  i=0;
  sus=recebidos-cor;
  lost=1000-recebidos;
  PER=((lost+cor)/1000.0)*100.0;
  Serial.print("Pacotes recebidos:  ");
  Serial.print(recebidos);
  Serial.println(" de 1000");
  
  Serial.print("Pacotes recebidos com sucesso ");
  Serial.println(sus);

  Serial.print("Pacotes recebidos corrompidos ");
  Serial.println(cor);
  
  Serial.print("Pacotes perdidos: ");
  Serial.println(lost);
  
  Serial.print("Packet Error Rate - PER: ");
  Serial.println(PER);

  
  }
