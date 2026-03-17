#include <stdio.h>
#include <string.h>
#include "undo_redo.h"

void init_stack(stack *s) {
    s->top = -1;
}

int is_empty(stack *s) {
    if (s->top == -1) {
        return 1; // Stack kosong
    } else {
        return 0; // Stack tidak kosong
    }

void push(stack *s, char kertas[100][50]) {
    if (s->top < 9) {
        s->top++;
        strcpy(s->kertas[s->top], kertas);
    } else {
        printf("Stack penuh, tidak bisa push.\n");
    }
}

void pop(stack *s, char kertas[100][50]) {
   if (is_empty(s)=1) {
        printf("Stack kosong, tidak bisa pop.\n");
        return
    } 

    for (int i = 0; i < 100; i++) {
        strcpy(kertas[i], s->kertas[s->top][i]);
    }
    s->top--;
    printf("Undo berhasil.\n");
}