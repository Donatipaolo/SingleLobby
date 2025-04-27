#include "communication_handler.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fuzione principale
void* communication_handler(void *args){
    //Inizializzazione delle variabili
    struct communication_handler_args* arg = args;

    fd_set tempfd;
    struct timeval tv;

    while(true){
        tempfd = *(arg->fdset);
        tv.tv_sec = 2;
        tv.tv_usec = 0;


        if(select((*arg->max_fd)+1,&tempfd,NULL,NULL,&tv) < 0){
            perror("Select");
            exit(EXIT_FAILURE);
        }

        //Controllo ogni fd
        for(size_t i = 3; i < *(arg->max_fd);i++){
            if(FD_ISSET(i,&tempfd)){
                handle_client(i,arg->head,arg->fdset);
            }
        }   
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void handle_client(int clientfd,struct list** head,fd_set* fdset){
    //Inizializzazione delle variabili
    struct pck pack;

    //Leggo il pacchetto
    if(read(clientfd,&pack,sizeof(struct pck)) <= 0){
        kill_client(clientfd,head,fdset);
        return;
    }

    //Controllo il tipo del pacchetto
    switch(pack.tp){
        case MESSAGE:
            send_msg_to_all(clientfd,head,&pack);
            break;
        case EXIT:
            kill_client(clientfd,head,fdset);
            break;
        default:
            kill_client(clientfd,head,fdset);
            break;
    }
}

void send_msg_to_all(int senderfd,struct list** head,struct pck *pk){

    //lock
    pthread_mutex_lock(&lock);

    //Prendo la posizione
    size_t pos = client_find(head,senderfd);

    //Inizializzo il pacchetto
    struct msg_pck* msg = (struct msg_pck*)pk->bytes;
    struct client* cli = at(head,pos);
    
    //Definisco il pacchetto
    pk->tp = MESSAGE;
    memcpy(msg->username,cli->username,USERNAME_LENGTH);

    for(int i = 0; i < size(head); i++){
        if(i != pos){
            //Invio il messaggio
            if(write(client_fd(head,i),pk,sizeof(struct pck)) < 0){
                printf("Errore nella write con %d\n",i);
            }
        }
    }

    pthread_mutex_unlock(&lock);
}

void kill_client(int clientfd,struct list** head,fd_set *fdset){
    //rimuovo il client
    pthread_mutex_lock(&lock);
    eliminate(head,client_find(head,clientfd));
    FD_CLR(clientfd,fdset);
    close(clientfd);
    pthread_mutex_unlock(&lock);
}