#ifndef GAMEFRAME_HPP
#define GAMEFRAME_HPP

#define paddleColor "#ffffff"
#define ballColor "#ffa939"
#define attackBallColor "#ff396e"
#define backgroundColor "#27522d"
#define PI 3.14159265

enum e_canvas
{
    WIDTH=1500,
    HEIGHT=750,
};

enum e_game
{
    GAME_START,
    GAME_LOADING,
    GAME_ING,
    GAME_END,
};

struct GameData {
    float ball_x;
    float ball_y;
    float ball_radius;

    float paddle_width;
    float paddle_height;
    float paddle1_x;
    float paddle1_y;
    float paddle2_x;
    float paddle2_y;
    
    int player1Lives;
    int player2Lives;

    int isGameStart;
};

#endif // GAMEFRAME_HPP