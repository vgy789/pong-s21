#include <stdio.h>

enum {
    BORDER_LENGTH = 80,
    BORDER_WIDTH = 25,
    
    P1_COLUMN = 3,
    P2_COLUMN = BORDER_LENGTH - 4,

    BALL_INIT_COLUMN = BORDER_LENGTH / 2,
    BALL_INIT_ROW = BORDER_WIDTH / 2,
};

const char PLAYER = '|';
const char BALL = '$';
const char BORDER = '#';
const char SPACE = ' ';

void print_frame(int ball_column, int ball_row, int p1_row, int p2_row);

int main(void) {
    int ball_column = BALL_INIT_COLUMN;
    int ball_row = BALL_INIT_ROW;


    print_frame(ball_column, ball_row, 6, 6);
    // printf("\e[1;1H\e[2J");

    return 0;
}

void print_frame(int ball_column, int ball_row, int p1_row, int p2_row) {
    for (int row = 0; row < BORDER_WIDTH; ++row) {
        for (int col = 0; col < BORDER_LENGTH; ++col) {
            if ((row == 0 || row == BORDER_WIDTH-1) || (col == 0 || col == BORDER_LENGTH-1)) {
                printf("%c", BORDER);
            } else if(row == ball_row && col == ball_column ) { 
                printf("%c", BALL);
            } else if ((row >= p1_row-1 && row <= p1_row+1 && col == P1_COLUMN) || (row >= p2_row-1 && row <= p2_row+1 && col== P2_COLUMN)) {
                printf("%c", PLAYER);
            } else {
                printf("%c", SPACE);
            }
        }
        printf("\n");
    }
}
