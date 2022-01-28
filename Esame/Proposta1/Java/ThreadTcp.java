import java.io.*;
import java.net.Socket;
import java.net.SocketTimeoutException;

public class ThreadTcp extends Thread {

    private Socket clientSocket = null;

    public ThreadTcp(Socket clientSocket) {
        this.clientSocket = clientSocket;
    }

    public void run() {
        DataInputStream inSock;
        DataOutputStream outSock;
        try {
            try {
                // creazione stream di input e out da socket
                inSock = new DataInputStream(clientSocket.getInputStream());
                outSock = new DataOutputStream(clientSocket.getOutputStream());

            } catch (SocketTimeoutException ste) {
                System.out.println("Timeout scattato: ");
                ste.printStackTrace();
                clientSocket.close();
                return;
            } catch (IOException ioe) {
                System.out
                        .println("Problemi nella creazione degli stream di input/output: ");
                ioe.printStackTrace();
                return;
            } catch (Exception e) {
                System.out
                        .println("Problemi nella creazione degli stream di input/output su socket: ");
                e.printStackTrace();
                return;
            }

            String op1 = "A";
            String op2 = "B";
            String line, echo;

            while ((line = inSock.readUTF()) != null) {

                if (op1.compareTo(line) == 0) {
                    // echo = inSock.readUTF();
                    // outSock.writeUTF(echo);
                }

                if (op2.compareTo(line) == 0) {

                }

            }
            clientSocket.shutdownOutput();
            clientSocket.shutdownInput();
            this.clientSocket.close();
            inSock.close();
            outSock.close();
        }
        // qui catturo le eccezioni non catturate all'interno del while
        // in seguito alle quali il server termina l'esecuzione
        catch (SocketTimeoutException ste) {
            System.out.println("Timeout scattato: ");
            ste.printStackTrace();
            return;
        } catch (EOFException e) {
            System.out.println("Figlio termina...");
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Errore irreversibile, ThreadTcp: termino...");
        }
    }

}
