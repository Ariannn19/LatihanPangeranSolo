#include <stdio.h>
#include "file_manager.h"

void save_ke_file(char filename[]) 
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Gagal membuka file untuk disimpan.\n");
        return;
    }
    fprintf(file, "Tes dulu banh.\n");
    fclose(file);
}

void load_dari_file(char filename[]) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Gagal membuka file untuk dimuat.\n");
        return;
    }
    fclose(file);
}
