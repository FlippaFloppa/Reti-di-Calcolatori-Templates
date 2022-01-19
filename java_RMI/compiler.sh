#/bin/sh

javac RMI_Server.java RMI_interfaceFile.java RMI_Client.java
rmic -vcompat RMI_Server