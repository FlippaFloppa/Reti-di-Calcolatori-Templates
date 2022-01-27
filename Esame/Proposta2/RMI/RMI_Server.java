import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class RMI_Server extends UnicastRemoteObject implements RMI_interfaceFile {

	private Dati d[];
	private static final int DIM = 5;

	public RMI_Server() throws RemoteException {
		super();
		d = new Dati[DIM];
		for (int i = 0; i < DIM; i++) {
			d[i] = new Dati("L", "L");
		}
	}

	public void visualizza() {
		for (int i = 0; i < DIM; i++) {
			System.out.println("Elemento: " + i + " -> " + d[i].toString());
		}
	}

	// synchronized (?)
	public int elimina_dati(String request) {
		System.out.println("Ricevuta richiesta: " + request);

		if (request != null)
			return 1;
		else
			return 0;
	}

	// synchronized (?)
	public Dati[] visualizza_dati(String request) {
		System.out.println("Ricevuta richiesta: " + request);
		visualizza();
		return d;
	}

	public static void main(String[] args) {

		final int REGISTRYPORT = 1099;
		String registryHost = "localhost";
		String serviceName = "nomeServizio"; // lookup name...

		// Registrazione del servizio RMI
		String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;

		try {
			RMI_Server serverRMI = new RMI_Server();
			Naming.rebind(completeName, serverRMI);
			System.out.println("Server RMI: Servizio \"" + serviceName + "\" registrato");
		} catch (Exception e) {
			System.err.println("Server RMI \"" + serviceName + "\": " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}
}