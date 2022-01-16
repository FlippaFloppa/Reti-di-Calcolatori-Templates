//Venerandi Lorenzo 0000914987
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.net.Socket;
import java.net.SocketTimeoutException;

public class ThreadTcp extends Thread {

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
        String esito,dir;
        File[] files;
        FileInputStream inFile;
        int occorrenze,result,numFile,count;
        String tmp;
        boolean state;
        BufferedReader src;

        try {
            String line;
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
                System.out.println("Problemi nella creazione degli stream di input/output " + "su socket: ");
                ioe.printStackTrace();
                // il server continua l'esecuzione riprendendo dall'inizio del ciclo
                return;
            } catch (Exception e) {
                System.out.println("Problemi nella creazione degli stream di input/output " + "su socket: ");
                e.printStackTrace();
                return;
            }

            // Leggo le operazioni
            while ((line = inSock.readUTF()) != null) {

                if (line.compareTo("C") == 0) {

                    result=0;

                    // Ricevo carattere da cercare
                    line = inSock.readUTF();
                    // Ricevo numero occorrenze
                    occorrenze=inSock.readInt();

                    files=new File(".").listFiles();
                    
                    for(File f:files){
                            
                        if(f.getName().contains(".txt")){

                            System.out.println("Esploro file "+f.getName());

                            src=new BufferedReader(new FileReader(f));
                            try{
                                while((tmp=src.readLine())!=null){

                                    count=0;
                                    //if(line.toUpperCase().compareTo(tmp.charAt(0)+"")==0){
                                        for(int i=0;i<tmp.length();i++){
                                            if(line.compareTo(tmp.charAt(i)+"")==0){
                                                count++;
                                            }
                                        }
                                    //}
                                    if(count>=occorrenze)result++;
                                }
                            }
                            //fine file
                            catch(Exception e){
                                src.close();
                            }
                        }
                    }

                    try {
                        outSock.writeInt(result);
                        outSock.flush();
                    }catch (Exception e) {
                        System.err.println("\nProblemi durante la ricezione e scrittura del file: " + e.getMessage());
                        e.printStackTrace();
                        clientSocket.close();
                        System.out.println("Terminata connessione con " + clientSocket);
                        return;
                    }

                } else if (line.compareTo("D") == 0) {

                    // Ricevo directory
                    line = inSock.readUTF();
                    numFile=0;
                    
                    dir=line;
                    files=new File(dir).listFiles();

                    if(files==null){
                        //esito negativo
                        outSock.writeInt(-1);
                    }
                    else{

                        //Conto numero files binari
                        for(File f:files){
                            if(!f.getName().contains(".txt")){
                                numFile++;
                            }
                        }
                        System.out.println("Trovati "+numFile);
                        outSock.writeInt(numFile);

                        for(File f:files){

                            if(!f.getName().contains(".txt")){
                                // Invio nome file
                                outSock.writeUTF(f.getName());
                                System.out.println("Invio file "+f.getName());
    
                                inFile=new FileInputStream(f);
                                FileUtility.trasferisci_a_byte_file_binario(new DataInputStream(inFile), outSock);
                                inFile.close();
                            }
                            
                        }

                        System.out.println("Invio file terminato");
                    }
                }
            }
        }
        // qui catturo le eccezioni non catturate all'interno del while
        // in seguito alle quali il server termina l'esecuzione
        catch(EOFException e){
            System.out.println("Connessione terminata, ThreadTcp: termino...");
            return;
        }
        catch (Exception e) {
            e.printStackTrace();
            System.out.println("Errore irreversibile, ThreadTcp: termino...");
            return;
        }
    }

}
