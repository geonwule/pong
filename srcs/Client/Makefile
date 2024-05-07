CXX = c++


OPNE_GL = ../../mac_opengl
CLIENT_SRCS_DIR = srcs
HEAD = ${CLIENT_SRCS_DIR}/gameElements

CLIENT_SRCS = $(wildcard ${CLIENT_SRCS_DIR}/*.cpp)

CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)
CLIENT_CFLAGS = -Wall -Wextra -Werror -std=c++11 -I${HEAD} -I ${OPNE_GL}/include/ -DGL_SILENCE_DEPRECATION
CLIENT_LFLAGS = -L ${OPNE_GL}/lib-x86_64 -lglfw3 -framework Cocoa -framework IOKit -framework OpenGL
CLIENT_NAME = pong_client

DEBUG_FLAGS = -g -fsanitize=address

.PHONY: all clean fclean re

all: $(CLIENT_NAME)

%.o: %.cpp
	$(CXX) $(CLIENT_CFLAGS) -c $< -o $@

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CXX) $(CLIENT_LFLAGS) $(CLIENT_OBJS) -o $(CLIENT_NAME)

debug: $(CLIENT_SRCS)
	$(CXX) $(CLIENT_CFLAGS) $(CLIENT_LFLAGS) $(DEBUG_FLAGS) $(CLIENT_SRCS) -I $(HEAD) -o $(CLIENT_NAME) 

clean:
	rm -f $(CLIENT_OBJS)

fclean: clean
	rm -rf $(CLIENT_NAME)*

re: fclean all