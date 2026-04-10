#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h> // Wajib ditambah untuk fungsi kursor Arian

// Panggil semua file header kelompok
#include "kasfa.h"
#include "arian.h"
#include "file_manager.h"
#include "undo_redo.h"

char text[max_brs][max_chr]; 
char clipboard[MAX_LINES][MAX_LENGTH]; 
int clip_lines = 0;
stack riwayat; 

// ==========================================
// INTEGRASI FITUR KURSOR ARIAN
// ==========================================
int cursorX = 0;
int cursorY = 0;

void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void render_editor() {
    system("cls");
    printf("=========================================\n");
    printf("             LIVE EDITOR MODE            \n");
    printf("=========================================\n");
    printf("[ESC] Selesai | [Ctrl+Z] Undo | [Ctrl+Y] Redo\n");
    printf("[Panah] Pindah Kursor | Pastikan tekan Enter untuk Save Undo\n\n");

    // Print seluruh isi dokumen
    for (int i = 0; i < linecount; i++) {
        gotoxy(0, i + 6); // Mulai print teks di baris ke-6 layar terminal
        printf("%s", text[i]);
    }

    // Batasi pergerakan Y agar tidak melebihi baris yang ada
    if (cursorY >= linecount) cursorY = linecount - 1;
    if (cursorY < 0) cursorY = 0;

    // Batasi pergerakan X agar tidak melebihi panjang teks di baris itu
    int len = strlen(text[cursorY]);
    if (cursorX > len) cursorX = len;
    if (cursorX < 0) cursorX = 0;

    // Posisikan kursor asli terminal ke koordinat yang dituju
    gotoxy(cursorX, cursorY + 6);
}

void moveCursor(int key) {
    int len = strlen(text[cursorY]);
    int newLen;

    if (key == 72) { // UP
        if (cursorY > 0) {
            cursorY--;
            newLen = strlen(text[cursorY]);
            if (cursorX > newLen) cursorX = newLen;
        }
    }
    else if (key == 80) { // DOWN
        if (cursorY < linecount - 1) {
            cursorY++;
            newLen = strlen(text[cursorY]);
            if (cursorX > newLen) cursorX = newLen;
        }
    }
    else if (key == 75) { // LEFT
        if (cursorX > 0) {
            cursorX--;
        }
        else if (cursorY > 0) {
            cursorY--;
            cursorX = strlen(text[cursorY]);
        }
    }
    else if (key == 77) { // RIGHT
        if (cursorX < len) {
            cursorX++;
        }
        else if (cursorY < linecount - 1) {
            cursorY++;
            cursorX = 0;
        }
    }
}

void insertChar(char ch) {
    int len = strlen(text[cursorY]);
    if (cursorX > len) cursorX = len;

    if (len < max_chr - 1) {
        for (int i = len; i >= cursorX; i--) {
            text[cursorY][i + 1] = text[cursorY][i];
        }
        text[cursorY][cursorX] = ch;
        cursorX++;
        return;
    }

    char currentChar = ch;
    int currentY = cursorY;
    int insertX = cursorX;

    while (currentY < max_brs - 1) {
        if (currentY >= linecount) {
            linecount++;
        }

        int currentLen = strlen(text[currentY]);
        if (currentLen < max_chr - 1) {
            for (int i = currentLen; i >= insertX; i--) {
                text[currentY][i + 1] = text[currentY][i];
            }
            text[currentY][insertX] = currentChar;
            break;
        }

        char overflowChar = text[currentY][max_chr - 2];
        for (int i = max_chr - 2; i > insertX; i--) {
            text[currentY][i] = text[currentY][i - 1];
        }
        text[currentY][insertX] = currentChar;
        text[currentY][max_chr - 1] = '\0';

        currentY++;
        currentChar = overflowChar;
        insertX = 0;
    }

    if (cursorX < max_chr - 1) {
        cursorX++;
    } else {
        cursorX = 0;
        if (cursorY < linecount - 1) cursorY++;
    }
}

void deleteChar() {
    int len = strlen(text[cursorY]);

    if (cursorX < len) {
        for (int i = cursorX; i < len; i++) {
            text[cursorY][i] = text[cursorY][i + 1];
        }
    }
    else if (cursorY < linecount - 1) {
        int currLen = len;
        int nextLen = strlen(text[cursorY + 1]);
        int spaceLeft = max_chr - 1 - currLen;
        int copyCount = (nextLen > spaceLeft) ? spaceLeft : nextLen;

        if (copyCount > 0) {
            strncpy(&text[cursorY][currLen], text[cursorY + 1], copyCount);
            text[cursorY][currLen + copyCount] = '\0';
        }

        if (nextLen > copyCount) {
            for (int i = 0; i < nextLen - copyCount; i++) {
                text[cursorY + 1][i] = text[cursorY + 1][copyCount + i];
            }
            text[cursorY + 1][nextLen - copyCount] = '\0';
        } else {
            for (int i = cursorY + 1; i < linecount - 1; i++)
                strcpy(text[i], text[i + 1]);
            text[linecount - 1][0] = '\0';
            linecount--;
        }
    }
}

