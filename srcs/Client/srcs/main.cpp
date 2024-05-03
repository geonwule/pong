#include <iostream>
#include <string>
#include <thread>
#include "../../../mac_opengl/include/GLFW/glfw3.h"

#define WIDTH 1500
#define HEIGHT 750

#define paddleColor "#ffffff"
#define ballColor "#ffa939"
#define attackBallColor "#ff396e"
#define backgroundColor "#27522d"

using namespace std;

// void loadBackgroundImage(const std::string& imagePath)
// {
//     GLuint texture = SOIL_load_OGL_texture
//     (
//         imagePath.c_str(),
//         SOIL_LOAD_AUTO,
//         SOIL_CREATE_NEW_ID,
//         SOIL_FLAG_INVERT_Y
//     );

//     if(texture == 0)
//         return;

//     glEnable(GL_TEXTURE_2D);
//     glBindTexture(GL_TEXTURE_2D, texture);
//     glBegin(GL_QUADS);
//     glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
//     glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
//     glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
//     glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f);
//     glEnd();
// }

void error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    cout << "framebuffer_size_callback" << endl;
    // Viewport를 새 윈도우 크기에 맞게 조정합니다.
    glViewport(0, 0, width, height);

    /** 가로세로 비율에 따라 좌표계 조정(도형등 왜곡 방지) **/

    // 윈도우의 가로 세로 비율을 얻습니다.
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
    float aspectRatio = (float)width / (float)height;

    // 좌표계를 조정합니다.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (width >= height)
    {
        glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);
    }
    else
    {
        glOrtho(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio, -1.0, 1.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// 키 콜백함수 정의

enum e_paddleMove
{
    UP,
    DOWN,
    STOP,
};

int isBallMoving = STOP;

int keyW = 0, keyS = 0, keyUp = 0, keyDown = 0;
int player1PaddleMove = STOP, player2PaddleMove = STOP;

// Paddle과 Ball의 위치와 속도
float paddle1Y = 0.0f, paddle2Y = 0.0f, ballX = 0.0f, ballY = 0.0f;
float paddleSpeed = 0.02f, ballSpeed = 0.01f;

// Ball의 방향
float ballDirX = 1.0f, ballDirY = 1.0f;

void resetKey()
{
    isBallMoving = STOP;
    ballX = 0.0f;
    ballY = 0.0f;
    ballDirX = 1.0f;
    ballDirY = 1.0f;

    player1PaddleMove = STOP;
    player2PaddleMove = STOP;

    paddle1Y = 0.0f;
    paddle2Y = 0.0f;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
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
            resetKey();
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

// 마우스 버튼 콜백 함수 정의
static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    (void)mods;
    switch (action)
    {
    case GLFW_PRESS:
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            cout << "Left button pressed" << endl;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            cout << "Right button pressed" << endl;
            break;
        default:
            cout << "Other button pressed" << endl;
            break;
        }
        break;
    case GLFW_RELEASE:
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            cout << "Left button released" << endl;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            cout << "Right button released" << endl;
            break;
        default:
            cout << "Other button released" << endl;
            break;
        }
        break;
    default:
        break;
    }
}

// 마우스 이동 콜백 함수 정의
static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    cout << "Cursor Position at (" << xpos << " : " << ypos << ")" << endl;
}

void drawTriangle()
{
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.9f, 0.9f); // 왼쪽 상단 근처
    glVertex2f(-0.8f, 0.9f); // 조금 오른쪽으로 이동
    glVertex2f(-0.9f, 0.8f); // 조금 아래로 이동
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(0.9f, 0.9f); // 오른쪽 상단 근처
    glVertex2f(0.8f, 0.9f); // 조금 왼쪽으로 이동
    glVertex2f(0.9f, 0.8f); // 조금 아래로 이동
    glEnd();
}

void drawRectangle()
{
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();
}

void drawPoint()
{
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
}

void drawLine()
{
    glBegin(GL_LINES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glEnd();
}

#include <sstream>
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

#define PI 3.14159265

void drawBall(float x, float y, float radius)
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

#include <algorithm> // std::max, std::min 함수를 사용하기 위해 필요합니다.

void drawSometing()
{
    // 배경 그리기
    drawBackground();

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
    ballX += ballDirX * ballSpeed;
    ballY += ballDirY * ballSpeed;
    drawBall(ballX, ballY, 0.05f);

    // Ball이 벽이나 Paddle에 부딪히면 방향 변경
    if (ballX <= -0.9f && ballY >= paddle1Y && ballY <= paddle1Y + 0.3f)
        ballDirX = -ballDirX; // 왼쪽 Paddle
    else if (ballX >= 0.9f && ballY >= paddle2Y && ballY <= paddle2Y + 0.3f)
        ballDirX = -ballDirX; // 오른쪽 Paddle
    else if (ballY <= -1.0f || ballY >= 1.0f)
        ballDirY = -ballDirY; // 위쪽 또는 아래쪽 벽
}

void studyGlfw()
{
    // 오류 콜백 함수 설정
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    // 프레임버퍼 사이즈 콜백 설정
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 윈도우의 초기 크기를 얻습니다.
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // 콜백 함수를 최초에 한 번 호출합니다.
    framebuffer_size_callback(window, width, height);

    // 키 콜백 설정
    glfwSetKeyCallback(window, key_callback);
    // 마우스 버튼 콜백 설정
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // 마우스 이동 콜백 설정
    glfwSetCursorPosCallback(window, cursor_position_callback);

    double time;

    while (!glfwWindowShouldClose(window))
    {
        // 시간 측정
        time = glfwGetTime();

        // 백 버퍼 초기화
        glClear(GL_COLOR_BUFFER_BIT);

        /* 그림 그리기 (백버퍼에)
        이벤트 처리가 많은 시간을 소모하는 작업이라면,
        그림 그리는 작업과 이벤트 처리를 별도의 스레드에서 수행하는 것이 더 효율적일 수 있습니다*/
        drawSometing();

        // 백버퍼와 프론트 버퍼 교환
        glfwSwapBuffers(window);

        // 이벤트 처리
        // 앞쪽에서 처리해줘도 문제 X
        glfwPollEvents();
    }

    glfwTerminate();
    return;
}

int main()
{
    studyGlfw();
    return 0;
}