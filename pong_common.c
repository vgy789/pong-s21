#include "pong_common.h"

_Bool is_round_won(game_ball ball) { 
    return ball.x == 1 || ball.x == border_length - 1; 
}

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
        if ((y_position == p1_pos + 2 && y_direction == -1) || 
            (y_position == p1_pos - 2 && y_direction == 1))
            direction *= -1;
    }

    if (x_position == p2_column - 1) {
        if ((y_position == p2_pos + 2 && y_direction == -1) || 
            (y_position == p2_pos - 2 && y_direction == 1))
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

