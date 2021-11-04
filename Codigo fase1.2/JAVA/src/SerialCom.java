

import com.fazecast.jSerialComm.SerialPort;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Scanner;


public class SerialCom {


    public static void main(String[] Args) throws IOException {

        SerialPort[] portas=SerialPort.getCommPorts();
        if(portas.length==0){
            System.out.println("Nao existem portas disponiveis");
        }
           System.out.println("Escolha a porta que deseja usar: \n");
        int i = 1;
        int y=0;
        for(SerialPort porta : portas){
            System.out.println(i++ + " - " + porta.getSystemPortName());
        }
        Scanner in= new Scanner(System.in);
        int numeroPorta= in.nextInt()-1;
        SerialPort porta = portas[numeroPorta];

        porta.openPort();

         if(porta.isOpen()){
             System.out.println("Porta COM aberta \n");
         }else {
             System.out.println("Porta COM fechada\n");
         }

        //setComPortParameters(int newBaudRate, int newDataBits, int newStopBits, int newParity)
        porta.setComPortParameters(9600,8,1,0);                          //default

        int op;
            System.out.println("-----------------------------------------------------");
            System.out.println("| Emulação e Simulação de Redes de Telecomunicações |");
            System.out.println("| Comunicação via porta série PC-Arduino-Arduino-PC |");
            System.out.println("|                                                   |");
            System.out.println("|             1- Enviar documento de texto          |");
            System.out.println("|             2- Enviar imagem                      |");
            System.out.println("|             3- Receber documento de texto         |");
            System.out.println("|             4- Receber imagem                     |");
            System.out.println("|             5-Sair                                |");
            System.out.println("|                                                   |");
            System.out.println("-----------------------------------------------------");

                Scanner in1= new Scanner(System.in);
                 op= in1.nextInt();
       long inicio;
       long fim;
       long total;


        switch (op) {
            case 1 -> {  //enviar txt
                //enviar
                ByteArrayOutputStream out = new ByteArrayOutputStream();
                Files.copy(Paths.get("C:\\Users\\Utilizador\\Desktop\\U.M\\toSend.txt"), out);
                System.out.println("out  " + out);
                byte[] b = out.toByteArray();
                inicio = System.currentTimeMillis();
                porta.writeBytes(b,b.length);
                while(porta.bytesAwaitingWrite()>0){
                    System.out.println("a espera");
                }
                fim = System.currentTimeMillis();
                total = fim - inicio;
                System.out.println(total + " ms para enviar o documento de texto");





            }
            case 2 -> {  //enviar png
                inicio = System.currentTimeMillis();
                //enviar
                ByteArrayOutputStream out = new ByteArrayOutputStream();
                Files.copy(Paths.get("C:\\Users\\Utilizador\\Desktop\\U.M\\foto.png"), out);
                System.out.println("out  " + out);
                byte[] b = out.toByteArray();
                porta.writeBytes(b,17608);

                while(porta.bytesAwaitingWrite()>0){
                    System.out.println("a espera");
                }
                System.out.println("b   " + Arrays.toString(b));
                fim = System.currentTimeMillis();
                total = fim - inicio;
                System.out.println(total + " ms para enviar uma imagem");

            }

            case 3 -> { //receber txt
                    byte[] buffer = new byte[86];
                
                

                    while(porta.bytesAvailable()<86){
                        System.out.println(porta.bytesAvailable());
                    }
                porta.readBytes(buffer, 86);
                System.out.println(Arrays.toString(buffer));

                Path path = Paths.get("C:\\Users\\Utilizador\\Desktop\\TXTRecebido.txt");
                if(Files.exists(path)){
                    Files.delete(path);
                }
                Files.createFile(path);
                Files.write(path,buffer);
                porta.closePort();
            }
            case 4 -> {  //receber png
                byte[] buffer = new byte[17608];
                while(porta.bytesAvailable()<17608){
                    System.out.println(porta.bytesAvailable());
                }
                porta.readBytes(buffer, 17608);
                System.out.println(Arrays.toString(buffer));
                Path path = Paths.get("C:\\Users\\Utilizador\\Desktop\\FOTORecebida.png");
                if(Files.exists(path)){
                    Files.delete(path);
                }
                Files.createFile(path);
                Files.write(path,buffer);
                porta.closePort();
            }
            case 5 ->  //sair
                porta.closePort();

            default -> throw new IllegalStateException("Unexpected value: " + op);
        }



    }


}
