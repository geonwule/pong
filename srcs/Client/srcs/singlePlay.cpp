#include <iostream>
#include <sstream>
#include <algorithm> // std::max, std::min 함수를 사용하기 위해 필요합니다.
#include "../../../mac_opengl/include/GLFW/glfw3.h"

#include "CircleObject.hpp"
#include "Paddle.hpp"

#define WIDTH 1500
#define HEIGHT 750

#define paddleColor "#ffffff"
#define ballColor "#ffa939"
#define attackBallColor "#ff396e"
#define backgroundColor "#27522d"
#define PI 3.14159265

using namespace std;


int isBallMoving = STOP;

int keyW = 0, keyS = 0, keyUp = 0, keyDown = 0;
int player1PaddleMove = STOP, player2PaddleMove = STOP;

// Paddle과 Ball의 위치와 속도
float paddle1Y = 0.0f, paddle2Y = 0.0f;//, ballX = 0.0f, ballY = 0.0f;
float paddleSpeed = 0.02f;//, ballSpeed = 0.01f;

// Ball의 방향
//float ballDirX = 1.0f, ballDirY = 1.0f;

CircleObject ball;

// 플레이어의 목숨
int player1Lives = 5, player2Lives = 5;

// void resetKey()
// {
//     isBallMoving = STOP;
//     ballX = 0.0f;
//     ballY = 0.0f;
//     ballDirX = 1.0f;
//     ballDirY = 1.0f;

//     player1PaddleMove = STOP;
//     player2PaddleMove = STOP;

//     paddle1Y = 0.0f;
//     paddle2Y = 0.0f;
// }

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            player1PaddleMove = UP;
            cout << "player1 = UP" << endl;
            break;
        case GLFW_KEY_S:
            cout << "player1 = DOWN" << endl;
            player1PaddleMove = DOWN;
            break;
        case GLFW_KEY_UP:
            cout << "player2 = UP" << endl;
            player2PaddleMove = UP;
            break;
        case GLFW_KEY_DOWN:
            cout << "player2 = DOWN" << endl;
            player2PaddleMove = DOWN;
            break;
        case GLFW_KEY_R:
            // resetKey();
            break;
        case GLFW_KEY_ESCAPE:
            cout << "ESC key pressed" << endl;
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        default:
            cout << "Other key pressed" << endl;
            break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            break;
        case GLFW_KEY_S:
            break;
        }
    }
}

void hexToRGB(const std::string &hex, float &r, float &g, float &b)
{
    unsigned int ir, ig, ib;
    std::stringstream ss;
    ss << std::hex << hex.substr(1, 2);
    ss >> ir;
    ss.clear();
    ss << std::hex << hex.substr(3, 2);
    ss >> ig;
    ss.clear();
    ss << std::hex << hex.substr(5, 2);
    ss >> ib;
    r = ir / 255.0f;
    g = ig / 255.0f;
    b = ib / 255.0f;
}

void drawBackground()
{
    float r, g, b;
    hexToRGB(backgroundColor, r, g, b);
    glClearColor(r, g, b, 1.0f); // RGBA
    glClear(GL_COLOR_BUFFER_BIT);
}

void drawPaddle(float x, float y, float width, float height)
{
    float r, g, b;
    hexToRGB(paddleColor, r, g, b);
    glColor3f(r, g, b);

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawBall()//(float x, float y, float radius)
{
    float x = ball.getX();
    float y = ball.getY();
    float radius = ball.getRadius();
    float r, g, b;
    hexToRGB(ballColor, r, g, b);
    glColor3f(r, g, b);

    // 원을 그립니다.
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i++)
    {
        float degInRad = i * PI / 180;
        glVertex2f(x + cos(degInRad) * radius, y + sin(degInRad) * radius);
    }
    glEnd();
}

