#include <iostream>
#include <string>
#include <thread>
#include "../../../mac_opengl/include/GLFW/glfw3.h"

#define WIDTH 1500
#define HEIGHT 750

using namespace std;

void error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// 키 콜백함수 정의
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            cout << "W key pressed" << endl;
            break;
        case GLFW_KEY_S:
            cout << "S key pressed" << endl;
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
            cout << "W key released" << endl;
            break;
        case GLFW_KEY_S:
            cout << "S key released" << endl;
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