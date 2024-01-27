#include <stdio.h>

enum {
    BORDER_LENGTH = 80,
    BORDER_WIDTH = 25,

    BALL_INIT_COLUMN = BORDER_LENGTH / 2,
    BALL_INIT_ROW = BORDER_WIDTH / 2,

    P1_COLUMN = 3,
    P2_COLUMN = BORDER_LENGTH - 4,
    P_INIT_ROW = BORDER_WIDTH / 2,
    P_INIT_SCORE = 21,
};

const char BALL = '$';
const char BORDER = '#';
const char NET = '|';
const char PLAYER = '|';
const char SPACE = ' ';

void cls(void);
void game_control(void);

void print_frame(int ball_column, int ball_row, int p1_row, int p2_row, int p1_score, int p2_score);

int main(void) {
    int ball_column = BALL_INIT_COLUMN;
    int ball_row = BALL_INIT_ROW;
    int key_pressed;
    int p1_score = P_INIT_SCORE;
    int p2_score = P_INIT_SCORE;

    while (key_pressed != 'q') {
        cls();
        print_frame(ball_column, ball_row, P_INIT_ROW, P_INIT_ROW, p1_score, p2_score);
        key_pressed = getchar();
    }

    return 0;
}

void cls(void) { printf("\e[1;1H\e[2J"); }

void print_score(int column, int p1_score, int p2_score) {
    switch (column) {
        case 30:
            printf("%c", (p1_score / 10) + 48);
        case 31:
            printf("%c", (p1_score % 10) + 48);
        case 46:
            printf("%c", (p2_score / 10) + 48);
        case 47:
            printf("%c", (p2_score % 10) + 48);
    }
}

void print_frame(int ball_column, int ball_row, int p1_row, int p2_row, int p1_score, int p2_score) {
    for (int row = 0; row < BORDER_WIDTH; ++row) {
        for (int col = 0; col < BORDER_LENGTH; ++col) {
            if ((row == 0 || row == BORDER_WIDTH - 1) || (col == 0 || col == BORDER_LENGTH - 1)) {
                printf("%c", BORDER);
            } else if (row == ball_row && col == ball_column) {
                printf("%c", BALL);
            } else if ((row >= p1_row - 1 && row <= p1_row + 1 && col == P1_COLUMN) ||
                       (row >= p2_row - 1 && row <= p2_row + 1 && col == P2_COLUMN)) {
                printf("%c", PLAYER);
            // } else if (row == 2 && (col == 30 || col == 31 || col == 46 || col == 47)) {
            //    print_score(col, p1_score, p2_score);
            } else if (col == BALL_INIT_COLUMN) {
                printf("%c", NET);
            } else {
                printf("%c", SPACE);
            }
        }
        printf("\n");
    }
}
