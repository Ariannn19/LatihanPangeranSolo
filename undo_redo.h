#ifndef undo_redo_h
#define undo_redo_h

typedef struct {
    char kertas[10][100][50];
    int top;
} stack;

void init_stack(stack *s);
void push(stack *s, char kertas[100][50]);
void pop(stack *s, char kertas[100][50]);
int is_empty(stack *s);

#endif