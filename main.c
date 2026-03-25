#include <stdio.h>
#include <string.h>
#include "file_manager.h"

int main() {
    char kertas[100][50];
    int jumlah_baris = 0;
    char filename[100];
    char pilihan;

    printf("Selamat datang di text editor sederhana!\n");
    printf("Apakah anda ingin Load file(L) atau Simpan file(S)? (L/S): ");
    scanf(" %c", &pilihan);

    getchar(); // Membersihkan newline dari input sebelumnya

    if(pilihan == 'L' || pilihan == 'l') {
        printf("Masukkan nama file untuk dimuat: ");
        scanf("%s", filename);
        getchar(); // Membersihkan newline dari input sebelumnya
        jumlah_baris = load_dari_file(filename, kertas);

        if(jumlah_baris > 0) {
            printf("Isi file: \n");

            for(int i = 0; i < jumlah_baris; i++) {
                printf("[%d] %s\n", i, kertas[i]);
            }

            int line_to_edit;
            printf("Masukkan nomor baris yang ingin diedit (ketik -1 untuk tidak mengedit): ");
            scanf("%d", &line_to_edit);
            getchar(); // Membersihkan newline dari input sebelumnya

            if (line_to_edit >= 0 && line_to_edit < jumlah_baris) {
                printf("Masukkan teks baru untuk baris %d: ", line_to_edit);
                fgets(kertas[line_to_edit], sizeof(kertas[line_to_edit]), stdin);
                kertas[line_to_edit][strcspn(kertas[line_to_edit], "\n")] = '\0'; // Menghapus newline
                printf("Baris %d telah diperbarui.\n", line_to_edit);
            } else {
                printf("Tidak ada perubahan yang dilakukan.\n");
            }
        }
    } else {
        printf("Masukkan nama file untuk disimpan: ");
        scanf("%s", filename);
        getchar(); // Membersihkan newline dari input sebelumnya

        strcpy(kertas[0], "Arikita rina");
        strcpy(kertas[1], "yorokobi kitto");
        strcpy(kertas[2], "fitari nara");
        strcpy(kertas[3], "mitsuki rareru");
        strcpy(kertas[4], "sawagashi hibi ni");
        strcpy(kertas[5], "waraenai kimi ni");
        strcpy(kertas[6], "omoi tsuku kagiri mabushii asu wo");
        strcpy(kertas[7], "akenai yoru ni ochite yuku maeni");
        jumlah_baris = 8;
    }

    if (jumlah_baris > 0) {
        save_ke_file(filename, kertas, jumlah_baris);
        printf("File berhasil disimpan.\n");
    } else {
        printf("Tidak ada data untuk disimpan.\n");
    }
    printf("Terima kasih telah menggunakan text editor sederhana ini!\n");
    return 0;
}