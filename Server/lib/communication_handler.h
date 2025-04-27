#ifndef COMMUNICATION_HANDLER_H
#define COMMUNICATION_HANDLER_H

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h> 
#include "list.h"
#include "client_listener.h"
#define USERNAME_LENGTH 20
#define MAX_PCK_LENGTH (1024 + USERNAME_LENGTH)

extern pthread_mutex_t lock;

//Strutture per la definizione del pacchetto
enum type{
    MESSAGE,
    EXIT
};

struct pck{
    enum type tp;
    uint8_t bytes[MAX_PCK_LENGTH];
};

//definzione della struct per ogni tipo di pacchetto
struct msg_pck{
    char msg[MAX_PCK_LENGTH]; 
    char username[USERNAME_LENGTH];
};


//////////////////////////////////////////////////////////////////////////////////////////////
//Gestione interna
struct communication_handler_args{
    fd_set *fdset;
    uint16_t *max_fd;
    struct list** head;
    int* num;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Funzione principale
void* communication_handler(void *args);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Funzioni per la gestione dei client
void handle_client(int clientfd,struct list** head,fd_set* fdset);
void send_msg_to_all(int senderfd,struct list** head,struct pck *pk);
void kill_client(int clientfd,struct list** head,fd_set *fdset);


#endif