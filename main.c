#include <conio.h>
#include "arian.h"

int main() {
    int ch;
    while (1) {
        render();

        ch = _getch();

        if (ch == 27) break; // ESC keluar

        if (ch == 224) {
            ch = _getch();

            if (ch == 83) {         // DELETE key
                deleteChar();
            } else {
                moveCursor(ch);     // arrow keys: UP/DOWN/LEFT/RIGHT
            }
        }
        else if (ch == 8) {
            backspace();
        }
        else if (ch == 13) {
            enterKey();
        }
        else {
            insertChar(ch);
        }
    }

    return 0;
}