#ifndef PONG_COMMON_H
#define PONG_COMMON_H

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

_Bool is_round_won(game_ball ball);
int get_x_direction(int y_position, int y_direction, int x_position, int x_direction, int p1_pos, int p2_pos);
int get_y_direction(int y_position, int y_direction, int x_position, int p1_pos, int p2_pos);
game_ball ball_movement(game_ball ball, int p1_row, int p2_row);
game_ball ball_reset(game_ball ball);
game_score upd_score(game_score score, game_ball ball);
int p1_control(char key, int current_pos);
int p2_control(char key, int current_pos);

#define WIN_SCR1_TEXT "\n\n\n\n\
    ########     ##      ##      ## #### ##    ##        \n\
    ##     ##  ####      ##  ##  ##  ##  ###   ##        \n\
    ##     ##    ##      ##  ##  ##  ##  ####  ##        \n\
    ########     ##      ##  ##  ##  ##  ## ## ##        \n\
    ##           ##      ##  ##  ##  ##  ##  ####        \n\
    ##           ##      ##  ##  ##  ##  ##   ###        \n\
    ##         ######     ###  ###  #### ##    ##        "

#define WIN_SCR2_TEXT "\n\n\n\n\
    ########   #######     ##      ## #### ##    ##        \n\
    ##     ## ##     ##    ##  ##  ##  ##  ###   ##        \n\
    ##     ##        ##    ##  ##  ##  ##  ####  ##        \n\
    ########   #######     ##  ##  ##  ##  ## ## ##        \n\
    ##        ##           ##  ##  ##  ##  ##  ####        \n\
    ##        ##           ##  ##  ##  ##  ##   ###        \n\
    ##        #########     ###  ###  #### ##    ##        "

#endif // PONG_COMMON_H
