#include <stdio.h>
#include "file_manager.h"

int main() {
    char filename[100];

    printf("Masukkan nama file untuk disimpan: ");
    scanf("%s", filename);
    save_ke_file(filename);

    printf("Masukkan nama file untuk dimuat: ");
    scanf("%s", filename);
    load_dari_file(filename);

    return 0;
}