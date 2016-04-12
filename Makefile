all: server client

server:
	gcc -w -o servidor servidor.c

client:
	gcc -w -o cliente cliente.c

clean:
	rm -rf *o cliente servidor cliPipe

git:
	$ git add .
	$ git add -A
	$ git commit -m 'Version final'
	$ git push origin master