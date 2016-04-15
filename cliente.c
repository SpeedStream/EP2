 #include <stdlib.h>
 #include <strings.h>
 #include <stdio.h>
 #include <fcntl.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <sys/stat.h>
 #include <netinet/in.h>
 #include <netdb.h> 
 #include <unistd.h>
 #include <signal.h>

#define MAX_BUF 1024

int cltSocket;
int endPadre=0;
int endHijo=0;
void signalFunc(int);
void error(char *);
void waitFor(unsigned int);

 
 int main(int argc, char *argv[]) {
    int clientPipe;
    char * cltPipe = "cliPipe";
    mkfifo(cltPipe, 0666);
    char buff[MAX_BUF];
    int portno;
 
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];
	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}

	portno = atoi(argv[2]);
	cltSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (cltSocket < 0){
		error("ERROR opening socket");
    }
	server = gethostbyname(argv[1]);

	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(cltSocket,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");

    /*
    Padre
        |--- fork --> Hijo => Lee socket --> Escribe en pipe
        |------------------=> Lee pipe   --> Imprime en pantalla
    */
	int init = 0;
	pid_t child_pid;
    child_pid = fork ();
    if(child_pid==0){
        //Proceso Hijo2. Escribirá al pipe cuando reciba en el socket
    	//printf("Cliente -> Hijo2\n");
    	int i = 0;
        signal(SIGINT, signalFunc);        //Señal de interrupción -> Ctrl+C
        do{
        	printf("Leyendo socket...\n");
            bzero(buffer,256);
            read(cltSocket,buffer,255);                 //Leemos socket de cliente y guardamos en buffer
            clientPipe = open(cltPipe, O_WRONLY);     //Abrimos pipe
            switch(buffer[0]){
                /*
                    Dependiendo del valor correspondiente en buffer, escribiremos en el dato en el pipe write_clientPipe
                    Leemos socket -> Escribimos en pipe -> Cerramos pipe
                */
                printf("Escribiendo en pipe...\n");
                case '0':
                	if(i>1){ write(clientPipe, "0", sizeof("0"));}
                	else   { i++; }
                    break;
                case '1':
                    if(i>1) { write(clientPipe, "1", sizeof("1")); }
                    else    { i++; }
                    break;
                case '2':
                	if(i>1) { write(clientPipe, "2", sizeof("2")); }
                    else    { i++; }
                    break;
                case '3':
                	if(i>1) { write(clientPipe, "3", sizeof("3")); }
                    else    { i++; }
                    break;
                case '4':
                    if(i>1) { write(clientPipe, "4", sizeof("4")); }
                    else    { i++; }
                    break;
                case '5':
                	if(i>1) { write(clientPipe, "5", sizeof("5")); }
                    else    { i++; }
                    break;
                case '6':
                	if(i>1) { write(clientPipe, "6", sizeof("6")); }
                    else    { i++; }
                    break;
                case '7':
                	if(i>1){ write(clientPipe, "7", sizeof("7")); }
                    else    { i++; }
                break;
                case '8':
                	if(i>1) { write(clientPipe, "8", sizeof("8")); }
                    else    {i++;}
                    break;
                case '9':
                	if(i>1) { write(clientPipe, "9", sizeof("9")); }
                    else    { i++; }
                    break;
            }
            printf("Cerrando pipe\n");
            close(clientPipe);
        }while (endHijo==0);
    }
    else{
        //Proceso Padre
        //printf("Cliente -> Padre\n");
       	int j = 0;
        signal(SIGINT, signalFunc);        //Señal de interrupción -> Ctrl+C
       	do {
       		printf("Leyendo en pipe...\n");
            clientPipe = open(cltPipe, O_RDONLY);       //Abrimos pipe
            read(clientPipe, buff, MAX_BUF);            //Leemos pipe
            char a = buff[0];                                //Amacenamos en biffer
            switch(a) {
                /*
                    Dependiendo del valor correspondiente en el pipe clientPipe, mostramos la imagen correspondiente
                    Leemos pipe -> Seleccionamos imagen -> Cerramos pipe
                */
                printf("Mostrando imagen...\n");
                case '0':
                  	if(j>1) { system("display -remote 0.jpg"); }
                    else    { j++; }
                    break;
                case '1':
                   	if(j>1) { system("display -remote 1.jpg"); }
                    else    { j++; }
                    break;
                case '2':
                   	if(j>1) { system("display -remote 2.jpg"); }
                    else    { j++; }
                    break;
                case '3':
                	if(j>1) { system("display -remote 3.jpg"); }
                    else    { j++; }
                    break;
                case '4':
                	if(j>1) { system("display -remote 4.jpg"); }
                    else    { j++; }
                    break;
                case '5':
                	if(j>1) { system("display -remote 5.jpg"); }
                    else    { j++; }
                    break;
                case '6':
                	if(j>1) { system("display -remote 6.jpg"); }
                    else    { j++; }
                    break;
                case '7':
                	if(j>1) { system("display -remote 7.jpg"); }
                    else    { j++; }
                    break;
                case '8':
                	if(j>1) { system("display -remote 8.jpg"); }
                    else    { j++; }
                    break;
                case '9':
                	if(j>1) { system("display -remote 9.jpg"); }
                    else    { j++; }
                    break;
                }
            printf("Cerrando pipe...\n");
            close(clientPipe);
        }while (endPadre==0);
    }
}

void signalFunc(int sig) {
    printf("Ending\n");
    close(cltSocket);
    endHijo++;
    endPadre++;
    exit(1);
}

void error(char *msg) {
     perror(msg);
     exit(0);
}