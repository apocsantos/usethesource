Primeira vers�o com sockets, seguindo a sugest�o do Bruno, de se utilizar sockets. (ver mail sequ�ncia de mails de 23/09 Subject: Montagem da estufa, cita�ao: "https://sites.google.com/site/toucatronic/arduino/arduino-e-socket-java
Aqui voc�s podem ver uma ideia, do que estou a falar.")


Abre socket, aguarda comando, executa comando, retorna informa��o
Comando � aceite quando � primida a tecla enter (carriage return), por causa dos mac's-
Comando � executado, devolve resultado, aguarda novo comando.

Para efeitos de teste existe uma fun��o com a lista de comandos, que dever� vir a ser removida.