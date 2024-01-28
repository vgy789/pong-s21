#include <ncurses.h>
#include <stdio.h>

enum {
    delay_duration = 100,

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

void initcurses(void);
void print_frame(game_ball ball, int p1_row, int p2_row, game_score score);
void win_scr1(void);
void win_scr2(void);

game_ball ball_movement(game_ball ball, int p1_row, int p2_row);
int get_x_direction(int y_position, int x_position, int x_direction, int p1_pos, int p2_pos);
int get_y_direction(int y_position, int y_direction);
game_ball ball_reset(game_ball ball);
game_score upd_score(game_score score, game_ball ball);

_Bool is_round_won(game_ball ball) { return ball.x == 1 || ball.x == border_length - 1; }

int main(void) {
    int key_pressed;
    game_score score = {p_init_score, p_init_score};
    int p1_row = p_init_row;
    int p2_row = p_init_row;

    initcurses();

    game_ball ball = {ball_init_column, ball_init_row, 1, 1};

    do {
        clear();
        print_frame(ball, p_init_row, p_init_row, score);
        refresh();
        ball = ball_movement(ball, p1_row, p2_row);

        if (is_round_won(ball)) {
            score = upd_score(score, ball);
            ball = ball_reset(ball);
        }

        key_pressed = getch();
    } while (score.p1 != 21 && score.p2 != 21 && key_pressed != 'q' && key_pressed != 'Q');

    clear();
    if (score.p1 == win_score || score.p2 == win_score) {
        attrset(COLOR_PAIR(5));
        score.p1 == win_score ? win_scr1() : win_scr2();
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
    init_pair(1, COLOR_RED, COLOR_RED);
    init_pair(2, COLOR_CYAN, COLOR_CYAN);
    init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(4, COLOR_GREEN, COLOR_GREEN);
    init_pair(5, -1, -1);
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
                attrset(COLOR_PAIR(2));
            } else if (row == ball.y && col == ball.x) {
                attrset(COLOR_PAIR(1));
            } else if ((row >= p1_row - 1 && row <= p1_row + 1 && col == p1_column) ||
                       (row >= p2_row - 1 && row <= p2_row + 1 && col == p2_column)) {
                attrset(COLOR_PAIR(4));
            } else if (col == ball_init_column) {
                attrset(COLOR_PAIR(3));
            } else {
                attrset(COLOR_PAIR(5));
            }
            printw(" ");
            attrset(COLOR_PAIR(5));
        }
        printw("\n");
    }
    print_score(score);
}

int get_x_direction(int y_position, int x_position, int x_direction, int p1_pos, int p2_pos) {
    int direction = x_direction;
    if ((x_position == p1_column || x_position == p2_column) &&
        (y_position == p1_pos || y_position == p1_pos + 1 || y_position == p1_pos - 1 ||
         y_position == p2_pos || y_position == p2_pos + 1 || y_position == p2_pos - 1))
        direction *= -1;
    return direction;
}

int get_y_direction(int y_position, int y_direction) {
    int direction = y_direction;
    if (y_position == (border_width - 2) || y_position == 1) direction *= -1;
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
    printw("        ########     ##      ##      ## #### ##    ##\n");
    printw("        ##     ##  ####      ##  ##  ##  ##  ###   ##\n");
    printw("        ##     ##    ##      ##  ##  ##  ##  ####  ##\n");
    printw("        ########     ##      ##  ##  ##  ##  ## ## ##\n");
    printw("        ##           ##      ##  ##  ##  ##  ##  ####\n");
    printw("        ##           ##      ##  ##  ##  ##  ##   ###\n");
    printw("        ##         ######     ###  ###  #### ##    ##");
}

void win_scr2(void) {
    printw("        ########   #######     ##      ## #### ##    ##\n");
    printw("        ##     ## ##     ##    ##  ##  ##  ##  ###   ##\n");
    printw("        ##     ##        ##    ##  ##  ##  ##  ####  ##\n");
    printw("        ########   #######     ##  ##  ##  ##  ## ## ##\n");
    printw("        ##        ##           ##  ##  ##  ##  ##  ####\n");
    printw("        ##        ##           ##  ##  ##  ##  ##   ###\n");
    printw("        ##        #########     ###  ###  #### ##    ##");
}
