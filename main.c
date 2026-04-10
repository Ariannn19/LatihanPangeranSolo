#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h> 
#include "kasfa.h"
#include "arian.h"
#include "file_manager.h"
#include "undo_redo.h"

stack riwayat; 

int main() {
    char pilihan; 
    char nama_file[50];
    
    // Sinkronisasi dengan variabel 'lines' milik arian.c
    if(lines == 0) lines = 1; 

    while(1) {
        system("cls"); 
        
        printf("\n");
        printf("  =======================================\n");
        printf("  ||                                   ||\n");
        printf("  ||     PANGERAN SOLO TEXT EDITOR     ||\n");
        printf("  ||                                   ||\n");
        printf("  =======================================\n");
        printf("  ||                                   ||\n");
        printf("  ||  [1] New File                     ||\n");
        printf("  ||  [2] Open File                    ||\n");
        printf("  ||  [3] Help & Shortcuts             ||\n");
        printf("  ||  [4] Exit                         ||\n");
        printf("  ||                                   ||\n");
        printf("  =======================================\n");
        printf("  Pilihan Anda (1-4): ");
        
        pilihan = _getch(); 

        if (pilihan == '1' || pilihan == '2') {
            
            if (pilihan == '1') {
                cursorX = 0; cursorY = 0; lines = 1;
                text[0][0] = '\0'; 
                
                init_stack(&riwayat);
                push(&riwayat, text, lines); 
            } 
            else if (pilihan == '2') {
                system("cls");
                printf("\nMasukkan nama file untuk dibuka: ");
                scanf("%s", nama_file);
                getchar(); 

                int loadedLines = load_dari_file(nama_file, text);
                
                if (loadedLines > 0) {
                    lines = loadedLines; 
                    cursorX = 0; cursorY = 0;
                    
                    init_stack(&riwayat);
                    push(&riwayat, text, lines); 
                } else {
                    printf("\nGagal memuat file. Tekan tombol apa saja untuk kembali...");
                    _getch();
                    continue; 
                }
            }

            // ==========================================
            // 2. LAYAR PETUNJUK (SPLASH SCREEN)
            // ==========================================
            system("cls");
            printf("\n");
            printf("  ============================================================\n");
            printf("  ||                 MEMASUKI MODE EDITOR                   ||\n");
            printf("  ============================================================\n");
            printf("  || PETUNJUK SHORTCUT:                                     ||\n");
            printf("  || - [ESC]    : Keluar ke Menu Utama dan Simpan Riwayat   ||\n");
            printf("  || - [Ctrl+S] : Simpan File (Save)                        ||\n");
            printf("  || - [Ctrl+Z] : Batal (Undo)                              ||\n");
            printf("  || - [Ctrl+Y] : Ulangi (Redo)                             ||\n");
            printf("  || - [Ctrl+F] : Cari Kata (Find)                          ||\n");
            printf("  || - [Ctrl+W] : Hitung Kata & Karakter (Word Counter)     ||\n");
            printf("  || - [Ctrl+R] : Tampilan Wrap Text                        ||\n");
            printf("  ============================================================\n");
            printf("  Tekan tombol apa saja untuk mulai mengetik...");
            _getch(); // Aplikasi berhenti sejenak sampai user memencet tombol

            // Masuk ke kanvas editor
            render();
            
            while(1) {
                int ch = _getch();
                
                if (ch == 224 || ch == 0) { 
                    ch = _getch();
                    moveCursor(ch);
                }
                else if (ch == 27) { 
                    push(&riwayat, text, lines);
                    break; 
                }
                else if (ch == 26) { 
                    pop(&riwayat, text, &lines);
                    cursorX = 0; cursorY = 0; 
                }
                else if (ch == 25) { 
                    redo(&riwayat, text, &lines);
                    cursorX = 0; cursorY = 0; 
                }
                else if (ch == 19) { 
                    system("cls");
                    printf("\n--- MODE SAVE ---\nMasukkan nama file (misal: tugas.txt): ");
                    scanf("%s", nama_file);
                    getchar(); 
                    save_ke_file(nama_file, text, lines);
                    printf("File %s berhasil disimpan! Tekan apa saja untuk lanjut...", nama_file);
                    _getch();
                }
                else if (ch == 6) { 
                    system("cls");
                    printf("\n--- MODE CARI KATA ---\n");
                    find(text, lines); 
                    printf("\nTekan tombol apa saja untuk kembali ke editor...");
                    _getch();
                }
                else if (ch == 23) { 
                    system("cls");
                    printf("\n--- ANALISIS DOKUMEN ---\n");
                    wordcounter(text, lines);
                    printf("\n\nTekan tombol apa saja untuk kembali ke editor...");
                    _getch();
                }
                else if (ch == 18) { 
                    system("cls");
                    printf("\n--- TAMPILAN WRAP TEXT ---\n");
                    wraptext(text, lines);
                    printf("\nTekan tombol apa saja untuk kembali ke editor...");
                    _getch();
                }
                else if (ch == '\r') { 
                    enterKey();
                    push(&riwayat, text, lines); 
                }
                else if (ch == '\b') { 
                    backspace();
                }
                else if (ch >= 32 && ch <= 126) { 
                    insertChar((char)ch);
                }
                
                render(); 
            }
        } 
        else if (pilihan == '3') {
            system("cls");
            printf("\n  === PANDUAN PENGGUNAAN ===\n");
            printf("  Navigasi:\n");
            printf("  - [Panah] Pindah Kursor\n");
            printf("  - [ESC]   Keluar dari Editor ke Menu Utama\n\n");
            printf("  Aksi Cepat (Shortcut):\n");
            printf("  - [Ctrl+S] Save File\n");
            printf("  - [Ctrl+Z] Undo (Batal)\n");
            printf("  - [Ctrl+Y] Redo (Ulangi)\n");
            printf("  - [Ctrl+F] Cari Kata (Find)\n");
            printf("  - [Ctrl+W] Hitung Kata & Karakter (Word Count)\n");
            printf("  - [Ctrl+R] Tampilan Wrap Text\n");
            printf("\n  Tekan tombol apa saja untuk kembali...");
            _getch();
        } 
        else if (pilihan == '4') {
            system("cls");
            printf("\n  Terima kasih telah menggunakan Pangeran Solo Text Editor!\n");
            break; 
        }
    }
    
    return 0;
}