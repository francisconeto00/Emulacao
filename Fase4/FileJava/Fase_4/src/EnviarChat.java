
import com.fazecast.jSerialComm.SerialPort;

import javax.swing.*;
import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;


public class EnviarChat {

    public static void sendPort(SerialPort serialPort, String mensagem) throws IOException, InterruptedException {
        int i = 0;
        int j = 0;
        int x = 0;
        byte[] bytesFromMessage;
        byte[] bytesToSend;
        float nPac;
        int nTramas;
        bytesFromMessage = mensagem.getBytes();
        nPac = (float) Math.ceil(((bytesFromMessage.length)/30.0));
        nTramas= (int) nPac;
        while(i<nTramas) {
            bytesToSend = new byte[31];
            if (i == (nTramas - 1)) {
                bytesToSend[0] = (byte) 0b0110010;
            } else {
                bytesToSend[0] = (byte) 0b00110001;
            }
            while  (j < 30) {
                if (j + x == bytesFromMessage.length) {
                    break;
                }
                bytesToSend[j + 1] = bytesFromMessage[j + x];
                j++;
            }
            x = x + 30; //Increment to next package
            serialPort.writeBytes(bytesToSend, 31);//Send to Serial Port the type byte array
            byte[] bufferR = new byte[2];
            while (serialPort.bytesAvailable() < 2000000) {//Espera pelo envio do último byte
                serialPort.readBytes(bufferR, 2);
                if (bufferR[0] == (-114) && bufferR[1] == (-114)) {
                    break;
                }
            }
            i++;
        }
    }
    public static void receiveFromPort(SerialPort serialPort){
        byte[] buffer = new byte[30];
        while (serialPort.bytesAvailable() < 30) {
            System.out.println(serialPort.bytesAvailable());
        }
        serialPort.readBytes(buffer, 30);
        System.out.println(Arrays.toString(buffer));
    }

    public static void main(String[] args) throws InterruptedException, IOException {

        SerialPort[] portas=SerialPort.getCommPorts();//coloca as portas disponiveis num array de portas
        if(portas.length==0){
            System.out.println("\nNao existem portas disponiveis!\n");
        }
        System.out.println("Escolha a porta que deseja usar: \n");
        int i = 1;
        int y=0;
        for(SerialPort porta : portas){
            System.out.println(i++ + " - " + porta.getSystemPortName()); //mostra ao utilizador as portas
        }
        Scanner in= new Scanner(System.in);
        int numeroPorta= in.nextInt()-1;
        SerialPort porta = portas[numeroPorta];//seleciona a porta desejada pelo utilizador
        porta.openPort(); //abre a porta para efectuar leitura ou escrita
        if(porta.isOpen()){
            System.out.println("Porta COM aberta \n");
        }else {
            System.out.println("Porta COM fechada\n");
        }
        porta.setComPortParameters(115200,8,1,0);       //defenir parametros da portaCom

        System.out.println("Introduza uma mensagem para enviar:\n");
        String mensagem = in.nextLine();
        System.out.println(mensagem);
        //Dá merda aqui!!! Pra testar, poe um terminal a receber primeiro e depois a enviar , e depois outro terminal a enviar e depois receber
        sendPort(porta,mensagem);
        while(true) {
            receiveFromPort(porta);
        }
    }

}
