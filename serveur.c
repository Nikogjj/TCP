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

#define NB_CLIENT_MAX 2

int server_fd;
int check_error;
int client_fd;
int nombre_client=0;
int tab_client[NB_CLIENT_MAX];


void* thread_recv(void* arg){
    int ID_client = (int)arg;
    while (1)
    {
        char reponse_buf[255];memset(reponse_buf,0,255);
        check_error=recv(ID_client,reponse_buf,sizeof reponse_buf,0);
        if (check_error==0||check_error==-1){
            printf("client[%d] has left the chat\n",ID_client);
            close(ID_client);
            for (int i = 0; i < nombre_client; i++)
            {
                if (tab_client[i]==ID_client)
                {
                    tab_client[i]=0;
                }
                
            }
            for (int i = 0; i < NB_CLIENT_MAX; i++)
            {
                if (tab_client[i]==0)
                {
                    tab_client[i]=tab_client[i+1];
                    tab_client[i+1]=0;
                }
                
            }
            
            nombre_client--;
            
            pthread_exit(NULL);           
        }
        else{
            printf("Nombre de clients: %d\n",nombre_client);
            for (int i = 0; i < nombre_client; i++)
            {
                if (tab_client[i]==ID_client)
                {
                    send(tab_client[i],"[OK]\n",6,0);perror("send a l'envoyeur du msg");
                }  
                else
                {
                    send(tab_client[i],reponse_buf,strlen(reponse_buf),0);perror("send");
                    printf("envoi effectué à client[%d]\n",tab_client[i]);
                }  
            }
        }
    }
}



void* thread_accept(){
    while (1)
    {

        struct sockaddr_in client_addr;
        socklen_t len;
        client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&len);perror("accept");
        if (client_fd==-1) pthread_exit(NULL);

        tab_client[nombre_client]=client_fd;

        send(client_fd,"Acces to server : OK ! Now you can chat online.\n",49,0);perror("send server OK !");
    
        pthread_t thread_receive;
        pthread_create(&thread_receive,NULL,thread_recv,(void*)tab_client[nombre_client]); 
        nombre_client++;
        if (nombre_client>=NB_CLIENT_MAX)
        {
            while(nombre_client>=NB_CLIENT_MAX){
                sleep(1);
            }
        };
        
    
    }

}

int main(){
    server_fd = socket(AF_INET,SOCK_STREAM,0);perror("socket");
    if(server_fd==-1)return EXIT_FAILURE;

    struct sockaddr_in serveur_addr={
        .sin_addr.s_addr=INADDR_ANY,
        .sin_family=AF_INET,
        .sin_port=htons(SERVER_PORT)
    };

    check_error=bind(server_fd,(struct sockaddr*)&serveur_addr,sizeof serveur_addr);perror("bind");
    if (check_error==-1)return EXIT_FAILURE;

    check_error= listen(server_fd,0);perror("listen");
    if (check_error==-1)return EXIT_FAILURE;

    pthread_t thread;
    pthread_create(&thread,NULL,thread_accept,NULL);

    while (1)
    {   char close_server[50];memset(close_server,0,50);
        fgets(close_server,50,stdin);
        close_server[strlen(close_server)-1]=0;
        if (strcmp(close_server,"close")==0)
        {
            break;
        }
        else{
            printf("Commande invalide. Entrez la commande \"close\" pour fermer le serveur.\n");
        }
    }
    
    close(server_fd);
    
    return 1;
}
