import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMI_interfaceFile extends Remote{
    
    // Definisci qua i metodi implementati poi su RMI_Server

    Prenotazione[] visualizza_prenotazioni(String tipo) throws RemoteException;

    int elimina_prenotazione(String targa) throws RemoteException;

}