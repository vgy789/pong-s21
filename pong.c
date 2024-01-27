#include <stdio.h>

enum {
    BORDER_LENGTH = 80,
    BORDER_WIDTH = 25,
    BALL_INIT_COLUMN = BORDER_LENGTH / 2,
    BALL_INIT_ROW = BORDER_WIDTH / 2,

    P1_COLUMN = 3,
    P2_COLUMN = BORDER_LENGTH - 4,
    P_INIT_ROW = BORDER_WIDTH / 2,
    P_INIT_SCORE = 0,

    P1_SCORE_POS1 = 30,
    P1_SCORE_POS2 = 31,
    P2_SCORE_POS1 = 49,
    P2_SCORE_POS2 = 50,
}; 

const char BALL = '$';
const char BORDER = '#';
const char NET = '|';
const char PLAYER = '|';
const char SPACE = ' ';

void cls(void);
void game_control(void);

void print_frame(int ball_column, int ball_row, int p1_row, int p2_row, int p1_score, int p2_score);

int get_x_direction(int x_position, int y_position, int x_direction, int p1_pos, int p2_pos);
int get_y_direction(int y_position, int y_direction);

int main(void) {
    int ball_column = BALL_INIT_COLUMN;
    int ball_row = BALL_INIT_ROW;
    int ball_x_direction = 1;
    int ball_y_direction = 1;

    int key_pressed;
    int p1_score = P_INIT_SCORE;
    int p2_score = P_INIT_SCORE;

    int p1_row = P_INIT_ROW;
    int p2_row = P_INIT_ROW;

    while (key_pressed != 'q') {
        cls();
        print_frame(ball_column, ball_row, P_INIT_ROW, P_INIT_ROW, p1_score, p2_score);

        ball_x_direction = get_x_direction(ball_column, ball_row, ball_x_direction, p1_row, p2_row);
        ball_y_direction = get_y_direction(ball_row, ball_y_direction);
        ball_column += ball_x_direction;
        ball_row += ball_y_direction;

        key_pressed = getchar();
    }

    return 0;
}

void cls(void) { printf("\e[1;1H\e[2J"); }

void print_score(int column, int p1_score, int p2_score) {
    if (column == P1_SCORE_POS1)
        printf("%c", (p1_score / 10) + '0');
    else if (column == P1_SCORE_POS2)
        printf("%c", (p1_score % 10) + '0');
    else if (column == P2_SCORE_POS1)
        printf("%c", (p2_score / 10) + '0');
    else if (column == P2_SCORE_POS2)
        printf("%c", (p2_score % 10) + '0');
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
            } else if (row == 2 && (col == P1_SCORE_POS1 || col == P1_SCORE_POS2 || col == P2_SCORE_POS1 ||
                                    col == P2_SCORE_POS2)) {
                print_score(col, p1_score, p2_score);
            } else if (col == BALL_INIT_COLUMN) {
                printf("%c", NET);
            } else {
                printf("%c", SPACE);
            }
        }
        printf("\n");
    }
}

int get_x_direction(int x_position, int y_position, int x_direction, int p1_pos, int p2_pos) {
    int direction = x_direction;
    if ((x_position == P1_COLUMN || x_position == P2_COLUMN) &&
        (y_position == p1_pos || y_position == p1_pos + 1 || y_position == p1_pos - 1 ||
         y_position == p2_pos || y_position == p2_pos + 1 || y_position == p2_pos - 1))
        direction *= -1;
    return direction;
}

int get_y_direction(int y_position, int y_direction) {
    int direction = y_direction;
    if (y_position == (BORDER_WIDTH - 2) || y_position == 1) direction *= -1;
    return direction;
}
