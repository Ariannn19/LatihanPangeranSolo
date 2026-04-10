#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// Panggil semua file header kelompok
#include "kasfa.h"
#include "arian.h"
#include "file_manager.h"
#include "undo_redo.h"

char text[max_brs][max_chr]; 
char clipboard[MAX_LINES][MAX_LENGTH]; 
int clip_lines = 0;
stack riwayat; 

int main() {
    char pilihan; 
    char menu_edit, menu_analisis;
    char fm_pil;
    char nama_file[50];

    // Persiapan Awal
    init_stack(&riwayat);
    push(&riwayat, text, linecount); 

    while(1) {
        system("cls"); // Bersihkan layar utama
        printf("=========================================\n");
        printf("       PANGERAN SOLO TEXT EDITOR\n");
        printf("=========================================\n");
        printf("1. Mode Live Editor (Ketik & Undo/Redo)\n");
        printf("2. Tampilkan Dokumen\n");
        printf("3. Menu Edit (Copy/Cut/Paste)\n");
        printf("4. Fitur Analisis (Find/Count/Wrap)\n");
        printf("5. File Manager (Save/Load)\n");
        printf("6. Edit Baris Tertentu\n");
        printf("0. Keluar\n");
        printf("-----------------------------------------\n");
        printf("Shortcut Menu: [Ctrl+F] Cari Kata\n");
        printf("=========================================\n");
        printf("Pilih menu atau shortcut: ");
        
        pilihan = _getch(); 

        switch(pilihan) {
            case '1': {
                system("cls");
                printf("=========================================\n");
                printf("             LIVE EDITOR MODE            \n");
                printf("=========================================\n");
                printf("[ESC] Selesai & Simpan | [Ctrl+Z] Undo | [Ctrl+Y] Redo\n\n");
                
                for (int i = 0; i < linecount; i++) {
                    printf("%d: %s\n", i + 1, text[i]);
                }
                
                int col = 0; 
                printf("%d: ", linecount + 1);
                
                while(1) {
                    char ch = _getch();
                    
                    if (ch == 27) { // ESC
                        if (col > 0) {
                            text[linecount][col] = '\0';
                            linecount++;
                            push(&riwayat, text, linecount);
                        }
                        break; 
                    }
                    else if (ch == 26) { // Ctrl+Z
                        if (col > 0) {
                            while(col > 0) {
                                printf("\b \b");
                                col--;
                            }
                        } else {
                            pop(&riwayat, text, &linecount);
                            system("cls");
                            printf("=========================================\n");
                            printf("             LIVE EDITOR MODE            \n");
                            printf("=========================================\n");
                            printf("[ESC] Selesai & Simpan | [Ctrl+Z] Undo | [Ctrl+Y] Redo\n\n");
                            for (int i = 0; i < linecount; i++) {
                                printf("%d: %s\n", i + 1, text[i]);
                            }
                            printf("%d: ", linecount + 1);
                        }
                    }
                    else if (ch == 25) { // Ctrl+Y
                        redo(&riwayat, text, &linecount);
                        system("cls");
                        printf("=========================================\n");
                        printf("             LIVE EDITOR MODE            \n");
                        printf("=========================================\n");
                        printf("[ESC] Selesai & Simpan | [Ctrl+Z] Undo | [Ctrl+Y] Redo\n\n");
                        for (int i = 0; i < linecount; i++) {
                            printf("%d: %s\n", i + 1, text[i]);
                        }
                        col = 0;
                        printf("%d: ", linecount + 1);
                    }
                    else if (ch == '\r') { // Enter
                        text[linecount][col] = '\0';
                        linecount++;
                        push(&riwayat, text, linecount); 
                        printf("\n%d: ", linecount + 1);
                        col = 0;
                    }
                    else if (ch == '\b') { // Backspace
                        if (col > 0) {
                            col--;
                            printf("\b \b"); 
                        }
                    }
                    else if (ch >= 32 && ch <= 126) { 
                        if (col < max_chr - 1) {
                            text[linecount][col] = ch;
                            col++;
                            printf("%c", ch); 
                        }
                    }
                }
                break;
            }
            
            case '2':
                printf("\n");
                showText(text, linecount);
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;
            
            case '3':
                printf("\n--- MENU EDIT ---\n");
                printf("a. Copy\nb. Cut\nc. Paste\nd. Lihat Clipboard\nPilih: ");
                menu_edit = _getch();
                printf("%c\n", menu_edit);

                if (menu_edit == 'a') copyText(text, linecount, clipboard, &clip_lines);
                else if (menu_edit == 'b') {
                    linecount = cutText(text, linecount, clipboard, &clip_lines);
                    push(&riwayat, text, linecount); 
                } 
                else if (menu_edit == 'c') {
                    linecount = pasteText(text, linecount, clipboard, clip_lines);
                    push(&riwayat, text, linecount); 
                } 
                else if (menu_edit == 'd') showClipboard(clipboard, clip_lines);
                
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;

            case '4':
                printf("\n--- FITUR ANALISIS ---\n");
                printf("a. Cari Kata\nb. Hitung Kata\nc. Wrap Text\nPilih: ");
                menu_analisis = _getch();
                printf("%c\n", menu_analisis);

                if (menu_analisis == 'a') find(text, linecount);
                else if (menu_analisis == 'b') wordcounter(text, linecount);
                else if (menu_analisis == 'c') wraptext(text, linecount);
                
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;

            case '5':
                printf("\n--- FILE MANAGER ---\n");
                printf("1. Simpan (Save)\n2. Buka (Load)\nPilih: ");
                fm_pil = _getch();
                printf("%c\n", fm_pil);
                
                printf("Masukkan nama file (misal: tugas.txt): ");
                scanf("%s", nama_file);
                getchar(); // Bersihkan sisa enter dari scanf

                if (fm_pil == '1') {
                    save_ke_file(nama_file, text, linecount);
                    printf("Berhasil disimpan!\n");
                } else if (fm_pil == '2') {
                    linecount = load_dari_file(nama_file, text);
                    push(&riwayat, text, linecount); 
                }
                
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;

            case '6':
                printf("\n--- EDIT BARIS TERTENTU ---\n");
                if (linecount == 0) {
                    printf("Dokumen masih kosong! Tidak ada baris yang bisa diedit.\n");
                } else {
                    int edit_line;
                    printf("Pilih baris yang mau diedit (1-%d): ", linecount);
                    scanf("%d", &edit_line);
                    getchar(); // Bersihkan sisa enter dari scanf

                    if (edit_line >= 1 && edit_line <= linecount) {
                        printf("Teks lama [%d]: %s\n", edit_line, text[edit_line - 1]);
                        printf("Masukkan teks baru: ");
                        
                        char teks_baru[max_chr];
                        fgets(teks_baru, max_chr, stdin);
                        teks_baru[strcspn(teks_baru, "\n")] = '\0'; // Hapus karakter newline (enter) dari akhir string

                        strcpy(text[edit_line - 1], teks_baru);
                        push(&riwayat, text, linecount); // Simpan ke riwayat agar bisa di-undo
                        
                        printf("Baris %d berhasil diperbarui!\n", edit_line);
                    } else {
                        printf("Pilihan baris tidak valid!\n");
                    }
                }
                
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;

            case 6: // ASCII 6 adalah Ctrl+F
                printf("\n[Shortcut Ctrl+F Terdeteksi] -> Cari Kata\n");
                find(text, linecount);
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;

            case '0':
                system("cls");
                printf("Menutup aplikasi Pangeran Solo... Sampai jumpa!\n");
                return 0;
        }
    }
    return 0;
}