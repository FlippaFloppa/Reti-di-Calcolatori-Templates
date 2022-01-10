public class ThreadTcp {

    private Socket clientSocket = null;

    /**
     * Constructor
     * 
     * @param clientSocket
     */
    public ThreadTcp(Socket clientSocket) {
        this.clientSocket = clientSocket;
    }

    public void run() {
        DataInputStream inSock;
        DataOutputStream outSock;
        try {
            String line;
            try {
                // creazione stream di input e out da socket
                inSock = new DataInputStream(clientSocket.getInputStream());
                outSock = new DataOutputStream(clientSocket.getOutputStream());
                line = inSock.readUTF();
            } catch (SocketTimeoutException ste) {
                System.out.println("Timeout scattato: ");
                ste.printStackTrace();
                clientSocket.close();
                return;
            } catch (IOException ioe) {
                System.out
                        .println("Problemi nella creazione degli stream di input/output "
                                + "su socket: ");
                ioe.printStackTrace();
                // il server continua l'esecuzione riprendendo dall'inizio del ciclo
                return;
            } catch (Exception e) {
                System.out
                        .println("Problemi nella creazione degli stream di input/output "
                                + "su socket: ");
                e.printStackTrace();
                return;
            }

            FileOutputStream outFile = null;
            String esito;
            // file check
            if (line == null) {
                System.out.println("Problemi nella ricezione: ");
                clientSocket.close();
                return;
            }

            /*
             * 
             * ESEMPIO RICEZIONE NOME FILE
             * 
             * else {
             * File curFile = new File(nomeFile);
             * if (curFile.exists()) {
             * try {
             * esito = "Sovrascritto file esistente";
             * // distruggo il file da sovrascrivere
             * curFile.delete();
             * }
             * catch (Exception e) {
             * System.out.println("Problemi nella notifica di file esistente: ");
             * e.printStackTrace();
             * return;
             * }
             * } else esito = "Creato nuovo file";
             * outFile = new FileOutputStream(nomeFile);
             * }
             */

            // ciclo di ricezione dal client, salvataggio file e stamapa a video
            try {
                /*
                 * ESEMPIO RICEZIONE FILE
                 * 
                 * System.out.println("Ricevo il file " + nomeFile + ": \n");
                 * FileUtility.trasferisci_a_byte_file_binario(inSock,
                 * new DataOutputStream(outFile));
                 * System.out.println("\nRicezione del file " + nomeFile + " terminata\n");
                 * // chiusura file
                 * outFile.close();
                 */
                clientSocket.shutdownInput(); // chiusura socket (downstream)
                outSock.writeUTF(esito);
                outSock.flush();
                clientSocket.shutdownOutput(); // chiusura socket (dupstream)
                System.out.println("\nTerminata connessione con " + clientSocket);
                clientSocket.close();
            } catch (SocketTimeoutException ste) {
                System.out.println("Timeout scattato: ");
                ste.printStackTrace();
                clientSocket.close();
                return;
            } catch (Exception e) {
                System.err
                        .println("\nProblemi durante la ricezione e scrittura del file: "
                                + e.getMessage());
                e.printStackTrace();
                clientSocket.close();
                System.out.println("Terminata connessione con " + clientSocket);
                return;
            }
        }
        // qui catturo le eccezioni non catturate all'interno del while
        // in seguito alle quali il server termina l'esecuzione
        catch (Exception e) {
            e.printStackTrace();
            System.out
                    .println("Errore irreversibile, ThreadTcp: termino...");
            System.exit(3);
        }
    }

}
