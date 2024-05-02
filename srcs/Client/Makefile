CXX = c++

GLAD_HEAD = ../../glad/include/glad/
# ../../glad/include/KHR/
GLAD_SRCS = ../../glad/src/gl.c

OPNE_GL = ../../mac_opengl
CLIENT_SRCS_DIR = srcs
CLIENT_SRCS = ${CLIENT_SRCS_DIR}/main.cpp ${GLAD_SRCS}
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)


CLIENT_CFLAGS = -std=c++11 -I ${OPNE_GL}/include/  -I ${GLAD_HEAD} -DGL_SILENCE_DEPRECATION
#-Wall -Wextra -Werror 
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