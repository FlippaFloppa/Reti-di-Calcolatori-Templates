import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.List;

public class RMI_Server extends UnicastRemoteObject implements RMI_interfaceFile {

	// Strutture
	private Prenotazione[] prenotazioni;
	private final String patternTarga = "[A-Z][A-Z]\\d{3}[A-Z][A-Z]";

	// Costruttore
	public RMI_Server() throws RemoteException {
		super();

		prenotazioni=new Prenotazione[5];

		prenotazioni[0]=new Prenotazione("auto","AA678LO","09836","09836_img/");
		prenotazioni[1]=new Prenotazione("camper","EZ678LO","43454","43454_img/");
		prenotazioni[2]=new Prenotazione("camper","HJ678LO","34354","34354_img/");
		prenotazioni[3]=new Prenotazione("auto","LK678LO","0836","0836_img/");
		prenotazioni[4]=new Prenotazione("auto","AB678LO","43433","43433_img/");
	}


	// Avvio del Server RMI
	public static void main(String[] args) {

		final int REGISTRYPORT = 1099;
		String registryHost = "localhost";
		String serviceName = "RentAcaR"; // lookup name...

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

	@Override
	public Prenotazione[] visualizza_prenotazioni(String tipo) throws RemoteException {

		int count=0;
		Prenotazione[] result=new Prenotazione[5];

		for(int i=0;i<5;i++){
			if(prenotazioni[i].getTipo().compareTo(tipo)==0
					&& prenotazioni[i].getTarga().compareTo("ED000AA")>=0){
				count++;
			}
		}

		result=new Prenotazione[count];
		count=0;

		for(int i=0;i<5;i++){
			if(prenotazioni[i].getTipo().compareTo(tipo)==0
					&& prenotazioni[i].getTarga().compareTo("ED000AA")>=0){
				result[count]=prenotazioni[i];
				count++;
			}
		}

		return result;
	}

	@Override
	public int elimina_prenotazione(String targa) throws RemoteException {
		if(!targa.matches(patternTarga))return -1;

		for(Prenotazione p:prenotazioni){
			if(p.getTarga().compareTo(targa)==0){
				p.setImageFolder("L");
				p.setTarga("L");
				p.setTarga("0");
				p.setTipo("L");

				return 0;
			}
		}

		return -2;
	}

}