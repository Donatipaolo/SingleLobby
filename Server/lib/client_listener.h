#ifndef CLIENT_LISTENER_H
#define CLIENT_LISTENER_H

//Codice per il thread che gestisce la connessione dei client

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>  
#include <sys/select.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "./list.h"
#include "change_username.h"
#define PORT 5000
#define USERNAME_LENGTH 20

extern pthread_mutex_t lock;

struct client_listener_args{
    fd_set *fdset;
    uint16_t *max_fd;
    struct list** head;
    int *num;
};

struct client{
    char* username;
    int fd;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Funzione principale
void* client_listener(void* args);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Inizializzazione della socket
int init_socket(int domain,int type,int protocol,int port, char* ip,bool server);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Funzioni per le liste
void* client_construct(char* username, int fd);
size_t client_find(struct list** head,int fd);
int client_fd(struct list** head, size_t position);




#endif