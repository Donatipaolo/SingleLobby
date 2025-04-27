#ifndef CHANGE_USERNAME_H
#define CHANGE_USERNAME_H

////////////////////////////////////////////////////////////////////////////
//includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "client_listener.h"
#include "communication_handler.h"
#include <sys/select.h>
#include <unistd.h>
#define USERNAME_LENGTH 20
extern pthread_mutex_t lock;


//struct
struct username_pck{
    char buffer[USERNAME_LENGTH];
    bool result;
};

struct change_username_args{
    int clientfd;
    struct list** head;
    fd_set *fdset;
};

//Funzioni principali
void* change_username(void* args);


















#endif