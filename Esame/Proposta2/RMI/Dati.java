import java.io.Serializable;

public class Dati implements Serializable {
    private String first;
    private String second;

    public Dati(String first, String second) {
        if (first == null || second == null) {
            throw new IllegalArgumentException("Errore creazione");
        }
        this.first = first;
        this.second = second;
    }

}
