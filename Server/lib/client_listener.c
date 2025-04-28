#include "client_listener.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Funzione principale
void* client_listener(void* args){

    struct client_listener_args *arg = args;

    //Inizializzazione delle variabili
    struct sockaddr_in addr;
    socklen_t len;
    int clientfd,sockfd;
    pthread_t id;
    *(arg->num) = 0;

    //Inizializzazione del socket
    sockfd = init_socket(AF_INET,SOCK_STREAM,0,PORT,NULL,true);
    printf("Socket Inizializzato\n");

    while(true){

        //Metto il server in ascolto
        if(listen(sockfd,3) < 0){
            perror("Listen");
            exit(EXIT_FAILURE);
        }

        if((clientfd = accept(sockfd,(struct sockaddr*)&addr,&len)) < 0){
            perror("Accept");
            exit(EXIT_FAILURE);
        }

        // Acquisisci il lock
        pthread_mutex_lock(&lock);

        //Aggiungo alla lista il client
        push(arg->head,client_construct(NULL,clientfd));

        //creo i parametri per il thread
        struct change_username_args *pk = malloc(sizeof(struct change_username_args));
        pk->clientfd = clientfd;
        pk->fdset = arg->fdset;
        pk->head = arg->head;

        if(pthread_create(&id,NULL,change_username,pk) < 0){
            pop(arg->head);
            free(pk);
            pthread_mutex_unlock(&lock);
            continue;
        }

        *(arg->num) += 1;

        //modifico l'fd max
        if(clientfd > *(arg->max_fd))
            *arg->max_fd = clientfd;

        // Rilascia il lock
        pthread_mutex_unlock(&lock);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Inizializzazione della socket
int init_socket(int domain,int type,int protocol,int port, char* ip,bool server){

    //Creazione della socket
    int sockfd, opt = 1;
    
    if((sockfd = socket(domain,type,protocol)) == -1){
        perror("Socket");
        exit(EXIT_FAILURE);
    }

    //Inserisco delle opzioni aggiuntive SO_REUSEADDR
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) == -1){
        perror("Set socket");
        exit(EXIT_FAILURE);
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
    }

    return sockfd;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Funzioni per le liste
void* client_construct(char* username, int fd){
    struct client* cli = malloc(sizeof(struct client));
    cli->fd = fd;
    cli->username = username;
    return cli;
}

size_t client_find(struct list** head,int fd){
    
    if(!head || !*head) return SIZE_MAX;
    
    struct list* current = *head;
    size_t index = 0;
    while(current != NULL){
        if(current->arg != NULL && ((struct client*)current->arg)->fd == fd)
            return index;

        index++;
        current = current->next;
    }

    return SIZE_MAX;
}

int client_fd(struct list** head, size_t position){
    if (!head || !*head) return -1;

    struct list* temp = *head;
    for (size_t i = 0; i < position; i++) {
        if (!temp) return -1;
        temp = temp->next;
    }

    return temp ? ((struct client*)temp->arg)->fd : -1;
}