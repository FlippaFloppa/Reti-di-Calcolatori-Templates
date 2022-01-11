import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMI_interfaceFile extends Remote{
    
    // Definisci qua i metodi implementati poi su RMI_Server

    int metodo1(int parametro1) throws RemoteException;

}