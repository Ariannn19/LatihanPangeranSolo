#include <stdio.h>
#include <string.h>
#include "undo_redo.h"

void init_stack(stack *s) {
    s->top = -1;
    s->max_top = -1; // Inisialisasi max_top dengan -1, akan diupdate saat push
}

int is_empty(stack *s) {
    if (s->top == -1) {
        return 1; // Stack kosong
    } else {
        return 0; // Stack tidak kosong
    }
}

void push(stack *s, char kertas_sekarang[100][100], int jumlah_baris) {
    if (s->top >= 9) { // Pastikan tidak melebihi kapasitas stack}
        printf("Riwayat penuh.\n");
        return;
    }
    s->top++;
    s->max_top = s->top; // Update max_top setiap kali push
    s->riwayat_baris[s->top] = jumlah_baris;

    for (int i = 0; i < jumlah_baris; i++) {
        strcpy(s->riwayat_kertas[s->top][i], kertas_sekarang[i]);
    }
}

void pop(stack *s, char kertas_tujuan[100][100], int *jumlah_baris_tujuan) {
    if (is_empty(s) == 1) {
        printf("Tidak ada riwayat untuk di-undo.\n");
        return;
    }

    s->top--; // Kurangi top untuk mendapatkan indeks yang benar

    if(s->top == -1) {
        *jumlah_baris_tujuan = 0; // Jika stack kosong setelah pop, set jumlah baris tujuan ke 0
        return;
    }

    *jumlah_baris_tujuan = s->riwayat_baris[s->top];
    for (int i = 0; i < *jumlah_baris_tujuan; i++) {
        strcpy(kertas_tujuan[i], s->riwayat_kertas[s->top][i]);
    }
}

void redo(stack *s, char kertas_tujuan[100][100], int *jumlah_baris_tujuan) {
    if (s->top >= s->max_top) {
        printf("Tidak ada riwayat untuk di-redo.\n");
        return;
    }
        
    s->top++; // Naikkan top untuk mendapatkan indeks yang benar

     *jumlah_baris_tujuan = s->riwayat_baris[s->top];
     
    for (int i = 0; i < *jumlah_baris_tujuan; i++) {
        strcpy(kertas_tujuan[i], s->riwayat_kertas[s->top][i]);
    }
}