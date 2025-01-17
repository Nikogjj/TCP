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
int check_if_client_is_accepted=0;
char debut_de_reponse[300];

void *thread_recv(){
    while (1)
    {
        char reponse[255];memset(reponse,0,255);
        int reponse_length=recv(client_fd,reponse,sizeof reponse,0);//perror("recv");
        if(reponse_length==-1)return NULL;

        printf("%s",reponse);

    }
    
}

int main(int argc,char **argv){
    int port_client=0;
    if (argc>1)
    {
        port_client=atoi(argv[1]);
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
    
    char pseudo[16];memset(pseudo,0,16);
    set_username(pseudo);


    client_fd = socket(AF_INET,SOCK_STREAM,0);perror("socket");
    if(client_fd==-1)return EXIT_FAILURE;


    int check_error=bind(client_fd,(struct sockaddr*)&client_addr,sizeof client_addr);perror("bind");
    if(check_error==-1)return EXIT_FAILURE;


    check_error=connect(client_fd,(struct sockaddr*)&serveur_addr,sizeof serveur_addr);//perror("connect");
    if(check_error==-1)return EXIT_FAILURE;
    printf("Wait until the server accept you...\n");

    char reponse[255];memset(reponse,0,255);
    int reponse_length=recv(client_fd,reponse,sizeof reponse,0);//perror("recv");
    if(reponse_length==-1)return EXIT_FAILURE;
    printf("%s",reponse); 
    
    
    check_if_client_is_accepted++;

    if (check_if_client_is_accepted==1)
    {
        pthread_t thread;
        pthread_create(&thread,NULL,thread_recv,NULL);

        while(1){
            
            memset(debut_de_reponse,0,300);
            sprintf(debut_de_reponse,"%s : ",pseudo);
            char reponse[255];memset(reponse,0,255);
            fgets(reponse,255,stdin);
            strcat(debut_de_reponse,reponse);
            send(client_fd,debut_de_reponse,strlen(debut_de_reponse),0);
            reponse[strlen(reponse)-1]=0;
            printf("Vous : %s ",reponse);

        }
    }
    



    


    close(client_fd);
}