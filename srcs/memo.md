## 2024.05.15.18:03
- playGame thread가 최초에만 잘 생성되어서 게임이 실행되고, 두번째 게임 실행부터는 스레드가 제대로 생성 안되고 있음.
- playGame thread에 대한 자원 회수의 문제로 추정.. 스레드가 종료될때 join을 어떻게 핸들링 할지 고민 필요.