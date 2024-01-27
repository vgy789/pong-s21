#include <stdio.h>

enum {
    BORDER_LENGTH = 80,
    BORDER_WIDTH = 25,

    P1_COLUMN = 3,
    P2_COLUMN = BORDER_LENGTH - 4,

    BALL_INIT_COLUMN = BORDER_LENGTH / 2,
    BALL_INIT_ROW = BORDER_WIDTH / 2,
    P_INIT_ROW = BORDER_WIDTH / 2,
};

const char PLAYER = '|';
const char BALL = '$';
const char BORDER = '#';
const char SPACE = ' ';
const char NET = '|';

void print_frame(int ball_column, int ball_row, int p1_row, int p2_row);
int get_x_direction(int x_position, int x_direction);
int get_y_direction(int y_position, int y_direction);

int main(void) {
    int ball_column = BALL_INIT_COLUMN;
    int ball_row = BALL_INIT_ROW;
    int ball_x_direction = 1;
    int ball_y_direction = 1;
    int key_pressed;

    while (key_pressed != 'q') {
        print_frame(ball_column, ball_row, P_INIT_ROW, P_INIT_ROW);
        ball_x_direction = get_x_direction(ball_column, ball_x_direction);
        ball_y_direction = get_y_direction(ball_row, ball_y_direction);
        ball_row += ball_y_direction;
        ball_column += ball_x_direction;

        key_pressed = getchar();
        printf("\e[1;1H\e[2J");
    }

    return 0;
}

void print_frame(int ball_column, int ball_row, int p1_row, int p2_row) {
    for (int row = 0; row < BORDER_WIDTH; ++row) {
        for (int col = 0; col < BORDER_LENGTH; ++col) {
            if ((row == 0 || row == BORDER_WIDTH - 1) || (col == 0 || col == BORDER_LENGTH - 1)) {
                printf("%c", BORDER);
            } else if (row == ball_row && col == ball_column) {
                printf("%c", BALL);
            } else if ((row >= p1_row - 1 && row <= p1_row + 1 && col == P1_COLUMN) ||
                       (row >= p2_row - 1 && row <= p2_row + 1 && col == P2_COLUMN)) {
                printf("%c", PLAYER);
            } else if (col == BALL_INIT_COLUMN) {
                printf("%c", NET);
            } else {
                printf("%c", SPACE);
            }
        }
        printf("\n");
    }
}

int get_x_direction(int x_position, int x_direction) {
    int direction = x_direction;
    if (x_position == (BORDER_LENGTH - 3) || x_position == 3) direction *= -1;
    return direction;
}

int get_y_direction(int y_position, int y_direction) {
    int direction = y_direction;
    if (y_position == (BORDER_WIDTH - 2) || y_position == 1) direction *= -1;
    return direction;
}