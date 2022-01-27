#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <cmath>
RF24 radio(17, 16); // CE, CSN
const byte address[6] = "00001";
char dataSend[32]="Grupo3";
//char dataSend[32]="Ultimo";
char eop[32]="Ultimo";
int i=0;
long ti=0;
long tf=0;
long total;
double tt=0;
float debito;
int nBits =32*8*1000;

void setup() {
  Serial.begin(115200); // ativa a porta
  radio.begin();// ativa o radio 
  radio.setChannel(100);// escolha do canal- canal 2400+100= channel 2500 Mhz
  radio.setPALevel(RF24_PA_MIN);// escolha do nivel do amplificador do transceiver
  radio.openWritingPipe(address); // abre o canal de escrita
  radio.setDataRate(RF24_1MBPS); //velocidade de transferencia do transceiver
  radio.disableCRC();  //desativa controlo de erros
  radio.setAutoAck(0); //desativa Acks automaticos 
  radio.stopListening();//desativa o modo de "leitura" do rf para assim conseguir enviar
}
void loop() { 
  if(i<999){ 
    //utilizando o loop infinito do arduino vamos enviar 999 pacotes  "Grupo3"
    
    ti=micros();//regista o tempo inicial de cada envio
    radio.write(&dataSend, sizeof(dataSend));// envia 999 vezes "Grupo3" 
    tf=micros();//regista o tempo final de cada envio
    total+=(tf-ti);//regista e vai acumolando o tempo total de cada envio
    Serial.println(i);
   i++;
    delay(100);
    }else{//envio do ultimo pacotes, assim pacotes total= 1000
     ti=micros();//regista o tempo inicial do envio do ultimo pacote
     radio.write(&eop,sizeof(eop));//envia o ultimo pacote
     tf=micros();//regista o tempo final de cada pacote 
     total+=tf-ti;//adiciona esse tempo ao tempo total ja tirado
      
     Serial.print("total  ");
      Serial.println(total);
     
 
      Serial.print("nbits ");
      Serial.println(nBits);
      tt=total*pow(10,-6);//converte o tempo total para segundos
      debito=(nBits/(tt)); //nBits/Tempototal
       Serial.println("Debito  ");
      Serial.println(debito);
      while(1);//quando enviar os 1000 pacotes fica parado
      }

}
