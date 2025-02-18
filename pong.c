#include <stdio.h>

#define COLOR_BALL "\e[31;41m"     // red on red
#define COLOR_BORDER "\e[36;46m"   // cyan on cyan
#define COLOR_NET "\e[90;100m"     // gray on gray
#define COLOR_PLAYER "\e[32;42m"   // green on green
#define COLOR_DEFAULT "\e[39;40m"  // default
#define COLOR_WINNER "\e[33;40m"   // yellow on black
#define SPACE " "

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

enum {
    border_length = 80,
    border_width = 25,
    ball_init_column = border_length / 2,
    ball_init_row = border_width / 2,

    p1_column = 3,
    p2_column = border_length - 4,
    p_init_row = border_width / 2,
    p_init_score = 0,

    p1_score_pos1 = 30,
    p1_score_pos2 = 31,
    p2_score_pos1 = 49,
    p2_score_pos2 = 50,

    win_score = 21,
};

const char* ch_ball = COLOR_BALL SPACE;
const char* border = COLOR_BORDER SPACE;
const char* ch_net = COLOR_NET SPACE;
const char* player = COLOR_PLAYER SPACE;
const char* space = SPACE;

void cls(void);
void print_frame(game_ball ball, int p1_row, int p2_row, game_score score);
void hide_cursor(void);
void show_cursor(void);
void set_default_color(void);
void win_scr1(void);
void win_scr2(void);
int p1_control(char key, int current_pos);
int p2_control(char key, int current_pos);
void show_winner_screen(game_score score);

game_ball ball_movement(game_ball ball, int p1_row, int p2_row);
int get_x_direction(int y_position, int y_direction, int x_position, int x_direction, int p1_pos, int p2_pos);
int get_y_direction(int y_position, int y_direction, int x_position, int p1_pos, int p2_pos);
game_ball ball_reset(game_ball ball);
game_score upd_score(game_score score, game_ball ball);

_Bool is_round_won(game_ball ball) { return ball.x == 1 || ball.x == border_length - 1; }

int main(void) {
    int key_pressed;
    game_score score = {p_init_score, p_init_score};
    int p1_row = p_init_row;
    int p2_row = p_init_row;

    hide_cursor();

    game_ball ball = {ball_init_column, ball_init_row, 1, 1};

    do {
        cls();
        print_frame(ball, p1_row, p2_row, score);
        ball = ball_movement(ball, p1_row, p2_row);

        if (is_round_won(ball)) {
            score = upd_score(score, ball);
            ball = ball_reset(ball);
        }

        key_pressed = getchar();
        if (key_pressed != -1) {
            p1_row += p1_control(key_pressed, p1_row);
            p2_row += p2_control(key_pressed, p2_row);
        }
    } while (score.p1 != 21 && score.p2 != 21 && key_pressed != 'q' && key_pressed != 'Q');

    cls();
    if (score.p1 == win_score || score.p2 == win_score) {
        show_winner_screen(score);
    }

    show_cursor();
    set_default_color();
    return 0;
}

void cls(void) { printf("\e[1;1H\e[2J"); }

void print_score(int column, game_score score) {
    if (column == p1_score_pos1)
        printf("%c", (score.p1 / 10) + '0');
    else if (column == p1_score_pos2)
        printf("%c", (score.p1 % 10) + '0');
    else if (column == p2_score_pos1)
        printf("%c", (score.p2 / 10) + '0');
    else if (column == p2_score_pos2)
        printf("%c", (score.p2 % 10) + '0');
}

void print_frame(game_ball ball, int p1_row, int p2_row, game_score score) {
    for (int row = 0; row < border_width; ++row) {
        for (int col = 0; col < border_length; ++col) {
            if ((row == 0 || row == border_width - 1) || (col == 0 || col == border_length - 1)) {
                printf("%s", border);
            } else if (row == ball.y && col == ball.x) {
                printf("%s", ch_ball);
            } else if ((row >= p1_row - 1 && row <= p1_row + 1 && col == p1_column) ||
                       (row >= p2_row - 1 && row <= p2_row + 1 && col == p2_column)) {
                printf("%s", player);
            } else if (row == 2 && (col == p1_score_pos1 || col == p1_score_pos2 || col == p2_score_pos1 ||
                                    col == p2_score_pos2)) {
                print_score(col, score);
            } else if (col == ball_init_column) {
                printf("%s", ch_net);
            } else {
                printf("%s", space);
            }
            set_default_color();
        }
        printf("\n");
    }
}

