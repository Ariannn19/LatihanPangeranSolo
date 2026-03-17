#include <stdio.h>
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

void load_dari_file(char filename[], char kertas [100][50], int *jumlah_baris)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Gagal membuka file untuk dimuat.\n");
        return;
    }

    printf("Isi file: \n");
    char buffer[150];
    while(fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
    fclose(file);
    printf("File berhasil dimuat.\n");
}
