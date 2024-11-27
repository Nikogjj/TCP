#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include "port.h"

int client_fd;
int ckeck_if_client_is_accepted;

void *thread_recv(){
    while (1)
    {
        char reponse[255];memset(reponse,0,255);
        int reponse_length=recv(client_fd,reponse,sizeof reponse,0);perror("recv");

        // printf("oifpifjqpifjqpdjf\n");
        if(reponse_length==-1)return EXIT_FAILURE;

            printf("$%s",reponse);    
        
    }
    
}

int main(int argc,int *argv){
    int port_client=0;
    if (argc>1)
    {
        port_client=&argv;
    }
    
    struct sockaddr_in client_addr={
        .sin_addr.s_addr=INADDR_ANY,
        .sin_family=AF_INET,
        .sin_port=htons(port_client)
    };

    struct sockaddr_in serveur_addr={
        .sin_addr.s_addr=INADDR_ANY,
        .sin_family=AF_INET,
        .sin_port=htons(SERVER_PORT)
    };

    socklen_t len;


    client_fd = socket(AF_INET,SOCK_STREAM,0);perror("socket");
    if(client_fd==-1)return EXIT_FAILURE;


    int check_error=bind(client_fd,(struct sockaddr*)&client_addr,sizeof client_addr);perror("bind");
    if(check_error==-1)return EXIT_FAILURE;
    printf("%d\n",check_error);
    printf("En attente d'une place sur le serveur...\n");
    printf("%d\n",check_error);

    check_error=connect(client_fd,(struct sockaddr*)&serveur_addr,sizeof serveur_addr);perror("connect");
    printf("%d\n",check_error);
    if(check_error==-1)return EXIT_FAILURE;
    while (/* condition */)
    {
        
    }
    

    pthread_t thread;
    pthread_create(&thread,NULL,thread_recv,NULL);

    while(1){
        char reponse[255];memset(reponse,0,255);
        fgets(reponse,255,stdin);
        send(client_fd,reponse,strlen(reponse),0);
        // system("clear");
    }

    


    close(client_fd);
}