void hide_cursor(void) { printf("\e[?25l"); }
void show_cursor(void) { printf("\e[?25h"); }
void set_default_color(void) { printf(COLOR_DEFAULT); };

int get_x_direction(int y_position, int y_direction, int x_position, int x_direction, int p1_pos,
                    int p2_pos) {
    int direction = x_direction;
    if (x_position == p1_column + 1) {
        if (y_position <= p1_pos + 2 && y_position >= p1_pos - 2) {
            if ((y_position <= p1_pos + 1 && y_position >= p1_pos - 1) ||
                (y_position == p1_pos + 2 && y_direction == -1) ||
                (y_position == p1_pos - 2 && y_direction == 1))
                direction *= -1;
        }
    }

    if (x_position == p2_column - 1) {
        if (y_position <= p2_pos + 2 && y_position >= p2_pos - 2) {
            if ((y_position <= p2_pos + 1 && y_position >= p2_pos - 1) ||
                (y_position == p2_pos + 2 && y_direction == -1) ||
                (y_position == p2_pos - 2 && y_direction == 1))
                direction *= -1;
        }
    }

    return direction;
}

int get_y_direction(int y_position, int y_direction, int x_position, int p1_pos, int p2_pos) {
    int direction = y_direction;
    if (y_position == (border_width - 2) || y_position == 1) direction *= -1;
    if (x_position == p1_column + 1) {
        if ((y_position == p1_pos + 2 && y_direction == -1) || (y_position == p1_pos - 2 && y_direction == 1))
            direction *= -1;
    }

    if (x_position == p2_column - 1) {
        if ((y_position == p2_pos + 2 && y_direction == -1) || (y_position == p2_pos - 2 && y_direction == 1))
            direction *= -1;
    }

    return direction;
}

game_ball ball_movement(game_ball ball, int p1_row, int p2_row) {
    ball.x_vector = get_x_direction(ball.y, ball.y_vector, ball.x, ball.x_vector, p1_row, p2_row);
    ball.y_vector = get_y_direction(ball.y, ball.y_vector, ball.x, p1_row, p2_row);
    ball.x += ball.x_vector;
    ball.y += ball.y_vector;
    return ball;
}

game_ball ball_reset(game_ball ball) {
    ball.x = ball_init_column;
    ball.y = ball_init_row;
    ball.x_vector *= -1;
    return ball;
}

game_score upd_score(game_score score, game_ball ball) {
    ball.x == 1 ? ++score.p2 : ++score.p1;
    return score;
}

void win_scr1(void) {
    printf("\n\n\n\n");
    printf("        ########     ##      ##      ## #### ##    ##        \n");
    printf("        ##     ##  ####      ##  ##  ##  ##  ###   ##        \n");
    printf("        ##     ##    ##      ##  ##  ##  ##  ####  ##        \n");
    printf("        ########     ##      ##  ##  ##  ##  ## ## ##        \n");
    printf("        ##           ##      ##  ##  ##  ##  ##  ####        \n");
    printf("        ##           ##      ##  ##  ##  ##  ##   ###        \n");
    printf("        ##         ######     ###  ###  #### ##    ##        ");
}

void win_scr2(void) {
    printf("\n\n\n\n");
    printf("        ########   #######     ##      ## #### ##    ##        \n");
    printf("        ##     ## ##     ##    ##  ##  ##  ##  ###   ##        \n");
    printf("        ##     ##        ##    ##  ##  ##  ##  ####  ##        \n");
    printf("        ########   #######     ##  ##  ##  ##  ## ## ##        \n");
    printf("        ##        ##           ##  ##  ##  ##  ##  ####        \n");
    printf("        ##        ##           ##  ##  ##  ##  ##   ###        \n");
    printf("        ##        #########     ###  ###  #### ##    ##        ");
}

int p1_control(char key, int current_pos) {
    int move = 0;
    if (((key == 'a') || (key == 'A')) && current_pos != 2) {
        move = -1;
    } else if (((key == 'z') || (key == 'Z')) && current_pos != border_width - 3) {
        move = 1;
    }
    return move;
}

int p2_control(char key, int current_pos) {
    int move = 0;
    if (((key == 'k') || (key == 'K')) && current_pos != 2) {
        move = -1;
    } else if (((key == 'm') || (key == 'M')) && current_pos != border_width - 3) {
        move = 1;
    }
    return move;
}

void show_winner_screen(game_score score) {
    printf(COLOR_WINNER);
    printf("\n\n\n\n");
    score.p1 == win_score ? win_scr1() : win_scr2();
    getchar();
}
