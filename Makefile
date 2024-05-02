CXX = c++

# for server
SERVER_SRCS_DIR = srcs/Server/srcs
SERVER_SRCS = ${SERVER_SRCS_DIR}/main.cpp
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)
SERVER_NAME = pong_server
SERVER_CFLAGS = -Wall -Wextra -Werror -std=c++98

# for client
OPNE_GL = ./mac_opengl
CLIENT_SRCS_DIR = srcs/Client/srcs
CLIENT_SRCS = ${CLIENT_SRCS_DIR}/main.cpp
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)
CLIENT_CFLAGS = -Wall -Wextra -Werror -std=c++11 -I ${OPNE_GL}/include/ -DGL_SILENCE_DEPRECATION
CLIENT_LFLAGS = -L ${OPNE_GL}/lib-x86_64 -lglfw3 -framework Cocoa -framework IOKit -framework OpenGL
CLIENT_NAME = pong_client

.PHONY: all clean fclean re server client

all: server client

server: $(SERVER_NAME)

client: $(CLIENT_NAME)

${SERVER_SRCS_DIR}/%.o: ${SERVER_SRCS_DIR}/%.cpp
	$(CXX) $(SERVER_CFLAGS) -c $< -o $@

${CLIENT_SRCS_DIR}/%.o: ${CLIENT_SRCS_DIR}/%.cpp
	$(CXX) $(CLIENT_CFLAGS) -c $< -o $@

$(SERVER_NAME): $(SERVER_OBJS)
	$(CXX) $(SERVER_OBJS) -o $(SERVER_NAME)

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CXX) $(CLIENT_LFLAGS) $(CLIENT_OBJS) -o $(CLIENT_NAME)

clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS)

fclean: clean
	rm -f $(SERVER_NAME) $(CLIENT_NAME)

re: fclean all