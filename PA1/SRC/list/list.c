#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


struct node{
    int data;
    struct node *next;
};

typedef struct node Nodey;
int listLength = 0;


void insert(Nodey **head, int value){
    if(*head == NULL){
        *head = (Nodey*)malloc(sizeof(Nodey));
        (*head) -> data = value;
        (*head) -> next = NULL;
        listLength++;
    }
    else if(listLength == 0){
        (*head) -> data = value;
        listLength++;
    }
    else{
        if(value < (*head)->data){
            Nodey *new = malloc(sizeof(Nodey));
            new -> data = value;
            new -> next = *head;

            *head = new;
            listLength++;
        }
        else if(value > (*head)->data){
            insert(&(*head)->next, value);
        }
    }
}

void delete(Nodey **head, int value){
    if(*head == NULL){
        return;
    }
    if(value > (*head)->data){
        delete(&(*head)->next, value);
    }
    else if(value == (*head)-> data){
        Nodey *temp = *head;
        *head = (*head) -> next;
        listLength--;
        free(temp);
    }
}

void printList(Nodey *head){
    printf("%d :", listLength);
    if(listLength > 0){
        for(Nodey *temp = head; temp != NULL; temp = temp->next){
            printf(" %d", temp->data);
        }
    }
    printf("\n");
}

void freeList(Nodey *head){
    if(head == NULL){
        return;
    }
    freeList(head->next);
    free(head);
}


int main(int argc, char **argv){
    Nodey *root = malloc (sizeof(Nodey));
    root->next = NULL;
    char letter;
    int digit;
    while(scanf(" %c %d", &letter, &digit) != EOF){

        if(letter == 'i'){
            insert(&root, digit);
        }
        else if(letter == 'd'){
            delete(&root, digit);
        }
        printList(root);
    }

    freeList(root);
    return EXIT_SUCCESS;
}
