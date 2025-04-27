#include "list.h"
#include <stdlib.h>
#include <string.h>

// Aggiunge un elemento in testa alla lista (stile stack)
int push(struct list** head, void *args) {
    if (!head) return -1;

    struct list* new_node = malloc(sizeof(struct list));
    if (!new_node) return -1;

    new_node->arg = args;
    new_node->next = *head;
    *head = new_node;
    return 0;
}

// Aggiunge un elemento in coda alla lista (stile queue)
int enqueue(struct list** head, void *args) {
    if (!head) return -1;

    struct list* new_node = malloc(sizeof(struct list));
    if (!new_node) return -1;

    new_node->arg = args;
    new_node->next = NULL;

    if (!*head) {
        *head = new_node;
        return 0;
    }

    struct list* temp = *head;
    while (temp->next) temp = temp->next;
    temp->next = new_node;
    return 0;
}

// Inserisce un elemento in una posizione specifica
int insert(struct list** head, void* args, size_t position) {
    if (!head) return -1;

    if (position == 0) return push(head, args);

    struct list* temp = *head;
    for (size_t i = 0; i < position - 1; i++) {
        if (!temp) return -1;
        temp = temp->next;
    }

    struct list* new_node = malloc(sizeof(struct list));
    if (!new_node) return -1;

    new_node->arg = args;
    new_node->next = temp ? temp->next : NULL;
    if (temp) temp->next = new_node;
    return 0;
}

// Rimuove il primo elemento (stile stack)
int pop(struct list** head) {
    if (!head || !*head) return -1;

    struct list* temp = *head;
    *head = (*head)->next;
    free(temp);
    return 0;

}

// Rimuove l'ultimo elemento (stile queue)
int dequeue(struct list** head) {
    if (!head || !*head) return -1;

    struct list* temp = *head;
    struct list* prev = NULL;

    while (temp->next) {
        prev = temp;
        temp = temp->next;
    }

    if (prev) {
        prev->next = NULL;
    } else {
        *head = NULL;
    }

    free(temp);
    return 0;
}

// Rimuove un elemento in una posizione specifica
int eliminate(struct list** head, size_t position) {
    if (!head || !*head) return -1;

    if (position == 0) return pop(head);

    struct list* temp = *head;
    struct list* prev = NULL;

    for (size_t i = 0; i < position; i++) {
        if (!temp) return -1;
        prev = temp;
        temp = temp->next;
    }

    if (!temp || !prev) return -1;

    prev->next = temp->next;
    free(temp);
    return 0;
}

// Restituisce il primo elemento
void* top(struct list** head) {
    if (!head || !*head) return NULL;
    return (*head)->arg;
}


// Restituisce l'ultimo elemento
void* bottom(struct list** head) {
    if (!head || !*head) return NULL;

    struct list* temp = *head;
    while (temp->next) temp = temp->next;
    return temp->arg;
}

// Restituisce l'elemento in una posizione (null se non esiste)
void* at(struct list** head, size_t position) {
    if (!head || !*head) return NULL;

    struct list* temp = *head;
    for (size_t i = 0; i < position; i++) {
        if (!temp) return NULL;
        temp = temp->next;
    }

    return temp ? temp->arg : NULL;
}

size_t position(struct list** head, void* value, size_t dim) {
    if (!head || !*head || !value || dim == 0) return (size_t)-1;

    struct list* temp = *head;
    size_t index = 0;

    while (temp) {
        if (temp->arg && memcmp(temp->arg, value, dim) == 0)
            return index;

        temp = temp->next;
        index++;
    }

    return (size_t)-1; // valore non trovato
}

//Altro
bool isEmpty(struct list** head){
    if (!head || !*head) return true;
    return false;
}


size_t size(struct list** head){
    if (!head || !*head) return 0;

    struct list* current = *head;
    size_t dim = 0;

    while(current != NULL){
        dim++;
        current = current->next;
    }

    return dim;
}
