#include "./lib/client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 5000
#define IP "127.0.0.1"
//Inizializzazione della socket
int init_socket(int domain,int type,int protocol,int port, char* ip,bool server);

int main(){

    int socketfd = init_socket(AF_INET,SOCK_STREAM,0,PORT,IP,false); 

    change_username(socketfd);

    return 0;
}

int init_socket(int domain,int type,int protocol,int port, char* ip,bool server){

    //Creazione della socket
    int sockfd, opt = 1;
    
    if((sockfd = socket(domain,type,protocol)) == -1){
        perror("Socket");
        exit(EXIT_FAILURE);
    }

    if(server){
        //Inserisco delle opzioni aggiuntive SO_REUSEADDR
        if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) == -1){
            perror("Set socket");
            exit(EXIT_FAILURE);
        }
    }

    //Inizializzazione dei valori
    struct sockaddr_in address;
    address.sin_family = domain; //
    address.sin_port = htons(port);
    
    //Controllo l'indirizzo
    if(server){
        address.sin_addr.s_addr = INADDR_ANY;
    }
    else{
        if (inet_pton(domain,ip, &address.sin_addr) <= 0) {
            perror("Inet Pton");
            exit(EXIT_FAILURE);
        }
    }

    //Binding dei valori
    if(server){
        if(bind(sockfd,(struct sockaddr*)&address,(socklen_t)sizeof(address)) < 0){
            perror("Binding");
            exit(EXIT_FAILURE);
        }
    }else{
        if(connect(sockfd,(struct sockaddr*)&address,sizeof(address)) < 0){
            perror("connect");
            exit(EXIT_FAILURE);
        }
    }

    return sockfd;
}
