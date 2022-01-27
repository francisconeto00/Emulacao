import com.fazecast.jSerialComm.SerialPort;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Scanner;

import static java.lang.Thread.sleep;

public class Emissor {

    public static boolean sendMessage(SerialPort serialPort) throws IOException {
        byte[] messageBytes = new byte[30];
        System.out.print("EU: ");
        Scanner input = new Scanner(System.in);
        String msg = input.nextLine();
        if(msg.equals("end")){
            return false;
        }
        messageBytes = msg.getBytes();
        serialPort.writeBytes(messageBytes,messageBytes.length);
        return true;
    }

    public static void readMessage(SerialPort serialPort) throws IOException {
        String s;
        byte[] buffer = new byte[30];
        while (serialPort.bytesAvailable() < 30) {
            //System.out.println(port.bytesAvailable());
        }
        serialPort.readBytes(buffer, 30);
        s = new String(buffer);
        System.out.println("ELE: " + s);
    }

    public static void sendTextFile(SerialPort serialPort) throws IOException {
        int j=0;
        int i=0;
        int tinc=0;
        long t = 0;
        int nTramas;
        int arraySize;
        byte[] fileBytes;
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        Files.copy(Paths.get("C:\\Users\\Utilizador\\Desktop\\Fase4\\Ficheiros\\Enviar\\hino.txt"), out);
        fileBytes = out.toByteArray();
        arraySize=fileBytes.length;

        nTramas= (int) ((fileBytes.length/30.0)+1);
        while(i<nTramas) {
            byte[] bytesToSend = new byte[30];
            while(j < 30) {
                if (j + tinc == arraySize) {
                    break;
                }
                bytesToSend[j] = fileBytes[j + tinc];
                j++;
            }
            j=0;
            tinc = tinc + 30;
            t+= System.currentTimeMillis();
            serialPort.writeBytes(bytesToSend, 30);//Envia para a porta série o array de bytes

            i++;
        }
        System.out.println("Tempo total de envio do ficheiro de texto: "+(t*Math.pow(10,6))+"s");

    }
    public static void sendImage(SerialPort serialPort) throws IOException, InterruptedException {
        int j=0;
        int i=0;
        int tinc=0;
        int nTramas;
        int arraySize;
        byte[] fileBytes;
        byte[] conf=new byte[1];
        boolean next=false;
        long t=0;
        //FileBytes= Files.readAllBytes(Paths.get("C:\\Users\\ricar\\OneDrive\\Área de Trabalho\\DEMO INTERMEDIA\\Enviar\\hino.txt"));
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        Files.copy(Paths.get("C:\\Users\\Utilizador\\Desktop\\Fase4\\Ficheiros\\Enviar\\aviao.jpg"), out);
        fileBytes = out.toByteArray();
        arraySize=fileBytes.length;
        nTramas= (int) ((fileBytes.length/30.0)+1);
        while(i<nTramas) {
            Thread.sleep(1);
            next=false;
            conf[0]=' ';
            byte[] bytesToSend = new byte[30];
            while(j < 30) {
                if (j + tinc == arraySize) {
                    serialPort.writeBytes(bytesToSend,20);
                    break;
                }
                bytesToSend[j] = fileBytes[j + tinc];
                j++;
            }
            j=0;
            tinc = tinc + 30;
            t+= System.currentTimeMillis();
            serialPort.writeBytes(bytesToSend, 30);//Envia para a porta série o array de bytes
            while(!next){
                serialPort.readBytes(conf,1);
                if(conf[0]=='A'){
                    next=true;
                }
            }

            i++;


        }
        System.out.println("Tempo total de envio da imagem: "+(t*Math.pow(10,6))+" s");
    }

    public static void main(String[] Args) throws IOException, InterruptedException {
        
        SerialPort[] portas = SerialPort.getCommPorts();//coloca as portas disponiveis num array de portas
        if (portas.length == 0) {
            System.out.println("\nNao existem portas disponiveis\n");
        }
        System.out.println("Escolha a porta que deseja usar: \n");
        int i = 1;
        
        for (SerialPort porta : portas) {
            System.out.println(i++ + " - " + porta.getSystemPortName()); //mostra ao utilizador as portas
        }
        Scanner in = new Scanner(System.in);
        int numeroPorta = in.nextInt() - 1;
        SerialPort porta = portas[numeroPorta];//seleciona a porta desejada pelo utilizador

        porta.openPort(); //abre a porta para efectuar leitura ou escrita

        if (porta.isOpen()) {
            System.out.println("Porta COM aberta \n");
        } else {
            System.out.println("Porta COM fechada\n");
        }
        porta.setComPortParameters(115200, 8, 1, 0);       //definir parametros da portaCom

        int op;    //Menu de opções
        System.out.println("-----------------------------------------------------");
        System.out.println("| Emulação e Simulação de Redes de Telecomunicações |");
        System.out.println("|                     Fase 4                        |");
        System.out.println("|                                                   |");
        System.out.println("|             1- Enviar ficheiro de Texto           |");
        System.out.println("|             2- Enviar Imagem                      |");
        System.out.println("|             3- Chat                               |");
        System.out.println("|             3- Sair                               |");
        System.out.println("|                                                   |");
        System.out.println("-----------------------------------------------------");

        Scanner in1 = new Scanner(System.in);
        op = in1.nextInt();
        boolean check=true;

        switch (op) {
            case 1:
                sendTextFile(porta);
                break;
            case 2:
                sendImage(porta);
                break;
            case 3:
                while (check) {
                    check=sendMessage(porta);
                    if(check){
                        readMessage(porta);
                    }

                }
            case 4:
                break;
        }
    }
}