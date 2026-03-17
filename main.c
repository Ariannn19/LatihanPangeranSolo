#include <stdio.h>
#include "file_manager.h"

int main() {
    char filename[100];
    char kertas_bohongan[100][50] = {
       "Arikita rina",
       "yorokobi kitto",
       "fitari nara",
       "mitsuki rareru"
    };
    int jumlah_baris = 100;

    printf("Masukkan nama file untuk disimpan: ");
    scanf("%s", filename);
    save_ke_file(filename, kertas_bohongan, jumlah_baris);

    printf("Masukkan nama file untuk dimuat: ");
    scanf("%s", filename);
    load_dari_file(filename, kertas_bohongan, &jumlah_baris);

    return 0;
}