#include <stdio.h>
#include <string.h>
#include "file_manager.h"

void save_ke_file(char filename[], char kertas [100][50], int jumlah_baris) 
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        printf("Gagal membuka file untuk disimpan.\n");
        return;
    }

    for(int i = 0; i < jumlah_baris; i++) {
        fprintf(file, "%s\n", kertas[i]);
    }

    fclose(file);
}

int load_dari_file(char filename[], char kertas [100][50]) 
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("File %s tidak ditemukan.\n", filename);
        return 0;
    }

    char buffer[50];
    int jumlah_baris = 0;

    while(fgets(buffer, sizeof(buffer), file) != NULL && jumlah_baris < 100) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Menghapus newline
        strncpy(kertas[jumlah_baris], buffer, 50);
        jumlah_baris++;
    }

    fclose(file);
    printf("Berhasil memuat %d baris dari file %s.\n", jumlah_baris, filename);
    return jumlah_baris;
}