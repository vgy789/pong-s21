#include <stdio.h>

enum {
    BORDER_LENGTH = 80,
    BORDER_WIDTH = 25,
    
    P1_COL_POS = 3,
    P2_COL_POS = BORDER_LENGTH - 3,

    BALL_COL_POS = BORDER_LENGTH / 2,
    BALL_ROW_POS = BORDER_WIDTH / 2,
};

const char PLAYER = '|';
const char BALL = '$';
const char BORDER = '#';
const char SPACE = ' ';

void print_frame(void);

int main(void) {
    print_frame();
    // printf("\e[1;1H\e[2J");
    return 0;
}

void print_frame(void) {
    for (int row = 0; row < BORDER_WIDTH; ++row) {
        for (int col = 0; col < BORDER_LENGTH; ++col) {
            if ((row == 0 || row == BORDER_WIDTH-1) || (col == 0 || col == BORDER_LENGTH-1)) {
                printf("%c", BORDER);
            } else {
                printf("%c", SPACE);
            }
        }
        printf("\n");
    }
}
