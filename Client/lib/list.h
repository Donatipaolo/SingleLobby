#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////////////////////
//Definizione struttura
struct list{
    void* arg;
    struct list* next;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//Funzioni per le liste

//Per aggiungere elementi
int push(struct list** head,void *args);
int enqueue(struct list** head, void *args);
int insert(struct list** head, void* args,size_t position);

//Per eliminare elementi
int pop(struct list** head);
int dequeue(struct list** head);
int eliminate(struct list** head,size_t position);


//Per la ricerca degli elementi
void* top(struct list** head);
void* bottom(struct list** head);
void* at(struct list** head,size_t position);
size_t position(struct list** head,void* value,size_t dim);


//Altro
bool isEmpty(struct list** head);
size_t size(struct list** head);

#endif