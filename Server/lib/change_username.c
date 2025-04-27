#include "change_username.h"

void* change_username(void* args){
    struct change_username_args *arg = args;
    struct username_pck pk;
    struct client* cli;

    while(true){

        //prendo in input lo username
        if(read(arg->clientfd,&pk,sizeof(struct username_pck)) <= 0){
            //rimuovo il client
            kill_client(arg->clientfd,arg->head,arg->fdset);
            pthread_exit(NULL);
        }
        //Apro la sezione critica
        pthread_mutex_lock(&lock);

        pk.result = 1;
        
        //Controllo se non è gia stato preso
        for(int i = 0; i < size(arg->head);i++){
            cli = at(arg->head,i);
            if(cli->username != NULL && strcmp(cli->username,pk.buffer) == 0){
                pk.result = 0;
                break;
            }
        }

        
        //Invio il pacchetto
        if(write(arg->clientfd,&pk,sizeof(struct username_pck)) <= 0){
            //rimuovo il client
            kill_client(arg->clientfd,arg->head,arg->fdset);
            pthread_exit(NULL);
        }

        pthread_mutex_unlock(&lock);

        if(pk.result){
            break;
        }

    }
    
    pthread_mutex_lock(&lock);
    //aggiorno il client nella client list
    cli = at(arg->head,client_find(arg->head,arg->clientfd));
    char* buffer = malloc(sizeof(char)*USERNAME_LENGTH);
    memcpy(buffer,pk.buffer,USERNAME_LENGTH);
    cli->username = buffer;
    
    //chiudo la sezione critica
    pthread_mutex_unlock(&lock);

    FD_SET(arg->clientfd,arg->fdset);
    free(arg);

    pthread_exit(NULL);

    //esco
}