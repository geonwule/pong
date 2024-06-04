# 🏓 Introduction
원격 플레이가 가능한 퐁게임 입니다.
<br/>
<br/>

<!--
# 🎮 Preview
| <img src="https://github.com/PongPong-Beeps/ft_transcendence/assets/117799441/a4560167-8400-4053-990a-d9e3007bfc97" width="500"> | <img src="https://github.com/PongPong-Beeps/ft_transcendence/assets/117799441/b26e00e0-abbc-4580-8b58-0b64f5726be8" width="500"> |
|:---:|:---:|
| lobby | game room |

| <img src="https://github.com/PongPong-Beeps/ft_transcendence/assets/117799441/787663e9-d1b9-479a-9060-d65d122de26a" width="500"> | <img src="https://github.com/PongPong-Beeps/ft_transcendence/assets/117799441/3c71a905-fdf9-400f-b489-d1f074294c80" width="500"> |
|:---:|:---:|
| versus | in game |

<br/>
<br/>
-->

# 🏗️ Architecture

<img width="1500" alt="image" src="https://github.com/geonwule/pong/assets/117799441/967305ca-e93d-41d3-89aa-66314415bfa1">

### 🕹️ 서버 - 클라이언트

- 1개의 서버와 N개의 클라이언트 구조
- 서버 / 클라이언트 각각 개발하여 루트에서 동시에 빌드
### :electron: 소켓 프로그래밍

- 1개의 서버에서 N개의 클라이언트를 소켓으로 연결
- 클라이언트 간 채팅 기능 구현
- 서버 → 클라이언트 게임 로직 API 구현

### 🐙 멀티스레딩

- 서버: 게임 실행 시 별도의 스레드에서 게임 로직 실행
- 클라이언트: 서버와의 send/receive를 위한 스레드 구현 및 메인 스레드에서 그래픽스 실행

<br/>
<br/>

# 🛠 Tech Stack
<img src="https://img.shields.io/badge/C/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white"/><a>
<img src="https://img.shields.io/badge/OpenGL-5586A4?style=for-the-badge&logo=opengl&logoColor=white"/>
<img src="https://img.shields.io/badge/Socket.io-010101?style=for-the-badge&logo=socketdotio&logoColor=white"/>
<img src="https://img.shields.io/badge/Threads-000000?style=for-the-badge&logo=threads&logoColor=white"/>
<br/>
<br/>

<!--
https://www.d5br5.dev/blog/pro_tip/tech_stack_icon  // 기술스택 뱃지 활용법 블로그
<img src="https://img.shields.io/badge/@-00599C?style=flat&logo=@&logoColor=white"/> // 기술스택 뱃지 양식
-->

# ⚙️ Build & Run

### 🏭 build

```sh
make
```

### 🏃 run

>Server

```sh
./pong_server [ip_address(생략시 기본값 = localhost)] <port_number> 
```


>Client

- run
   ```sh
   ./pong_client [ip_address(생략시 기본값 = localhost)] <port_number>
    ```
- chat
   ```sh
   messages # 채팅메시지 입력 
   ```
- game start
   ```sh
   Game Start # "Game Start" 입력
   ```
<br/>
<br/>

# 🎥 Video
[![YouTube](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://youtu.be/JWj8zLozZ5I)


<!--
https://www.d5br5.dev/blog/pro_tip/tech_stack_icon  // 기술스택 뱃지 활용법 블로그
<img src="https://img.shields.io/badge/@-00599C?style=flat&logo=@&logoColor=white"/> // 기술스택 뱃지 양식
-->
