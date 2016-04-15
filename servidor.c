/* A simple server in the internet domain using TCP
The port number is passed as an argument */
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


void error(char *);
void waitFor(unsigned int);

int main(int argc, char *argv[]) {
    int i=0;
    char buffer[256];
    int sockfd[3], portno[3], clilen;
    int newsockfd, newsockfd2, newsockfd3, newsockfd4;
    struct sockaddr_in serv_addr[3], cli_addr[3];
    int n,n2,n3,n4;
    //Verificamos cantidad de entradas
    while(i!=argc){
        printf("argv[%i]: %s\n", i, argv[i]);
        i++;
    }
    //Verificamos cantidad de puerts dados
    if (argc < 4 || argc > 5) {
        fprintf(stderr,"ERROR, cantidad de puertos incorrecta\n");
        exit(1);
    }
    
    i=0;
    while(i<=3){
        //Abrimos los sockets
        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd[i] < 0)
            fprintf("ERROR opening socket %i", sockfd[i]);
        else
            printf("Socket %i opened: %i\n", i, sockfd[i]);
        i++;
    }

    i=0;
    while(i<=3){
        //Asignamos los sockets al puerto dado
        bzero((char *) &serv_addr[i], sizeof(serv_addr[i]));
        portno[i] = atoi(argv[i+1]);
        serv_addr[i].sin_family = AF_INET;
        serv_addr[i].sin_addr.s_addr = INADDR_ANY;
        serv_addr[i].sin_port = htons(portno[i]);
        i++;
    }

    i=0;
    while(i<=3){
        //Verificamos la asociacion
        if (bind(sockfd[i], (struct sockaddr *) &serv_addr[i], sizeof(serv_addr[i])) < 0)
            error("ERROR on binding");
        i++;
    }

    i=0;
    while(i<=3){
        //Ponemos en escucha a los puertos con solo una conexion
        listen(sockfd[i],0);
        i++;
    }
    i=0;
    clilen = sizeof(cli_addr);
    while(i<4){
        if(i==0){
            newsockfd = accept(sockfd[0], (struct sockaddr *) &cli_addr[0], &clilen);
            if(newsockfd < 0) { error("ERROR on accept"); }
            else { i=1;printf("%d\n", i); }
        }
        if(i==1){
            newsockfd2 = accept(sockfd[1], (struct sockaddr *) &cli_addr[1], &clilen);
            if(newsockfd2 < 0) { error("ERROR on accept"); }
            else { i++;printf("%d\n", i); }
        }
        if(i==2){
            newsockfd3 = accept(sockfd[2], (struct sockaddr *) &cli_addr[2], &clilen);
            if(newsockfd3 < 0) { error("ERROR on accept"); }
            else { i++;printf("%d\n", i); }
        }
        if(i==3){
            newsockfd4 = accept(sockfd[3], (struct sockaddr *) &cli_addr[3], &clilen);
            if(newsockfd4 < 0){ error("ERROR on accept"); }
            else { i++;printf("%d\n", i); }
        }
    }
    if (i==4){
        do{
            time_t epoch_time;
            struct tm *tm_p;
            epoch_time = time( NULL );
            tm_p = localtime( &epoch_time );
            int hora = tm_p->tm_hour;
            int min  = tm_p->tm_min;
            int seg  = tm_p->tm_sec;
            char dhora[] = {(hora/10)+48};
            char dmin[] = {(min/10)+48};
            char dseg[]  = {(seg/10)+48};
            char uhora[] = {(hora%10)+48};
            char umin[] = {(min%10)+48};
            char useg[]  = {(seg%10)+48};

            write(newsockfd,dmin,1);
            write(newsockfd2,umin,1);
            write(newsockfd3,dseg,1);
            write(newsockfd4,useg,1);
            waitFor(1);
        }while(1==1);
    }
}

void error(char *msg) {
    perror(msg);
    exit(1);
}

//Delay
void waitFor (unsigned int secs) {
    int retTime = time(0) + secs;
    while (time(0) < retTime);
}