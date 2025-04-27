#include "./lib/client_listener.h"
#include "./lib/communication_handler.h"
#include "./lib/change_username.h"

//Definizione del lock
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


int main(){

    struct list** head = malloc(sizeof(struct list*));
    fd_set *fdset = malloc(sizeof(fd_set));
    uint16_t *max_fd = malloc(sizeof(uint16_t));
    int *num = malloc(sizeof(int));
    pthread_t id;

    //Inizializzazione setfd
    FD_ZERO(fdset);
    *head = NULL;

    //Inizializzazione di ogni thread
    struct client_listener_args *cli_args = malloc(sizeof(struct client_listener_args));
    cli_args->fdset = fdset;
    cli_args->head = head;
    cli_args->max_fd = max_fd;
    cli_args->num = num;

    if(pthread_create(&id,NULL,client_listener,cli_args) < 0){
        perror("client listener");
        exit(EXIT_FAILURE);
    }

    struct communication_handler_args* com_args = malloc(sizeof(struct communication_handler_args));
    com_args ->fdset = fdset;
    com_args->head = head;
    com_args->max_fd = max_fd;
    com_args->num = num;

    if(pthread_create(&id,NULL,communication_handler,com_args) < 0){
        perror("communication handler");
        exit(EXIT_FAILURE);
    }

    printf("Server is working!\n");
    while(true){

    }

    return 0;
}