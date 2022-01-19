import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class RMI_Server extends UnicastRemoteObject implements RMI_interfaceFile {

	// Costruttore
	public RMI_Server() throws RemoteException {
		super();
	}

	// Implementa metodo1
    public int metodo1(int parametro1){
        if(parametro1>0)return 0;
        else return -1;
    }


	// Avvio del Server RMI
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