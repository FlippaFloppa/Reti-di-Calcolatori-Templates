//Venerandi Lorenzo 0000914987

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
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
		} // try
			// Per esercizio si possono dividere le diverse eccezioni
		catch (Exception e) {
			System.out.println("Problemi, i seguenti: ");
			e.printStackTrace();
			System.out.println("Usage: java ClientTcp serverAddr serverPort");
			System.exit(2);
		}

		// oggetti utilizzati dal client per la comunicazione e la lettura del file
		// locale
		Socket socket = null;
		FileOutputStream outFile = null;
		DataInputStream inSock = null;
		DataOutputStream outSock = null;
		String line = null,nomeFile;
		int numFile,occorrenze;

		// creazione stream di input da tastiera
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		String operation = "\n^D(Unix)/^Z(Win)+invio per uscire\nC) Conteggio linee\nD) Download file binari\n";

		try {
			// creazione socket
			try {
				socket = new Socket(addr, port);
				// socket.setSoTimeout(30000); OPZIONALE
				System.out.println("Creata la socket: " + socket);
			} catch (Exception e) {
				System.out.println("Problemi nella creazione della socket: ");
				e.printStackTrace();
			}

			// creazione stream di input/output su socket
			try {
				inSock = new DataInputStream(socket.getInputStream());
				outSock = new DataOutputStream(socket.getOutputStream());
			} catch (IOException e) {
				System.out.println("Problemi nella creazione degli stream su socket: ");
				e.printStackTrace();
			}

			System.out.print(operation);
			while ((line = stdIn.readLine()) != null) {

				if (line.compareTo("C") == 0) {

					// trasmissione operazione
					try {
						outSock.writeUTF(line);
						System.out.println("Inviato " + line);
					} catch (Exception e) {
						System.out.println("Problemi nell'invio di " + line + ": ");
						e.printStackTrace();
						continue;
					}

					System.out.print("Inserisci il carattere per il controllo: ");
					line = stdIn.readLine();

					// trasmissione targa
					try {
						outSock.writeUTF(line.trim());
						System.out.println("Inviato " + line);
					} catch (Exception e) {
						System.out.println("Problemi nell'invio di " + line + ": ");
						e.printStackTrace();
						continue;
					}

					System.out.print("Inserisci il numero minimo di occorrenze: ");
					line = stdIn.readLine();
					occorrenze=Integer.parseInt(line.trim());

					// trasmissione targa
					try {
						outSock.writeInt(occorrenze);
						System.out.println("Inviato " + occorrenze);
					} catch (Exception e) {
						System.out.println("Problemi nell'invio di " + occorrenze + ": ");
						e.printStackTrace();
						continue;
					}

					// ricezione esito
					int esito;
					try {
						esito = inSock.readInt();
						if(esito>=0){
							System.out.println("Numero di occorrenze trovate: "+esito);
						}
					} catch (SocketTimeoutException ste) {
						System.out.println("Timeout scattato: ");
						ste.printStackTrace();
						continue;
					} catch (Exception e) {
						System.out.println("Problemi nella ricezione dell'esito, i seguenti: ");
						e.printStackTrace();
						continue;
					}

				} else if (line.compareTo("D") == 0) {

					// trasmissione operazione
					try {
						outSock.writeUTF(line);
						System.out.println("Inviato " + line);
					} catch (Exception e) {
						System.out.println("Problemi nell'invio di " + line + ": ");
						e.printStackTrace();
						continue;
					}

					System.out.print("Inserisci la directory per il download: ");
					line = stdIn.readLine();

					// trasmissione targa
					try {
						outSock.writeUTF(line);
						System.out.println("Inviato " + line);
					} catch (Exception e) {
						System.out.println("Problemi nell'invio di " + line + ": ");
						e.printStackTrace();
						continue;
					}

					//ricevo i files
					numFile=inSock.readInt();

					if(numFile<0){
						System.out.println("Files non individuati");
					}
					else{
						for(int i=0;i<numFile;i++){

							// Leggo nome file
							nomeFile=inSock.readUTF();
							System.out.println("Ricevo file "+nomeFile);

							outFile=new FileOutputStream(nomeFile);
							FileUtility.trasferisci_a_byte_file_binario(inSock, new DataOutputStream(outFile));
							outFile.close();
						}
					}


				} else {
					System.out.println("Servizio non riconosciuto!");
				}

				// tutto ok, pronto per nuova richiesta
				System.out.print(operation);

			}
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