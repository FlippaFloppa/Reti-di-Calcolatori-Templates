## Java RMI

* RMI_Client.java
* RMI_Server.java
* RMI_interfaceFile.java

### Deploy Server 
- Compila Interface e Server: `javac RMI_Server.java RMI_interfaceFile.java`
- Genera stub e skeleton: `rmic -vcompat RMI_Server`
- Lancia il registry in una shell separata: `rmiregistry`
- Lancia il server: `java RMI_Server ...`

### Deploy Client
- Compila il Client: `javac RMI_Client.java`
- Lancia il Client: `java RMI_Client ...`