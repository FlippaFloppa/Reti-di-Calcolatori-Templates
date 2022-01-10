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
		FileInputStream inFile = null;
		DataInputStream inSock = null;
		DataOutputStream outSock = null;
		String line = null;

		// creazione stream di input da tastiera
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		String operation = "\n^D(Unix)/^Z(Win)+invio per uscire, OPERAZIONE: ";
		System.out
				.print(operation);

		try {
			while ((line = stdIn.readLine()) != null) {
				// se il file esiste creo la socket

				// creazione socket
				try {
					socket = new Socket(addr, port);
					// socket.setSoTimeout(30000); OPZIONALE
					System.out.println("Creata la socket: " + socket);
				} catch (Exception e) {
					System.out.println("Problemi nella creazione della socket: ");
					e.printStackTrace();
					System.out
							.print(operation);
					continue;
					// il client continua l'esecuzione riprendendo dall'inizio del ciclo
				}

				// creazione stream di input/output su socket
				try {
					inSock = new DataInputStream(socket.getInputStream());
					outSock = new DataOutputStream(socket.getOutputStream());
				} catch (IOException e) {
					System.out
							.println("Problemi nella creazione degli stream su socket: ");
					e.printStackTrace();
					System.out
							.print(operation);
					continue;
					// il client continua l'esecuzione riprendendo dall'inizio del ciclo
				}

				// -------------------------------------------------
				// Inserire operazioni/modifiche sulla trasmissione
				// -------------------------------------------------

				// trasmissione
				try {
					outSock.writeUTF(line);
					System.out.println("Inviato " + line);
				} catch (Exception e) {
					System.out.println("Problemi nell'invio di " + line
							+ ": ");
					e.printStackTrace();
					System.out
							.print(operation);
					// il client continua l'esecuzione riprendendo dall'inizio del ciclo
					continue;
				}

				/*
				 * 
				 * ESEMPIO TRASFERIMENTO FILE CLIENT->SERVER
				 * 
				 * System.out.println("Inizio la trasmissione di " + nomeFile);
				 * 
				 * // trasferimento file
				 * try {
				 * // FileUtility.trasferisci_a_linee_UTF_e_stampa_a_video(new
				 * // DataInputStream(inFile), outSock);
				 * FileUtility.trasferisci_a_byte_file_binario(new DataInputStream(inFile),
				 * outSock);
				 * inFile.close(); // chiusura file
				 * socket.shutdownOutput(); // chiusura socket in upstream, invio l'EOF al
				 * server
				 * System.out.println("Trasmissione di " + nomeFile + " terminata ");
				 * } catch (SocketTimeoutException ste) {
				 * System.out.println("Timeout scattato: ");
				 * ste.printStackTrace();
				 * socket.close();
				 * System.out
				 * .print(operation);
				 * // il client continua l'esecuzione riprendendo dall'inizio del ciclo
				 * continue;
				 * } catch (Exception e) {
				 * System.out.println("Problemi nell'invio di " + nomeFile + ": ");
				 * e.printStackTrace();
				 * socket.close();
				 * System.out
				 * .print(operation);
				 * // il client continua l'esecuzione riprendendo dall'inizio del ciclo
				 * continue;
				 * }
				 * 
				 */

				// ricezione esito
				String esito;
				try {
					esito = inSock.readUTF();
					System.out.println("Esito trasmissione: " + esito);
					// chiudo la socket in downstream
					socket.shutdownInput();
					System.out.println("Terminata la chiusura della socket: " + socket);
				} catch (SocketTimeoutException ste) {
					System.out.println("Timeout scattato: ");
					ste.printStackTrace();
					socket.close();
					System.out
							.print(operation);
					// il client continua l'esecuzione riprendendo dall'inizio del ciclo
					continue;
				} catch (Exception e) {
					System.out
							.println("Problemi nella ricezione dell'esito, i seguenti: ");
					e.printStackTrace();
					socket.close();
					System.out
							.print(operation);
					continue;
					// il client continua l'esecuzione riprendendo dall'inizio del ciclo
				}

				// tutto ok, pronto per nuova richiesta
				System.out
						.print(operation);

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