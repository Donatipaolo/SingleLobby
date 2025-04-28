#ifndef CLIENT_H
#define CLIENT_H
//includes and defie
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include "list.h"
#define USERNAME_LENGTH 20
#define MSG_LENGTH 1024
#define MAX_PCK_LENGTH (MSG_LENGTH + USERNAME_LENGTH)
#define STDIN_FD fileno(stdin)
#define N_COLOR 9


//Strutture per la definizione del pacchetto
enum type{
    MESSAGE,
    EXIT
};

enum Color{
    COLOR_CYAN,
    COLOR_YELLOW,
    COLOR_BRIGHT_MAGENTA,
    COLOR_BRIGHT_RED,
    COLOR_RED,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_RESET,
    COLOR_WHITE,
    COLOR_GREEN
};

struct client{
    char username[USERNAME_LENGTH];
    enum Color color;
};

struct pck{
    enum type tp;
    uint8_t bytes[MAX_PCK_LENGTH];
};

struct msg_pck{
    char msg[MSG_LENGTH]; 
    char username[USERNAME_LENGTH];
};

struct username_pck{
    char buffer[USERNAME_LENGTH];
    bool result;
};

///////////////////////////////////////////////////////////////////////////
//Funzioni principali
void communication(int serverfd, char* username);
void handle_server(int serverfd,struct list** head);
void change_username(int serverfd);

///////////////////////////////////////////////////////////////////////////
//Funzioni per l'invio dei pacchetti
void send_msg(int serverfd,char* buffer,char* username);
void send_exit(int serverfd);

//////////////////////////////////////////////////////////////////////////
//Funzioni per la parte grafica
void print_my_msg(enum Color color, char* buffer);
void print_other_msg(struct list** head, char* buffer, char* username);
void print_init_communication();

/////////////////////////////////////////////////////////////////////////
//Altre funzioni
int safe_fgets(char *buffer,ssize_t dim);
void clean_input_buffer();
void strip_new_line(char *buffer);
const char* get_color_code(enum Color color);
void print_colored(const char *text,enum Color color);
void clear_n_lines_up(int n);
void move_cursor_to_line_start();
void wait_for_key_press();

/////////////////////////////////////////////////////////////////////////
//Funzioni per la lista
struct client* construct_client(struct list** head,char* username);
enum Color get_color(struct list** head, char *username);
enum Color get_next_color(struct list** head);

#endif