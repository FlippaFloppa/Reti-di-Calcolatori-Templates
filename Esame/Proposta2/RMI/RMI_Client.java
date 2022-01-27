import java.io.*;
import java.rmi.*;

public class RMI_Client {

	private static final int DIM = 5;

	public static void main(String[] args) {

		final int REGISTRYPORT = 1099;
		String registryHost = null; // host remoto con registry
		String serviceName = "";

		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
		Dati dati[] = new Dati[DIM];

		// Controllo dei parametri della riga di comando
		if (args.length != 2) {
			System.out.println("Sintassi: RMI_Registry_IP ServiceName");
			System.exit(1);
		}
		registryHost = args[0];
		serviceName = args[1];

		System.out.println("Invio richieste a " + registryHost + " per il servizio di nome " + serviceName);

		// Connessione al servizio RMI remoto
		try {
			String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
			RMI_interfaceFile serverRMI = (RMI_interfaceFile) Naming.lookup(completeName);
			System.out.println("ClientRMI: Servizio \"" + serviceName + "\" connesso");

			System.out.println("\nRichieste di servizio fino a fine file");

			String service;
			String op1 = "A";
			String op2 = "B";
			String richiesta = "Servizio (P1: " + op1 + ") (P2: " + op2 + ")";
			System.out.println(richiesta);

			while ((service = stdIn.readLine()) != null) {

				if (service.equals(op1)) {

					System.out.println("Inserisci richiesta");
					String line = stdIn.readLine();
					int res = serverRMI.elimina_dati(line);
					System.out.println("Result: " + res);

				} else if (service.equals(op2)) {

					System.out.println("Inserisci richiesta");
					String line = stdIn.readLine();
					dati = serverRMI.visualizza_dati(line);
					System.out.println("Result: " + dati[0].toString());

				} else {
					System.out.println("Servizio non disponibile");
				}
				System.out.println(richiesta);
			}

		} catch (NotBoundException nbe) {
			System.err.println("ClientRMI: il nome fornito non risulta registrato; " + nbe.getMessage());
			nbe.printStackTrace();
			System.exit(1);
		} catch (Exception e) {
			System.err.println("ClientRMI: " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}

}