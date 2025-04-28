#include "client.h"
#include <string.h>
#include <sys/select.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <termios.h>

///////////////////////////////////////////////////////////////////////////
//Funzioni principali
void communication(int serverfd,char* username){
    //Inizializzazione delle variabili
    fd_set fdset,tempfd;
    FD_ZERO(&fdset);
    FD_SET(STDIN_FD,&fdset);
    FD_SET(serverfd,&fdset);

    //msg
    char buffer[MSG_LENGTH];
    system("clear");
    print_init_communication();

    //Inizializzo la lista 
    struct list* head = NULL;

    while(true){
        tempfd = fdset;

        if(select(serverfd+1,&tempfd,NULL,NULL,NULL) < 0){
            perror("select");
            close(serverfd);
            exit(EXIT_FAILURE);
        }

        if(FD_ISSET(STDIN_FD,&tempfd)){
            safe_fgets(buffer,MSG_LENGTH);
            send_msg(serverfd,buffer,username);
            print_my_msg(COLOR_GREEN,buffer);
        }

        if(FD_ISSET(serverfd,&tempfd)){
            handle_server(serverfd,&head);
        }

    }

}

void handle_server(int serverfd,struct list** head){
    //Inizizalizzo il pacchetto
    struct pck pk;
    struct msg_pck *msg = (struct msg_pck*)pk.bytes;

    //Leggo il pacchetto
    if(read(serverfd,&pk,sizeof(struct pck)) <= 0){
        perror("read");
        close(serverfd);
        exit(EXIT_FAILURE);
    }

    //controllo i tipi
    switch(pk.tp){
        case MESSAGE:
            print_other_msg(head,msg->msg,msg->username);
            break;
        default:
            break;
    }
}

void change_username(int serverfd){
    //Inizializzazione delle variabili
    struct username_pck pck;

    while(true){

        printf("inserisci Il tuo username : ");
        safe_fgets(pck.buffer,USERNAME_LENGTH);
        printf("Comunicazione con il server in corso...\n");
    
        if(write(serverfd,&pck,sizeof(struct username_pck)) < 0){
            perror("write");
            close(serverfd);
            exit(EXIT_FAILURE);
        }
  
        //leggo il risultato
        if(read(serverfd,&pck,sizeof(struct username_pck)) <= 0){
            perror("read");
            close(serverfd);
            exit(EXIT_FAILURE);
        }
    
        if(pck.result) break;

        printf("Username NON valido\n");
        wait_for_key_press();
        system("clear");
    }

    communication(serverfd,pck.buffer);

}

///////////////////////////////////////////////////////////////////////////
//Funzioni per l'invio dei pacchetti
void send_msg(int serverfd,char *buffer,char* username){
    //Creo un pacchetto
    struct pck pk;
    struct msg_pck *msgpk = (struct msg_pck*)pk.bytes;

    //Definisco il pacchetto
    pk.tp = MESSAGE;
    memcpy(msgpk->msg,buffer,MSG_LENGTH);
    memcpy(msgpk->username,username,USERNAME_LENGTH);

    //Invio il pacchetto
    if(write(serverfd,&pk,sizeof(struct pck)) < 0){
        perror("Write");
        close(serverfd);
        exit(EXIT_FAILURE);
    }
}

void send_exit(int serverfd){
    //creo il pacchetto
    struct pck pk;

    //Definisco il pacchetto
    pk.tp = EXIT;

    //Invio il pacchetto
    if(write(serverfd,&pk,sizeof(struct pck)) < 0){
        perror("Write");
        close(serverfd);
        exit(EXIT_FAILURE);
    }
}

//////////////////////////////////////////////////////////////////////////
//Funzioni per la parte grafica
void print_my_msg(enum Color color, char* buffer){

    //Prendo la dimensione del terminale
    struct winsize w;
    
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        perror("ioctl");
        return;
    }

    clear_n_lines_up(1+ (strlen(buffer)/w.ws_col));
    print_colored("[you]    : ",color);
    print_colored(buffer,color);
    printf("\n");
    printf(">   ");
    fflush(stdout);
}

