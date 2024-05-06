C++ 소켓 통신에서는 바이너리 데이터를 직접 전송하는 방식을 사용할 수 있습니다. 이 방식은 JSON과 같은 텍스트 기반 형식보다 효율적일 수 있지만, 데이터의 구조를 직접 관리해야 하므로 복잡할 수 있습니다.

다음은 바이너리 데이터를 전송하는 방법의 예입니다:
struct GameData {
    float ball_x;
    float ball_y;
    float ball_radius;

    float paddle_width;
    flaot paddle_height;
    float paddle1_x;
    float paddle1_y;
    float paddle2_x;
    float paddle2_y;
    
    int playerlives1;
    int playerlives2;
};

// ...

GameData data;
data.ball_x = ball_x;
data.ball_y = ball_y;
// ... 나머지 필드도 설정 ...

// 구조체를 바이트 배열로 변환하여 전송합니다.
send(sockfd, &data, sizeof(data), 0);


배열로
수신 측에서는 다음과 같이 메시지를 처리할 수 있습니다:

GameData data;

// 바이트 배열을 구조체로 변환합니다.
int valread = recv(sockfd, &data, sizeof(data), 0);
if (valread != sizeof(data)) {
    cerr << "Failed to receive message" << endl;
    exit(1);
    return;
}

int ball_x = data.ball_x;
int ball_y = data.ball_y;
// ... 나머지 값들도 추출 ...


data
이 방식을 사용하면, 서버와 클라이언트가 주고받는 메시지의 형식을 쉽게 정의하고 관리할 수 있습니다. 하지만, 이 방식은 플랫폼 간의 호환성 문제를 일으킬 수 있으므로 주의가 필요합니다. 예를 들어, 서로 다른 시스템에서는 정수의 바이트 순서(엔디언)가 다를 수 있습니다. 이런 문제를 피하려면, 네트워크 바이트 순서를 사용하는 것이 좋습니다.