void drawHeart(float x, float y, float radius)
{
    int segments = 100;
    float angle = 2.0f * 3.1416f / segments;

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f); // 빨간색으로 설정
    for(int i = 0; i <= segments; i++) {
        float dx = radius * cosf(i * angle);
        float dy = radius * sinf(i * angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void drawLives()
{
    float radius = 0.05f;
    float offset = 0.1f;

    // Player 1의 목숨 그리기
    for(int i = 0; i < player1Lives; i++) {
        drawHeart(-1.75f + i * offset, 0.9f, radius);
    }

    // Player 2의 목숨 그리기
    for(int i = 0; i < player2Lives; i++) {
        drawHeart(1.5f + i * offset, 0.9f, radius);
    }
}

void singlePlay()
{

    // 배경 그리기
    drawBackground();

    // 목숨 그리기
    drawLives();

    // Paddle 그리기
    if (player1PaddleMove == UP)
        paddle1Y = std::min(paddle1Y + paddleSpeed, 1.0f - 0.3f);
    else if (player1PaddleMove == DOWN)
        paddle1Y = std::max(paddle1Y - paddleSpeed, -1.0f);

    if (player2PaddleMove == UP)
        paddle2Y = std::min(paddle2Y + paddleSpeed, 1.0f - 0.3f);
    else if (player2PaddleMove == DOWN)
        paddle2Y = std::max(paddle2Y - paddleSpeed, -1.0f);

    drawPaddle(-0.9f, paddle1Y, 0.1f, 0.3f); // 왼쪽 Paddle
    drawPaddle(0.9f, paddle2Y, 0.1f, 0.3f);  // 오른쪽 Paddle

    // Ball 그리기
    // ballX += ballDirX * ballSpeed;
    // ballY += ballDirY * ballSpeed;
    ball.move();
    drawBall();
    // drawBall(ballX, ballY, 0.05f);

    ball.collisions(paddle1Y, paddle2Y);
    ball.checkLives(player1Lives, player2Lives);
    
}

// // Ball이 벽이나 Paddle에 부딪히면 방향 변경
//     if (ballX <= -0.9f && ballY >= paddle1Y && ballY <= paddle1Y + 0.3f)
//         ballDirX = -ballDirX; // 왼쪽 Paddle
//     else if (ballX >= 0.9f && ballY >= paddle2Y && ballY <= paddle2Y + 0.3f)
//         ballDirX = -ballDirX; // 오른쪽 Paddle
//     else if (ballY <= -1.0f || ballY >= 1.0f)
//         ballDirY = -ballDirY; // 위쪽 또는 아래쪽 벽

//     // Ball이 Paddle을 넘어가면 해당 플레이어의 목숨 감소
//     if (ballX < -0.91f) {
//         player1Lives--;
//         ballX = 0.0f;
//         ballY = 0.0f;
//     }
//     else if (ballX > 0.91f) {
//         player2Lives--;
//         ballX = 0.0f;
//         ballY = 0.0f;
//     }


/** 삼각형, 사각형, 점, 선 그리기 **/
// void drawTriangle()
// {
//     glBegin(GL_TRIANGLES);
//     glVertex2f(-0.9f, 0.9f); // 왼쪽 상단 근처
//     glVertex2f(-0.8f, 0.9f); // 조금 오른쪽으로 이동
//     glVertex2f(-0.9f, 0.8f); // 조금 아래로 이동
//     glEnd();

//     glBegin(GL_TRIANGLES);
//     glVertex2f(0.9f, 0.9f); // 오른쪽 상단 근처
//     glVertex2f(0.8f, 0.9f); // 조금 왼쪽으로 이동
//     glVertex2f(0.9f, 0.8f); // 조금 아래로 이동
//     glEnd();
// }

// void drawRectangle()
// {
//     glBegin(GL_QUADS);
//     glVertex2f(-0.5f, -0.5f);
//     glVertex2f(-0.5f, 0.5f);
//     glVertex2f(0.5f, 0.5f);
//     glVertex2f(0.5f, -0.5f);
//     glEnd();
// }

// void drawPoint()
// {
//     glBegin(GL_POINTS);
//     glVertex2f(0.0f, 0.0f);
//     glEnd();
// }

// void drawLine()
// {
//     glBegin(GL_LINES);
//     glVertex2f(-0.5f, -0.5f);
//     glVertex2f(0.5f, 0.5f);
//     glEnd();
// }
