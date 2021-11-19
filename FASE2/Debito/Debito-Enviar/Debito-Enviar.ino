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
   Serial.begin(9600);
  radio.begin();
  radio.setChannel(100);
  
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_1MBPS);  
  radio.disableCRC();
  radio.setAutoAck(0);
  radio.stopListening();
}
void loop() { 
  if(i<999){
    
    ti=micros();//regista o tempo inicial de cada envio
    radio.write(&dataSend, 32);// envia 999 vezes 
    tf=micros();//regista o tempo final de cada envio
    total+=(tf-ti);//regista e vai acumolando o tempo total de cada envio
    Serial.println(i);
   i++;
    delay(100);
    }else{
     ti=micros();//igual aos de cima mas para apenas o ultimo pacote
     radio.write(&eop,sizeof(eop));
     tf=micros();
     total+=tf-ti;
      
     Serial.print("total  ");
      Serial.println(total);
     
 
      Serial.print("nbits");
      Serial.println(nBits);
      tt=total*pow(10,-6);//converte o tempo total para segundos
      Serial.println(tt);
      debito=(nBits/(tt)); //nBits/Tempototal
       Serial.println("Debito  ");
      Serial.println(debito);
      while(1);
      }

}
