#ifndef kasfa_H
#define kasfa_H
#define max_brs 100
#define max_chr 256

extern char text[max_brs][max_chr];
extern int linecount;

void displaytext ();
void find (char text[][max_chr],int linecount);
void wordcounter (char text[][max_chr], int linecount);
void wraptext(char text[][max_chr], int linecount);

#endif