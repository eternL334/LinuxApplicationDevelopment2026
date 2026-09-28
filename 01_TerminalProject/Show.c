#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>

void putline(WINDOW *w, int y, char *s) {
    for (int x = 0; x < getmaxx(w)-1 && s[x] && s[x] != '\n'; x++)
        mvwaddch(w, y, x, isprint((unsigned char)s[x]) ? (unsigned char)s[x] : ' ');
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 1;
    }

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    if (LINES < 3 || COLS < 5) {
        endwin();
        fclose(f);
        return 1;
    }
    box(stdscr, 0, 0);
    mvaddnstr(0, 2, argv[1], COLS-4);
    refresh();
    WINDOW *w = newwin(LINES-2, COLS-2, 1, 1);
    scrollok(w, TRUE);
    keypad(w, TRUE);

    char *s = NULL;
    size_t n = 0;
    for (int y = 0; y < getmaxy(w) && getline(&s, &n, f) != -1; y++)
        putline(w, y, s);
    wrefresh(w);

    int c;
    while ((c = wgetch(w)) != 27) {
        if (c == ' ' && getline(&s, &n, f) != -1) {
            wscrl(w, 1);
            putline(w, getmaxy(w)-1, s);
            wrefresh(w);
        }
    }
    free(s);
    fclose(f);
    delwin(w);
    endwin();
    return 0;
}
