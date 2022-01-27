import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketTimeoutException;

public class ClientTcp {
    public static void main(String[] args) throws IOException {

        InetAddress addr = null;
        int port = -1;

        try { // check args
            if (args.length == 2) {
                addr = InetAddress.getByName(args[0]);
                port = Integer.parseInt(args[1]);
            } else {
                System.out.println("Usage: java ClientTcp serverAddr serverPort");
                System.exit(1);
            }
        } catch (Exception e) {
            System.out.println("Problemi, i seguenti: ");
            e.printStackTrace();
            System.out.println("Usage: java ClientTcp serverAddr serverPort");
            System.exit(2);
        }

        // oggetti utilizzati dal client per la comunicazione
        Socket socket = null;
        DataInputStream inSock = null;
        DataOutputStream outSock = null;

        // creazione stream di input da tastiera
        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

        String op1 = "A";
        String op2 = "B";

        String operation = "\n^D(Unix)/^Z(Win)+invio per uscire\n Inserisci: " + op1 + " o " + op2
                + System.lineSeparator();

        // creazione socket
        try {
            socket = new Socket(addr, port);
            socket.setSoTimeout(30000);
            System.out.println("Creata la socket: " + socket);
        } catch (Exception e) {
            System.out.println("Problemi nella creazione della socket: ");
            e.printStackTrace();
            System.exit(-1);
        }

        // creazione stream di input/output su socket
        try {
            inSock = new DataInputStream(socket.getInputStream());
            outSock = new DataOutputStream(socket.getOutputStream());
        } catch (IOException e) {
            System.out
                    .println("Problemi nella creazione degli stream su socket: ");
            e.printStackTrace();
            System.exit(-1);
        }

        System.out.print(operation);
        String line;

        try {
            while ((line = stdIn.readLine()) != null) {

                if (line.compareTo(op1) == 0 || line.compareTo(op2) == 0) {

                    // trasmissione
                    try {
                        outSock.writeUTF(line);
                        System.out.println("Inviato " + line);
                    } catch (Exception e) {
                        System.out.println("Problemi nell'invio di " + line);
                        e.printStackTrace();
                        System.out.print(operation);
                        // il client continua l'esecuzione riprendendo dall'inizio del ciclo
                        continue;
                    }

                    if (line.compareTo(op1) == 0) {

                        // System.out.println("Hai inserito " + op1);
                        // System.out.println("Inserisci echo");
                        // String echo = stdIn.readLine();
                        // outSock.writeUTF(echo);
                        // try {
                        // System.out.println("Ho ricevuto " + inSock.readUTF());
                        // } catch (Exception e) {
                        // continue;
                        // }
                    }

                    if (line.compareTo(op2) == 0) {
                        System.out.println("Hai inserito " + op2);

                    }

                }

                // tutto ok, pronto per nuova richiesta
                System.out.print(operation);

            }
            socket.shutdownInput();
            socket.shutdownOutput();
            socket.close();
            System.out.println("ClientTcp: termino...");
        }
        // qui catturo le eccezioni non catturate all'interno del while
        // quali per esempio la caduta della connessione con il server
        // in seguito alle quali il client termina l'esecuzione
        catch (Exception e) {
            System.err.println("Errore irreversibile, il seguente: ");
            e.printStackTrace();
            System.err.println("Chiudo!");
            System.exit(3);
        }
    }
}