void print_other_msg(struct list** head, char* buffer, char* username){
    enum Color color = get_color(head,username);
    printf("\033[2K");
    move_cursor_to_line_start();
    print_colored("[",color);
    print_colored(username,color);
    print_colored("]    : ",color);
    print_colored(buffer,color);
    printf("\n");
    printf(">   ");
    fflush(stdout);
}

void print_init_communication(){
    printf("\n");
    printf("========================================\n");
    printf("|                                      |\n");
    printf("|          CONNESSO ALLA CHAT          |\n");
    printf("|                                      |\n");
    printf("========================================\n\n");
    printf(">   ");
    fflush(stdout);
}

//////////////////////////////////////////////////////////////////////////
//Altre funzioni
int safe_fgets(char *buffer,ssize_t dim){
    if(!buffer || dim < 1) return -1;

    if(fgets(buffer,dim,stdin) != NULL){
        //controllo se è presente un il \n
        if(strchr(buffer,'\n') == NULL){
            clean_input_buffer();
        }

        strip_new_line(buffer);
        return 0;
    }
    return -1;
}

void clean_input_buffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF){}
}

void strip_new_line(char *buffer){
    char *c = strchr(buffer,'\n');
    if(c) *c = '\0';
}

const char* get_color_code(enum Color color) {
    switch (color) {
        case COLOR_RED:     return "\033[31m";
        case COLOR_GREEN:   return "\033[32m";
        case COLOR_YELLOW:  return "\033[33m";
        case COLOR_BLUE:    return "\033[34m";
        case COLOR_MAGENTA: return "\033[35m";
        case COLOR_CYAN:    return "\033[36m";
        case COLOR_WHITE:   return "\033[37m";
        case COLOR_BRIGHT_MAGENTA:  return "\033[95m";
        case COLOR_BRIGHT_RED:      return "\033[91m";
        case COLOR_RESET:   
        default:            return "\033[0m";
    }
}
  
void print_colored(const char *text,enum Color color) {
    printf("%s%s\033[0m", get_color_code(color), text); // \033[0m serve per resettare il colore
}

void move_cursor_to_line_start() {
    printf("\r"); // Carriage return: riporta il cursore all'inizio della riga
}

void clear_n_lines_up(int n) {
    for (int i = 0; i < n; ++i) {
        printf("\033[2K"); // ANSI escape: cancella tutta la linea corrente
        printf("\033[1A"); // ANSI escape: cursore su di una riga
    }
    // Dopo aver cancellato n righe, porta il cursore all'inizio della riga corrente
    printf("\033[2K"); // Pulisci anche la riga su cui ti trovi adesso
    move_cursor_to_line_start();
}

void wait_for_key_press() {
    printf("\nPremi INVIO per continuare...");
    while (getchar() != '\n'); // Aspetta fino a che non arriva un invio
}

/////////////////////////////////////////////////////////////////////////
//Funzioni per la lista
struct client* construct_client(struct list** head,char* username){
    struct client* cli = malloc(sizeof(struct client));
    memcpy(cli->username,username,USERNAME_LENGTH);
    cli->color = get_next_color(head);
    return cli;
}

enum Color get_next_color(struct list** head){
    //prendo la dimensione della lista
    size_t dim = size(head);
    enum Color result = (enum Color)dim%N_COLOR;
    return result;
}

enum Color get_color(struct list** head, char *username){
    struct list* current = *head;

    while(current != NULL){
        if(strcmp(((struct client*)current->arg)->username,username) == 0){
            return ((struct client*)current->arg)->color;
        }
        current = current->next;
    }

    //Se non c'`e lo aggiungo 
    enqueue(head,construct_client(head,username));

    return get_color(head,username);
}