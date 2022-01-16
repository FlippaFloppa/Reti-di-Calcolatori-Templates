import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;
import java.rmi.NotBoundException;

public class RMI_Client {

    public static void main(String[] args) {
		final int REGISTRYPORT = 1099;
		String registryHost = null; // host remoto con registry
		String serviceName = "";
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

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
            // Modifica questa
            String richiesta="Seleziona il servizio:\nE) Elimina registrazione\n" +
					"L) Lista prenotazioni di un veicolo specifico\n" +
					"^D/^Z per terminare\n";
			System.out.print(richiesta);

			while ((service = stdIn.readLine()) != null) {

				if (service.equals("E")) {

					System.out.print("Inserisci la targa: ");
                    String targa=stdIn.readLine().trim();
					int esito;

					if ((esito=serverRMI.elimina_prenotazione(targa)) == 0)
						System.out.println("Esito eliminazione positivo");
					else if(esito==-1)
                    	System.out.println("Targa non trovata");
					else if(esito==-2)
						System.out.println("Formato targa errato");
				}

				else if (service.equals("L")) {

					System.out.print("Inserisci il tipo: ");
					String tipo=stdIn.readLine().trim();

					Prenotazione[] result=serverRMI.visualizza_prenotazioni(tipo);

					if(result!=null){
						for(Prenotazione p:result){
							System.out.println(p.getTipo()+"\t"+p.getTarga()+"\t"+p.getPatente());
						}
					}
				}

				else
					System.out.println("Servizio non disponibile");

				System.out.print(richiesta);
			} // while (!EOF), fine richieste utente

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
