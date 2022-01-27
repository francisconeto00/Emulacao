import com.fazecast.jSerialComm.SerialPort;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Scanner;

public class Recetor {

    public static void sendMessage(SerialPort serialPort) throws IOException {
        byte[] messageBytes = new byte[30];
        System.out.print("EU: ");
        Scanner input = new Scanner(System.in);
        String msg = input.nextLine();
        messageBytes = msg.getBytes();
        serialPort.writeBytes(messageBytes,messageBytes.length);
    }

    public static void receiveTextFILEFromPort(SerialPort serialPort) throws IOException {

        byte[] buffer = new byte[385];
        while(serialPort.bytesAvailable()<385){ //espera pelo fim do envio dos bytes todos
            System.out.println(serialPort.bytesAvailable());
        }
        serialPort.readBytes(buffer, 385);// recebe os bytes e coloca no buffer
        System.out.println(Arrays.toString(buffer));
        Path path = Paths.get("C:\\Users\\Utilizador\\Desktop\\Fase4\\Ficheiros\\Recebido\\HINORecebido.txt"); //path do destino
        if(Files.exists(path)){ //se existir elimina
            Files.delete(path);
        }
        Files.createFile(path);//se nao existir cria
        Files.write(path,buffer);//escreve o ficheiro
    }
    public static void receiveImageFromPort(SerialPort serialPort) throws IOException {

        byte[] buffer = new byte[5120];
        while(serialPort.bytesAvailable()<5120){ //espera pelo fim do envio dos bytes todos
            System.out.println(serialPort.bytesAvailable());
        }
        serialPort.readBytes(buffer, 5120);// recebe os bytes e coloca no buffer
       // System.out.println(Arrays.toString(buffer));
        Path path = Paths.get("C:\\Users\\Utilizador\\Desktop\\Fase4\\Ficheiros\\Recebido\\AviãoRecebido.jpg");
        if(Files.exists(path)){ //se existir elimina
            Files.delete(path);
        }
        Files.createFile(path);//se nao existir cria
        Files.write(path,buffer);//escreve o ficheiro
        System.out.println("Ficheiro recebido");
    }

    public static void readMessage(SerialPort serialPort) throws IOException {
        String str;
        byte[] buffer = new byte[30];
        while (serialPort.bytesAvailable() < 30) {
            //System.out.println(serialPort.bytesAvailable());
        }
        serialPort.readBytes(buffer, 30);
        String s = new String(buffer);
        System.out.println("ELE:" + s);
    }

    public static void main(String[] Args) throws IOException,InterruptedException {

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
        porta.setComPortParameters(115200, 8, 1, 0);       //defenir parametros da portaCom

        int op;    //Menu de opções
        System.out.println("-----------------------------------------------------");
        System.out.println("| Emulação e Simulação de Redes de Telecomunicações |");
        System.out.println("|                     Fase 4                        |");
        System.out.println("|                                                   |");
        System.out.println("|             1- Receber Ficheiro de Texto          |");
        System.out.println("|             2- Receber Imagem                     |");
        System.out.println("|             3- Chat                               |");
        System.out.println("|             4- Sair                               |");
        System.out.println("|                                                   |");
        System.out.println("-----------------------------------------------------");

        Scanner in1 = new Scanner(System.in);
        op = in1.nextInt();
        boolean isFile;

        switch (op) {
            case 1:
                receiveTextFILEFromPort(porta);
                break;
            case 2:
                receiveImageFromPort(porta);
                break;
            case 3:
                while (true)
                {
                    readMessage(porta);
                    sendMessage(porta);
                }
            case 4:
                break;
        }
    }
}
