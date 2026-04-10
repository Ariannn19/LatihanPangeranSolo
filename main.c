#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

// Panggil semua file header kelompok
#include "kasfa.h"
#include "arian.h"
#include "file_manager.h"
#include "undo_redo.h"

// Variabel Global Pendukung
char clipboard[MAX_LINES][MAX_LENGTH]; 
int clip_lines = 0;
stack riwayat; 

// Fungsi untuk menyamakan state antara Arian dan Kasfa
void sync_state() {
    linecount = lines; 
}

int main() {
    char pilihan; 
    char menu_edit, menu_analisis;
    char fm_pil;
    char nama_file[50];

    // Persiapan Awal
    if (lines == 0) lines = 1; 
    sync_state();
    init_stack(&riwayat);
    push(&riwayat, text, linecount); 

    while(1) {
        system("cls"); 
        printf("=========================================\n");
        printf("       PANGERAN SOLO TEXT EDITOR\n");
        printf("=========================================\n");
        printf("1. Mode Live Editor (Arian Cursor)\n");
        printf("2. Tampilkan Dokumen\n");
        printf("3. Menu Edit (Copy/Cut/Paste)\n");
        printf("4. Fitur Analisis (Find/Count/Wrap)\n");
        printf("5. File Manager (Save/Load)\n");
        printf("6. Edit Baris Tertentu\n");
        printf("0. Keluar\n");
        printf("-----------------------------------------\n");
        printf("Shortcut: [Ctrl+F] Cari | [Ctrl+S] Save | [Ctrl+L] Load\n");
        printf("=========================================\n");
        printf("Pilih menu atau shortcut: ");
        
        pilihan = _getch(); 

        switch(pilihan) {
            case '1': {
                // Pindah ke baris paling bawah sebelum mulai ngetik
                cursorY = lines - 1;
                if(cursorY < 0) cursorY = 0;
                cursorX = strlen(text[cursorY]);
                
                render(); // Panggil fungsi render bawaan Arian
                
                while(1) {
                    int ch = _getch();
                    
                    if (ch == 224) { // Panah Navigasi
                        ch = _getch();
                        moveCursor(ch);
                    }
                    else if (ch == 27) { // ESC: Selesai
                        sync_state();
                        push(&riwayat, text, linecount);
                        break; 
                    }
                    else if (ch == 26) { // Ctrl+Z: Undo
                        sync_state();
                        pop(&riwayat, text, &linecount);
                        lines = linecount; // Kembalikan state ke Arian
                        if (cursorY >= lines) cursorY = lines - 1; // Jaga kursor gak tembus
                        cursorX = 0;
                    }
                    else if (ch == 25) { // Ctrl+Y: Redo
                        sync_state();
                        redo(&riwayat, text, &linecount);
                        lines = linecount;
                        if (cursorY >= lines) cursorY = lines - 1;
                        cursorX = 0;
                    }
                    else if (ch == '\r') { // Enter
                        enterKey();
                        sync_state();
                        push(&riwayat, text, linecount); 
                    }
                    else if (ch == '\b') { // Backspace
                        backspace();
                    }
                    else if (ch >= 32 && ch <= 126) { // Ngetik biasa
                        insertChar((char)ch);
                    }
                    
                    render(); // Selalu render ulang layar setiap ada tombol ditekan
                }
                break;
            }
            
            case '2':
                printf("\n--- ISI DOKUMEN ---\n");
                for (int i = 0; i < lines; i++) {
                    printf("%d: %s\n", i + 1, text[i]);
                }
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;
            
            case '3':
                printf("\n--- MENU EDIT ---\n");
                printf("a. Copy\nb. Cut\nc. Paste\nd. Lihat Clipboard\nPilih: ");
                menu_edit = _getch();
                printf("%c\n", menu_edit);

                // CATATAN: Pastikan Arian tidak menghapus fungsi copyText, cutText, pasteText 
                // di file arian.c yang baru ya! Kalau dihapus, kode ini bakal error.
                /*
                if (menu_edit == 'a') copyText(text, linecount, clipboard, &clip_lines);
                else if (menu_edit == 'b') {
                    linecount = cutText(text, linecount, clipboard, &clip_lines);
                    lines = linecount;
                    push(&riwayat, text, linecount); 
                } 
                else if (menu_edit == 'c') {
                    linecount = pasteText(text, linecount, clipboard, clip_lines);
                    lines = linecount;
                    push(&riwayat, text, linecount); 
                } 
                else if (menu_edit == 'd') showClipboard(clipboard, clip_lines);
                */
                printf("Fitur dalam penyelarasan...\n");
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;

            case '4':
                sync_state();
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
                getchar(); 

                if (fm_pil == '1') {
                    save_ke_file(nama_file, text, lines);
                    printf("Berhasil disimpan!\n");
                } else if (fm_pil == '2') {
                    lines = load_dari_file(nama_file, text);
                    sync_state();
                    push(&riwayat, text, linecount); 
                }
                
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;

            case '6':
                printf("\n--- EDIT BARIS TERTENTU ---\n");
                if (lines == 0) {
                    printf("Dokumen masih kosong!\n");
                } else {
                    int edit_line;
                    printf("Pilih baris yang mau diedit (1-%d): ", lines);
                    scanf("%d", &edit_line);
                    getchar(); 

                    if (edit_line >= 1 && edit_line <= lines) {
                        printf("Teks lama [%d]: %s\n", edit_line, text[edit_line - 1]);
                        printf("Masukkan teks baru: ");
                        
                        char teks_baru[MAX_LENGTH];
                        fgets(teks_baru, MAX_LENGTH, stdin);
                        teks_baru[strcspn(teks_baru, "\n")] = '\0'; 

                        strcpy(text[edit_line - 1], teks_baru);
                        sync_state();
                        push(&riwayat, text, linecount); 
                        
                        printf("Baris %d berhasil diperbarui!\n", edit_line);
                    } else {
                        printf("Pilihan baris tidak valid!\n");
                    }
                }
                
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;

            case 6: // Ctrl+F
                sync_state();
                printf("\n[Shortcut Ctrl+F] -> Cari Kata\n");
                find(text, linecount);
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;

            case 19: // Ctrl+S
                printf("\n[Shortcut Ctrl+S] -> Simpan File\n");
                printf("Masukkan nama file untuk disimpan: ");
                scanf("%s", nama_file);
                getchar(); 
                save_ke_file(nama_file, text, lines);
                printf("Berhasil disimpan!\n");
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;

            case 12: // Ctrl+L
                printf("\n[Shortcut Ctrl+L] -> Buka File\n");
                printf("Masukkan nama file yang ingin dibuka: ");
                scanf("%s", nama_file);
                getchar(); 
                lines = load_dari_file(nama_file, text);
                sync_state();
                push(&riwayat, text, linecount); 
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