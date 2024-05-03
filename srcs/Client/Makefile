CXX = c++

OPNE_GL = ../../mac_opengl
CLIENT_SRCS_DIR = srcs
CLIENT_SRCS = ${CLIENT_SRCS_DIR}/main.cpp ${CLIENT_SRCS_DIR}/singlePlay.cpp
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)
CLIENT_CFLAGS = -Wall -Wextra -Werror -std=c++11 -I ${OPNE_GL}/include/ -DGL_SILENCE_DEPRECATION
CLIENT_LFLAGS = -L ${OPNE_GL}/lib-x86_64 -lglfw3 -framework Cocoa -framework IOKit -framework OpenGL
CLIENT_NAME = pong_client

.PHONY: all clean fclean re

all: $(CLIENT_NAME)

%.o: %.cpp
	$(CXX) $(CLIENT_CFLAGS) -c $< -o $@

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CXX) $(CLIENT_LFLAGS) $(CLIENT_OBJS) -o $(CLIENT_NAME)

clean:
	rm -f $(CLIENT_OBJS)

fclean: clean
	rm -f $(CLIENT_NAME)

re: fclean all