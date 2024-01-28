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

    WIN_SCORE = 21,
};

const char* BALL = "\e[39;41m ";
const char* BORDER = "\e[32;46m ";
const char* NET = "\e[39;100m ";
const char* PLAYER = "\e[39;42m ";
const char* SPACE = " ";

typedef struct {
    int x;
    int y;
    int x_vector;
    int y_vector;
} game_ball;

typedef struct {
    int p1;
    int p2;
} game_score;

void cls(void);
void print_frame(game_ball ball, int p1_row, int p2_row, game_score score);
void hide_cursor(void);
void show_cursor(void);
void set_default_color(void);
void win_scr1(void);
void win_scr2(void);
int is_left_row_control_key(char key);
int is_right_row_control_key(char key);
int change_row_position(int current_row_position, char key);

game_ball ball_movement(game_ball ball, int p1_row, int p2_row);
int get_x_direction(int y_position, int x_position, int x_direction, int p1_pos, int p2_pos);
int get_y_direction(int y_position, int y_direction);
game_ball ball_reset(game_ball ball);
game_score upd_score(game_score score, game_ball ball);

_Bool is_round_won(game_ball ball) { return ball.x == 1 || ball.x == BORDER_LENGTH - 1; }

int main(void) {
    int key_pressed;
    game_score score = {P_INIT_SCORE, P_INIT_SCORE};
    int p1_row = P_INIT_ROW;
    int p2_row = P_INIT_ROW;

    hide_cursor();

    game_ball ball = {BALL_INIT_COLUMN, BALL_INIT_ROW, 1, 1};

    do {
        cls();
        print_frame(ball, p1_row, p2_row, score);
        ball = ball_movement(ball, p1_row, p2_row);

        if (is_round_won(ball)) {
            score = upd_score(score, ball);
            ball = ball_reset(ball);
        }

        key_pressed = getchar();

        if (is_left_row_control_key(key_pressed)) {
            p1_row += change_row_position(p1_row, key_pressed);
            print_frame(ball, p1_row, p2_row, score);
        } else if (is_right_row_control_key(key_pressed)) {
            p2_row += change_row_position(p2_row, key_pressed);
            print_frame(ball, p1_row, p2_row, score);
        }

    } while (score.p1 != 21 && score.p2 != 21 && key_pressed != 'q' && key_pressed != 'Q');

    cls();
    if (score.p1 == WIN_SCORE || score.p2 == WIN_SCORE) {
        printf("\e[32;49m");
        score.p1 == WIN_SCORE ? win_scr1() : win_scr2();
    }

    show_cursor();
    set_default_color();
    return 0;
}

void cls(void) { printf("\e[1;1H\e[2J"); }

void print_score(int column, game_score score) {
    if (column == P1_SCORE_POS1)
        printf("%c", (score.p1 / 10) + '0');
    else if (column == P1_SCORE_POS2)
        printf("%c", (score.p1 % 10) + '0');
    else if (column == P2_SCORE_POS1)
        printf("%c", (score.p2 / 10) + '0');
    else if (column == P2_SCORE_POS2)
        printf("%c", (score.p2 % 10) + '0');
}

void print_frame(game_ball ball, int p1_row, int p2_row, game_score score) {
    for (int row = 0; row < BORDER_WIDTH; ++row) {
        for (int col = 0; col < BORDER_LENGTH; ++col) {
            if ((row == 0 || row == BORDER_WIDTH - 1) || (col == 0 || col == BORDER_LENGTH - 1)) {
                printf("%s", BORDER);
            } else if (row == ball.y && col == ball.x) {
                printf("%s", BALL);
            } else if ((row >= p1_row - 1 && row <= p1_row + 1 && col == P1_COLUMN) ||
                       (row >= p2_row - 1 && row <= p2_row + 1 && col == P2_COLUMN)) {
                printf("%s", PLAYER);
            } else if (row == 2 && (col == P1_SCORE_POS1 || col == P1_SCORE_POS2 || col == P2_SCORE_POS1 ||
                                    col == P2_SCORE_POS2)) {
                print_score(col, score);
            } else if (col == BALL_INIT_COLUMN) {
                printf("%s", NET);
            } else {
                printf("%s", SPACE);
            }
            set_default_color();
        }
        printf("\n");
    }
}

void hide_cursor(void) { printf("\e[?25l"); }
void show_cursor(void) { printf("\e[?25h"); }
void set_default_color(void) { printf("\e[39;40m"); };

int get_x_direction(int y_position, int x_position, int x_direction, int p1_pos, int p2_pos) {
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

game_ball ball_movement(game_ball ball, int p1_row, int p2_row) {
    ball.x_vector = get_x_direction(ball.y, ball.x, ball.x_vector, p1_row, p2_row);
    ball.y_vector = get_y_direction(ball.y, ball.y_vector);
    ball.x += ball.x_vector;
    ball.y += ball.y_vector;
    return ball;
}

game_ball ball_reset(game_ball ball) {
    ball.x = BALL_INIT_COLUMN;
    ball.y = BALL_INIT_ROW;
    ball.x_vector *= -1;
    return ball;
}

game_score upd_score(game_score score, game_ball ball) {
    ball.x == 1 ? ++score.p2 : ++score.p1;
    return score;
}

void win_scr1(void) {
    printf("        ########     ##      ##      ## #### ##    ##   \n");
    printf("        ##     ##  ####      ##  ##  ##  ##  ###   ##   \n");
    printf("        ##     ##    ##      ##  ##  ##  ##  ####  ##   \n");
    printf("        ########     ##      ##  ##  ##  ##  ## ## ##   \n");
    printf("        ##           ##      ##  ##  ##  ##  ##  ####   \n");
    printf("        ##           ##      ##  ##  ##  ##  ##   ###   \n");
    printf("        ##         ######     ###  ###  #### ##    ##   ");
}

void win_scr2(void) {
    printf("        ########   #######     ##      ## #### ##    ##   \n");
    printf("        ##     ## ##     ##    ##  ##  ##  ##  ###   ##   \n");
    printf("        ##     ##        ##    ##  ##  ##  ##  ####  ##   \n");
    printf("        ########   #######     ##  ##  ##  ##  ## ## ##   \n");
    printf("        ##        ##           ##  ##  ##  ##  ##  ####   \n");
    printf("        ##        ##           ##  ##  ##  ##  ##   ###   \n");
    printf("        ##        #########     ###  ###  #### ##    ##   ");
}

int is_left_row_control_key(char key) {
    int res = 0;
    if ((key == 'a') || (key == 'A') || (key == 'z') || (key == 'Z')) res = 1;
    return res;
}

int is_right_row_control_key(char key) {
    int res = 0;
    if ((key == 'k') || (key == 'K') || (key == 'm') || (key == 'M')) res = 1;
    return res;
}

int change_row_position(int current_row_position, char key) {
    int new_position = 0;

    if (((key == 'a') || (key == 'A') || (key == 'k') || (key == 'K')) &&
        !(current_row_position <= BORDER_WIDTH - BORDER_WIDTH + 2))
        new_position = -1;
    else if (((key == 'z') || (key == 'Z') || (key == 'm') || (key == 'M')) &&
             !(current_row_position >= BORDER_WIDTH - 3))
        new_position = 1;

    return new_position;
}