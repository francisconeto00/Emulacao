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
int suc=0;
int cor=0;
int enD = 0; 
boolean flag=false;
float PER=0.0;
int nBits=0;
int lost=0;
int u=0; 
void setup() {
  Serial.begin(115200); // ativa a porta
  radio.begin();// ativa o radio 
  radio.setChannel(100);// escolha do canal- canal 2400+100= channel 2500 Mhz
  radio.setPALevel(RF24_PA_MIN);// escolha do nivel do amplificador do transceiver
  radio.openReadingPipe(0, address); // abre o canal de leitura
  radio.setDataRate(RF24_1MBPS); //velocidade de transferencia do transceiver
  radio.disableCRC();  //desativa controlo de erros
  radio.setAutoAck(0); //desativa Acks automaticos 
  radio.startListening();//rf começa a ouvir para receber todos os pacotes
 
  }


  
void loop() {
  
 //utilizando o loop do arduino para receber os 1000 pacotes
  if(radio.available() && recebidos<=999 && enD==0 ){//se receber 1000 pacotes ou se receber o ultimmo sai do ciclo 
    radio.startListening();//rf começa a ouvir 
    radio.read(&txt,sizeof(txt));//le e escreve na variavel txt
    Serial.println(txt);
    
    for(i;i<sizeof(txt);i++){//ciclo para comparar os dados recebidos com os dados esperados de receber
      if(txt[i] != dataR[i]){        
          flag==true;   //se receber algum caracter diferente flag=true e assim deteta se um erro no envio 
       }
    }
        recebidos++;//incrementa o total de pacotes recebidos
      if(strcmp(eop,txt)==0){//verificaçao do ultimo pacote
        enD=1; //muda a variavel para nao entrar novamente no ciclo
        flag=false;  //o que supostamente era um erro afinal é o ultimo pacote logo a flag nao e true
        fim();//chama a função parra mostrar as estatisticas dos dados recebidos
        }
        
     if(flag){//caso a verificção de erros se verifique  
        cor++;//incrementa os corrompidos 
        flag=false;//volta a por a variavel a false para nova deteçao de erros 
        }
   Serial.println(recebidos);
    }
  
}
void fim(){
  i=0;
  suc=recebidos-cor;//n pacotes recebidos com sucesso 
  lost=1000-recebidos;//n pacotes perdidos 
  PER=((lost+cor)/1000.0)*100.0;//calculo do PER-  ((perdidos+ corrompidos)/total de pacotes)*100.0
  Serial.print("Pacotes recebidos:  ");
  Serial.print(recebidos);
  Serial.println(" de 1000");
  
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
