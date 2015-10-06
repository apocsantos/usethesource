Primeira versão com sockets, seguindo a sugestão do Bruno, de se utilizar sockets. (ver mail sequência de mails de 23/09 Subject: Montagem da estufa, citaçao: "https://sites.google.com/site/toucatronic/arduino/arduino-e-socket-java
Aqui vocês podem ver uma ideia, do que estou a falar.")


Abre socket, aguarda comando, executa comando, retorna informação
Comando é aceite quando é primida a tecla enter (carriage return), por causa dos mac's-
Comando é executado, devolve resultado, aguarda novo comando.

Para efeitos de teste existe uma função com a lista de comandos, que deverá vir a ser removida.