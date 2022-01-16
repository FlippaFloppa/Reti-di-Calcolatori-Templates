import java.io.Serializable;
import java.nio.file.Path;

public class Prenotazione implements Serializable {

    private String tipo;
    private String targa;
    private String patente;
    private String imageFolder;

    public Prenotazione(String tipo, String targa, String patente, String imageFolder) {
        this.tipo = tipo;
        this.targa = targa;
        this.patente = patente;
        this.imageFolder = imageFolder;
    }

    public Prenotazione() {
        this.tipo = "L";
        this.imageFolder = "L";
        this.patente = "0";
        this.targa = "L";
    }

    public String getTipo() {
        return tipo;
    }

    public void setTipo(String tipo) {
        this.tipo = tipo;
    }

    public String getTarga() {
        return targa;
    }

    public void setTarga(String targa) {
        this.targa = targa;
    }

    public String getPatente() {
        return patente;
    }

    public void setPatente(String patente) {
        this.patente = patente;
    }

    public String getImageFolder() {
        return imageFolder;
    }

    public void setImageFolder(String imageFolder) {
        this.imageFolder = imageFolder;
    }
}