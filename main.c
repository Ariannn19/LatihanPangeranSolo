#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "kasfa.h"
#include "arian.h"
#include "file_manager.h"
#include "undo_redo.h"

int main() {
    char pilihan; 
    char nama_file[50];

    // Persiapan Awal
    if(linecount == 0) linecount = 1; 

    while(1) {
        system("cls"); 
        printf("=========================================\n");
        printf("       PANGERAN SOLO TEXT EDITOR         \n");
        printf("=========================================\n");
        printf("1. New File\n");
        printf("2. Open File\n");
        printf("3. Help\n");
        printf("4. Keluar\n");
        printf("=========================================\n");
        printf("Pilih menu (1-4): ");
        
        pilihan = _getch(); 

        // GABUNGAN MENU 1 (NEW) DAN 2 (OPEN)
        if (pilihan == '1' || pilihan == '2') {
            
            if (pilihan == '1') {
                // SETUP NEW FILE
                cursorX = 0; cursorY = 0; linecount = 1;
                text[0][0] = '\0'; 
                
                init_stack(&riwayat);
                push(&riwayat, text, linecount); 
            } 
            else if (pilihan == '2') {
                // SETUP OPEN FILE
                system("cls");
                printf("Masukkan nama file untuk dibuka: ");
                scanf("%s", nama_file);
                getchar(); // Membersihkan sisa enter

                int loadedLines = load_dari_file(nama_file, text);
                
                if (loadedLines > 0) {
                    linecount = loadedLines; // Update jumlah baris
                    cursorX = 0; cursorY = 0;
                    
                    init_stack(&riwayat);
                    push(&riwayat, text, linecount); 
                } else {
                    printf("\nGagal memuat file. Tekan tombol apa saja untuk kembali...");
                    _getch();
                    continue; // Batal masuk editor, kembali ke awal while menu
                }
            }

            // ==========================================
            // LIVE EDITOR LOOP (Langsung di dalam main)
            // ==========================================
            render_editor();
            
            while(1) {
                int ch = _getch();
                
                if (ch == 224 || ch == 0) { // Tombol Panah
                    ch = _getch();
                    moveCursor(ch);
                }
                else if (ch == 27) { // ESC: Keluar ke Menu Utama
                    push(&riwayat, text, linecount);
                    break; // Memecah loop editor, kembali ke loop menu
                }
                else if (ch == 26) { // Ctrl+Z: Undo
                    pop(&riwayat, text, &linecount);
                    cursorX = 0; cursorY = 0; 
                }
                else if (ch == 25) { // Ctrl+Y: Redo
                    redo(&riwayat, text, &linecount);
                    cursorX = 0; cursorY = 0; 
                }
                else if (ch == 19) { // Ctrl+S: Save
                    system("cls");
                    printf("--- MODE SAVE ---\nMasukkan nama file (misal: tugas.txt): ");
                    scanf("%s", nama_file);
                    getchar(); 
                    save_ke_file(nama_file, text, linecount);
                    printf("File %s berhasil disimpan! Tekan apa saja untuk lanjut...", nama_file);
                    _getch();
                }
                else if (ch == 6) { // Ctrl+F: Find
                    system("cls");
                    printf("--- MODE CARI KATA ---\n");
                    find(text, linecount);
                    printf("\nTekan tombol apa saja untuk kembali ke editor...");
                    _getch();
                }
                else if (ch == 23) { // Ctrl+W: Word Counter
                    system("cls");
                    printf("--- ANALISIS DOKUMEN ---\n");
                    wordcounter(text, linecount);
                    printf("\n\nTekan tombol apa saja untuk kembali ke editor...");
                    _getch();
                }
                else if (ch == 18) { // Ctrl+R: Wrap Text
                    system("cls");
                    printf("--- TAMPILAN WRAP TEXT ---\n");
                    wraptext(text, linecount);
                    printf("\nTekan tombol apa saja untuk kembali ke editor...");
                    _getch();
                }
                else if (ch == '\r') { // Enter
                    enterKey();
                    push(&riwayat, text, linecount); 
                }
                else if (ch == '\b') { // Backspace
                    backspace_editor();
                }
                else if (ch >= 32 && ch <= 126) { // Ketik huruf biasa
                    insertChar((char)ch);
                }
                
                render_editor(); // Selalu render ulang setelah aksi
            }
            // ==========================================
            // AKHIR DARI LIVE EDITOR LOOP
            // ==========================================
        } 
        else if (pilihan == '3') {
            // HELP
            system("cls");
            printf("=== PANDUAN PENGGUNAAN ===\n");
            printf("Navigasi:\n");
            printf("- [Panah] Pindah Kursor\n");
            printf("- [ESC]   Keluar dari Editor ke Menu Utama\n\n");
            printf("Aksi Cepat (Shortcut):\n");
            printf("- [Ctrl+S] Save File\n");
            printf("- [Ctrl+Z] Undo (Batal)\n");
            printf("- [Ctrl+Y] Redo (Ulangi)\n");
            printf("- [Ctrl+F] Cari Kata (Find)\n");
            printf("- [Ctrl+W] Hitung Kata & Karakter (Word Count)\n");
            printf("- [Ctrl+R] Tampilan Wrap Text\n");
            printf("\nTekan tombol apa saja untuk kembali...");
            _getch();
        } 
        else if (pilihan == '4') {
            // KELUAR
            system("cls");
            printf("Terima kasih telah menggunakan Pangeran Solo Text Editor!\n");
            break; // Keluar dari aplikasi sepenuhnya
        }
    }
    
    return 0;
}