#include <iostream>
#include <sstream> //hexToRGB

#ifdef __APPLE__
#include "../../../mac_opengl/include/GLFW/glfw3.h"
#else
#include <GLFW/glfw3.h>
#endif

#include "GameFrame.hpp"
#include "Util.hpp"
#include "Client.hpp"
#include "Cache.hpp"

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

void drawBall(float x, float y, float radius)//(float x, float y, float radius)
{
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

void drawLives(int player1Lives, int player2Lives)
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

void multiPlay()
{
    GameData &data = Cache::data;
    // 배경 그리기
    drawBackground();

    // 목숨 그리기
    drawLives(data.player1Lives, data.player2Lives);

    // Paddle 그리기
    drawPaddle(data.paddle1_x, data.paddle1_y, data.paddle_width, data.paddle_height); // 왼쪽 Paddle
    drawPaddle(data.paddle2_x, data.paddle2_y, data.paddle_width, data.paddle_height);  // 오른쪽 Paddle

    // Ball 그리기
    drawBall(data.ball_x, data.ball_y, data.ball_radius); // Ball
}
