import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class MultiThreadServer {
    public static final int PORT = 1050; // default port

    public static void main(String[] args) throws IOException {

        int port = -1;

        // controllo argomenti
        try {
            if (args.length == 1) {
                port = Integer.parseInt(args[0]);
                if (port < 1024 || port > 65535) {
                    System.out.println("Usage: java MultiThreadServer [serverPort>1024]");
                    System.exit(1);
                }
            } else if (args.length == 0) {
                port = PORT;
            } else {
                System.out.println("Usage: java MultiThreadServer or java MultiThreadServer port");
                System.exit(1);
            }
        } // try
        catch (Exception e) {
            System.out.println("Problemi, i seguenti: ");
            e.printStackTrace();
            System.out.println("Usage: java MultiThreadServer or java MultiThreadServer port");
            System.exit(1);
        }

        ServerSocket serverSocket = null;
        Socket clientSocket = null;

        try {
            serverSocket = new ServerSocket(port);
            serverSocket.setReuseAddress(true);
            System.out.println("MultiThreadServer: avviato ");
            System.out.println("Server: creata la server socket: " + serverSocket);
        } catch (Exception e) {
            System.err
                    .println("Server: problemi nella creazione della server socket: "
                            + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        }

        try {

            while (true) {
                System.out.println("Server: in attesa di richieste...\n");

                try {
                    clientSocket = serverSocket.accept();
                    clientSocket.setSoTimeout(30000);
                    System.out.println("Server: connessione accettata: " + clientSocket);
                } catch (Exception e) {
                    System.err
                            .println("Server: problemi nella accettazione della connessione: "
                                    + e.getMessage());
                    e.printStackTrace();
                    continue;
                }

                // serizio delegato ad un nuovo thread
                try {
                    new ThreadTcp(clientSocket).start();
                } catch (Exception e) {
                    System.err.println("Server: problemi nel server thread: "
                            + e.getMessage());
                    e.printStackTrace();
                    continue;
                }

            } // while
        }
        // qui catturo le eccezioni non catturate all'interno del while
        // in seguito alle quali il server termina l'esecuzione
        catch (Exception e) {
            e.printStackTrace();
            // chiusura di stream e socket
            System.out.println("MultiThreadServer: termino...");
            System.exit(2);
        }

    }
}
