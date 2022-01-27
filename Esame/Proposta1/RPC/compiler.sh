#!/bin/sh

rpcgen RPC_xFile.x
gcc -o server RPC_Server.c RPC_xFile_svc.c RPC_xFile_xdr.c
gcc -o client RPC_Client.c RPC_xFile_clnt.c RPC_xFile_xdr.c