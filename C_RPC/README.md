## RPC di Sun (C)

### Preparazione RPC
* Compilare correttamente il file RPC_xFile.x
* Eseguire `rpcgen RPC_xFile.x` per generare i files ausiliari

### Deploy Server
* Compilare il Server: `gcc -o server RPC_Server.c RPC_xFile_svc.c RPC_xFile_xdr.c`
* Lanciare `rpcbind`
* Lanciare il Server: `./server`

### Deploy Client
* Compilare il Server: `gcc -o client RPC_Client.c RPC_xFile_clnt.c RPC_xFile_xdr.c`
* Lanciare il Server: `./client localhost`
