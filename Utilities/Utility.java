package newLib;

import java.io.*;

public class Utility {

    // ricevi_file goto 11
    // invia_file goto 89
    // elimina_occorrenze goto 149

    public void ricevi_file(BufferedReader stdIn, DataOutputStream outSock, DataInputStream inSock) throws IOException {

        final int SIZE = 256;

        byte[] buff = new byte[SIZE];
        int numFile, letti = 0;
        boolean ok;
        String direttorio, nomeFile;
        long dim;
        DataOutputStream outFile = null;

        // String dimMinima;
        long dimMin = -1;

        ok = false;

        System.out.println("Inserisci un direttorio");
        direttorio = stdIn.readLine();
        System.out.println("Invio direttorio: " + direttorio);
        outSock.writeUTF(direttorio);

        // while (!ok) {
        //     System.out.println("Inserisci dim minima");
        //     dimMinima = stdIn.readLine();
        //     try {
        //         dimMin = Long.parseLong(dimMinima);
        //         ok = true;
        //     } catch (NumberFormatException e) {
        //         continue;
        //     }
        // }

        outSock.writeLong(dimMin);

        ok = false;
        // ricevo il numero di file
        numFile = inSock.readInt();

        while (numFile > 0) {
            // ricevo la dim
            dim = inSock.readLong();
            if (numFile > 0) {
                // ricevo il nome
                nomeFile = inSock.readUTF();
                System.out.println("Ricevo " + nomeFile + " dim: " + dim);

                try {
                    outFile = new DataOutputStream(new FileOutputStream(nomeFile));
                    ok = true;
                } catch (Exception e) {
                    System.out.println("Errore nella creazione di " + nomeFile);
                    ok = false;

                } finally {
                    if (ok) {
                        while (dim > 0) {
                            letti = inSock.read(buff, 0, (int) (dim > SIZE ? SIZE : dim));
                            outFile.write(buff, 0, letti);
                            dim = dim - letti;
                        }
                        outFile.flush();
                        outFile.close();

                    } else {
                        while (dim > 0) {
                            // leggo senza scrivere
                            letti = inSock.read(buff, 0, (int) (dim > SIZE ? SIZE : dim));
                            dim = dim - letti;
                        }
                    }
                }
            }
            numFile--;
        }
    }

    public void invia_file(DataOutputStream outSock, DataInputStream inSock) throws IOException {

        final int SIZE = 256;

        String direttorio;
        // long dimMinima;
        File dir;
        File[] files;
        DataInputStream inFile = null;
        int letti;
        byte[] buff = new byte[SIZE];

        int numFile = 0;
        direttorio = inSock.readUTF();
        // dimMinima = inSock.readLong();

        dir = new File(direttorio);
        files = dir.listFiles();

        if (files == null) {
            // torno 0 file
            outSock.writeInt(0);
        }

        for (int i = 0; i < files.length; i++) {
            // if (files[i].length() > dimMinima) {
            try {
                inFile = new DataInputStream(new FileInputStream(files[i]));
                numFile++;
                inFile.close();
            } catch (Exception e) {
                System.out.println("Errore apertura " + files[i].getName());
                continue;
            }
            // }
        }
        System.out.println("numFile: " + numFile);
        outSock.writeInt(numFile);

        for (int i = 0; i < files.length; i++) {
            // if (files[i].length() > dimMinima) {
            inFile = new DataInputStream(new FileInputStream(files[i]));
            System.out.println("Apertura " + files[i].getName());
            outSock.writeLong(files[i].length());
            outSock.writeUTF(files[i].getName());
            try {
                while ((letti = inFile.read(buff)) > 0) {
                    outSock.write(buff, 0, letti);
                }
            } catch (IOException e) {
                System.out.println(("Errore lettura"));
                inFile.close();
                continue;
            }
            inFile.close();
            // }
        }
        System.out.println("Ho finito i file");
    }

    public void elimina_occorrenze(DataOutputStream outSock, DataInputStream inSock) throws IOException {
        // return numero occorrenze eliminate
        // return -1 se il file non è di testo o non si riesce ad aprirlo

        String nomeFile, parola, lineaAttuale;
        File inF = null, outF = null;
        BufferedWriter outFile = null;
        BufferedReader inFile = null;

        int numeroOccorrenze = 0;

        nomeFile = inSock.readUTF();
        parola = inSock.readUTF();
        System.out.println("Ricevuto: " + nomeFile + " " + parola);

        if (!nomeFile.endsWith(".txt")) {
            outSock.writeInt(-1);
            // non è un file di testo
            // continue;
        }

        try {
            inF = new File(nomeFile);
            outF = new File(nomeFile + ".txt");
            outFile = new BufferedWriter(new FileWriter(outF));
            inFile = new BufferedReader(new FileReader(inF));
        } catch (Exception e) {
            System.out.println("Errore apertura file");
            outSock.writeInt(-1);
            // continue;
        }

        String[] pezzi;

        while ((lineaAttuale = inFile.readLine()) != null) {
            if (!lineaAttuale.contains(parola)) {
                outFile.write(lineaAttuale);
            } else {
                pezzi = lineaAttuale.split(" ");
                for (int i = 0; i < pezzi.length; i++) {
                    if (pezzi[i].compareTo(parola) != 0) {
                        outFile.write(pezzi[i] + " ");
                    } else {
                        numeroOccorrenze++;
                        outFile.write(" ");
                    }
                }
            }
            outFile.newLine();
        }

        outFile.close();
        inFile.close();
        if (!outF.renameTo(inF)) {
            System.out.println("Errore rename");
        }

        System.out.println("Invio res: " + numeroOccorrenze);
        outSock.writeInt(numeroOccorrenze);
    }
}
