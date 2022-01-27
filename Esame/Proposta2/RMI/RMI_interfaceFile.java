import java.rmi.*;

public interface RMI_interfaceFile extends Remote {

    public int elimina_dati(String request) throws RemoteException;

    public Dati[] visualizza_dati(String request) throws RemoteException;

}