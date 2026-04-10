#ifndef undo_redo_h
#define undo_redo_h

typedef struct {
    char riwayat_kertas[10][100][256];
    int riwayat_baris[10];
    int top;
    int max_top;
} stack;

void init_stack(stack *s);
void push(stack *s, char kertas_sekarang[100][256], int jumlah_baris);
void pop(stack *s, char kertas_tujuan[100][256], int *jumlah_baris_tujuan);
void redo(stack *s, char kertas_tujuan[100][256], int *jumlah_baris_tujuan);
int is_empty(stack *s);

#endif