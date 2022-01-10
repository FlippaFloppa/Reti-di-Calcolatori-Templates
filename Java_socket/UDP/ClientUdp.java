import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.StringTokenizer;

public class ClientUdp {

	private static final int buffer = 256;

	public static void main(String[] args) {

		InetAddress addr = null;
		int port = -1;

		try {
			if (args.length == 2) {
				addr = InetAddress.getByName(args[0]);
				port = Integer.parseInt(args[1]);
			} else {
				// Modifica secondo le specifiche
				System.out.println("Usage: java ClientUdp serverIP serverPort");
				System.exit(1);
			}
		} catch (UnknownHostException e) {
			System.out
					.println("Problemi nella determinazione dell'endpoint del server : ");
			e.printStackTrace();
			System.out.println("ClientUdp: interrompo...");
			System.exit(2);
		}

		DatagramSocket socket = null;
		DatagramPacket packet = null;
		byte[] buf = new byte[buffer];

		// creazione della socket datagram, settaggio timeout di 30s
		// e creazione datagram packet
		try {
			socket = new DatagramSocket();
			// socket.setSoTimeout(30000); OPZIONALE
			packet = new DatagramPacket(buf, buf.length, addr, port);
			System.out.println("\nClientUdp: avviato");
			System.out.println("Creata la socket: " + socket);
		} catch (SocketException e) {
			System.out.println("Problemi nella creazione della socket: ");
			e.printStackTrace();
			System.out.println("ClientUdp: interrompo...");
			System.exit(1);
		}

		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		// --------------------------------
		// Modifca questa stringa secondo le specifiche
		// -------------------------------------
		String operazione = "\nINSERIRE OPERAZIONE\n^D(Unix)/^Z(Win)+invio per uscire";

		System.out
				.print(operazione);

		try {
			ByteArrayOutputStream boStream = null;
			DataOutputStream doStream = null;
			byte[] data = null;
			String line = null;
			String richiesta = null;
			String risposta = null;
			ByteArrayInputStream biStream = null;
			DataInputStream diStream = null;

			while ((line = stdIn.readLine()) != null) {

				// ----------------------------------
				// Inserire operazioni sull'input
				// ------------------------------
				richiesta = line; // esempio

				// riempimento e invio del pacchetto
				try {
					boStream = new ByteArrayOutputStream();
					doStream = new DataOutputStream(boStream);
					doStream.writeUTF(richiesta);
					data = boStream.toByteArray();
					packet.setData(data);
					socket.send(packet);
					System.out.println("Richiesta inviata a " + addr + ", " + port);
				} catch (IOException e) {
					System.out.println("Problemi nell'invio della richiesta: ");
					e.printStackTrace();
					System.out
							.print(operazione);
					continue;
					// il client continua l'esecuzione riprendendo dall'inizio del ciclo
				}

				try {
					// settaggio del buffer di ricezione
					packet.setData(buf);
					socket.receive(packet);
					// sospensiva solo per i millisecondi indicati, dopodich� solleva una
					// SocketException
				} catch (IOException e) {
					System.out.println("Problemi nella ricezione del datagramma: ");
					e.printStackTrace();
					System.out
							.print(operazione);
					continue;
					// il client continua l'esecuzione riprendendo dall'inizio del ciclo
				}
				try {
					biStream = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
					diStream = new DataInputStream(biStream);
					risposta = diStream.readUTF();

					// -----------------------------
					// Modificare per eventuali operazioni sulla risposta
					// -----------------------------
					System.out.println("Risposta: " + risposta);
				} catch (IOException e) {
					System.out.println("Problemi nella lettura della risposta: ");
					e.printStackTrace();
					System.out
							.print(operazione);
					continue;
					// il client continua l'esecuzione riprendendo dall'inizio del ciclo
				}

				// tutto ok, pronto per nuova richiesta
				System.out
						.print(operazione);
			} // while
		}
		// qui catturo le eccezioni non catturate all'interno del while
		// in seguito alle quali il client termina l'esecuzione
		catch (Exception e) {
			e.printStackTrace();
		}

		System.out.println("ClientUdp: termino...");
		socket.close();
	}

}
