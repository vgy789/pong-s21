#include <ncurses.h>
#include <stdio.h>
#include "pong_common.h"

enum {
    delay_duration = 100,
};

enum colors {
    COLOR_PAIR_DEFAULT = 5,
    COLOR_PAIR_WINNER = 6,
    COLOR_PAIR_BALL = 1,
    COLOR_PAIR_BORDER = 2,
    COLOR_PAIR_NET = 3,
    COLOR_PAIR_PLAYER = 4
};

void initcurses(void);
void print_frame(game_ball ball, int p1_row, int p2_row, game_score score);
void show_winner_screen(game_score score);
void win_scr1(void);
void win_scr2(void);

int main(void) {
    int key_pressed;
    game_score score = {p_init_score, p_init_score};
    int p1_row = p_init_row;
    int p2_row = p_init_row;

    initcurses();

    game_ball ball = {ball_init_column, ball_init_row, 1, 1};

    do {
        clear();
        print_frame(ball, p1_row, p2_row, score);
        refresh();
        ball = ball_movement(ball, p1_row, p2_row);

        if (is_round_won(ball)) {
            score = upd_score(score, ball);
            ball = ball_reset(ball);
            p1_row = p_init_row;
            p2_row = p_init_row;
        }

        key_pressed = getch();
        if (key_pressed != -1) {
            p1_row += p1_control(key_pressed, p1_row);
            p2_row += p2_control(key_pressed, p2_row);
        }

    } while (score.p1 != 21 && score.p2 != 21 && key_pressed != 'q' && key_pressed != 'Q');

    clear();
    if (score.p1 == win_score || score.p2 == win_score) {
        show_winner_screen(score);
    }

    endwin();
    return 0;
}

void initcurses(void) {
    initscr();
    start_color();
    cbreak();  // отключает буферизацию строк и обработку символов стирания
    timeout(delay_duration);  // не ожидать ввод
    keypad(stdscr, TRUE);  // обрабатывать стрелочки как 1 введённый символ
    noecho();              // ввод без эха
    curs_set(0);           // скрыть курсор

    // создать цветовые пары
    init_pair(COLOR_PAIR_BALL, COLOR_RED, COLOR_RED);
    init_pair(COLOR_PAIR_BORDER, COLOR_CYAN, COLOR_CYAN);
    init_pair(COLOR_PAIR_NET, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(COLOR_PAIR_PLAYER, COLOR_GREEN, COLOR_GREEN);
    init_pair(COLOR_PAIR_DEFAULT, -1, -1);
    init_pair(COLOR_PAIR_WINNER, COLOR_YELLOW, COLOR_BLACK);
}

void print_score(game_score score) {
    mvaddch(2, p1_score_pos1, (score.p1 / 10) + '0');
    mvaddch(2, p1_score_pos2, (score.p1 % 10) + '0');
    mvaddch(2, p2_score_pos1, (score.p2 / 10) + '0');
    mvaddch(2, p2_score_pos2, (score.p2 % 10) + '0');
}

void print_frame(game_ball ball, int p1_row, int p2_row, game_score score) {
    for (int row = 0; row < border_width; ++row) {
        for (int col = 0; col < border_length; ++col) {
            if ((row == 0 || row == border_width - 1) || (col == 0 || col == border_length - 1)) {
                attrset(COLOR_PAIR(COLOR_PAIR_BORDER));
            } else if (row == ball.y && col == ball.x) {
                attrset(COLOR_PAIR(COLOR_PAIR_BALL));
            } else if ((row >= p1_row - 1 && row <= p1_row + 1 && col == p1_column) ||
                       (row >= p2_row - 1 && row <= p2_row + 1 && col == p2_column)) {
                attrset(COLOR_PAIR(COLOR_PAIR_PLAYER));
            } else if (col == ball_init_column) {
                attrset(COLOR_PAIR(COLOR_PAIR_NET));
            } else {
                attrset(COLOR_PAIR(COLOR_PAIR_DEFAULT));
            }
            printw(" ");
            attrset(COLOR_PAIR(COLOR_PAIR_DEFAULT));
        }
        printw("\n");
    }
    print_score(score);
}

void show_winner_screen(game_score score) {
    attrset(COLOR_PAIR(COLOR_PAIR_WINNER));
    printf("\n\n\n\n");
    score.p1 == win_score ? win_scr1() : win_scr2();
    refresh();
    timeout(-1);
    getch();
}

void win_scr1(void) {
    printw(WIN_SCR1_TEXT);
}

void win_scr2(void) {
    printw(WIN_SCR2_TEXT);
}
