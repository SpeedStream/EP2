all: server client

server:
	gcc -w -o servidor servidor.c

client:
	gcc -w -o cliente cliente.c

clean:
	rm -rf *o cliente servidor cliPipe