void backspace_editor() {
    if (cursorX > 0) {
        cursorX--;
        deleteChar();
    }
    else if (cursorY > 0) {
        cursorX = strlen(text[cursorY - 1]);
        cursorY--;
        deleteChar();
    }
}

void enterKey() {
    if (linecount >= max_brs) return;

    char temp[max_chr];
    strcpy(temp, &text[cursorY][cursorX]);
    text[cursorY][cursorX] = '\0';

    for (int i = linecount; i > cursorY; i--) {
        strcpy(text[i], text[i - 1]);
    }
    strcpy(text[cursorY + 1], temp);
    linecount++;
    cursorY++;
    cursorX = 0;
}
// ==========================================


int main() {
    char pilihan; 
    char menu_edit, menu_analisis;
    char fm_pil;
    char nama_file[50];

    // Persiapan Awal
    if(linecount == 0) linecount = 1; // Minimal ada 1 baris kosong untuk Arian Editor
    init_stack(&riwayat);
    push(&riwayat, text, linecount); 

    while(1) {
        system("cls"); 
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
        printf("Shortcut: [Ctrl+F] Cari | [Ctrl+S] Save | [Ctrl+L] Load\n");
        printf("=========================================\n");
        printf("Pilih menu atau shortcut: ");
        
        pilihan = _getch(); 

        switch(pilihan) {
            case '1': {
                // Set posisi awal kursor ke akhir baris terakhir
                cursorY = linecount - 1;
                cursorX = strlen(text[cursorY]);
                
                render_editor();
                
                while(1) {
                    int ch = _getch();
                    
                    if (ch == 224) { // Deteksi Tombol Panah (Windows Arrow Keys API)
                        ch = _getch();
                        moveCursor(ch);
                    }
                    else if (ch == 27) { // ESC
                        push(&riwayat, text, linecount);
                        break; 
                    }
                    else if (ch == 26) { // Ctrl+Z
                        pop(&riwayat, text, &linecount);
                        cursorX = 0; cursorY = 0; // Reset ke awal dokumen setelah Undo
                    }
                    else if (ch == 25) { // Ctrl+Y
                        redo(&riwayat, text, &linecount);
                        cursorX = 0; cursorY = 0; // Reset ke awal dokumen setelah Redo
                    }
                    else if (ch == '\r') { // Enter
                        enterKey();
                        push(&riwayat, text, linecount); // Save Undo statenya saat ganti baris
                    }
                    else if (ch == '\b') { // Backspace
                        backspace_editor();
                    }
                    else if (ch >= 32 && ch <= 126) { // Karakter yang bisa diprint
                        insertChar((char)ch);
                    }
                    
                    render_editor(); // Gambar ulang layarnya setiap habis nekan tombol
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
                getchar();

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
                    getchar(); 

                    if (edit_line >= 1 && edit_line <= linecount) {
                        printf("Teks lama [%d]: %s\n", edit_line, text[edit_line - 1]);
                        printf("Masukkan teks baru: ");
                        
                        char teks_baru[max_chr];
                        fgets(teks_baru, max_chr, stdin);
                        teks_baru[strcspn(teks_baru, "\n")] = '\0'; 

                        strcpy(text[edit_line - 1], teks_baru);
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
                printf("\n[Shortcut Ctrl+F Terdeteksi] -> Cari Kata\n");
                find(text, linecount);
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;

            case 19: // Ctrl+S
                printf("\n[Shortcut Ctrl+S Terdeteksi] -> Simpan File\n");
                printf("Masukkan nama file untuk disimpan (misal: tugas.txt): ");
                scanf("%s", nama_file);
                getchar(); 
                save_ke_file(nama_file, text, linecount);
                printf("Berhasil disimpan!\n");
                printf("\nTekan tombol apa saja untuk kembali...");
                _getch();
                break;

            case 12: // Ctrl+L
                printf("\n[Shortcut Ctrl+L Terdeteksi] -> Buka File\n");
                printf("Masukkan nama file yang ingin dibuka (misal: tugas.txt): ");
                scanf("%s", nama_file);
                getchar(); 
                linecount = load_dari_file(nama_file, text);
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