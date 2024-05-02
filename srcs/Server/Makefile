CXX = c++

SERVER_SRCS_DIR = srcs
SERVER_SRCS = ${SERVER_SRCS_DIR}/main.cpp
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)
SERVER_NAME = pong_server
SERVER_CFLAGS = -Wall -Wextra -Werror -std=c++98

.PHONY: all clean fclean re

all: $(SERVER_NAME)

%.o: %.cpp
	$(CXX) $(SERVER_CFLAGS) -c $< -o $@

$(SERVER_NAME): $(SERVER_OBJS)
	$(CXX) $(SERVER_OBJS) -o $(SERVER_NAME)

clean:
	rm -f $(SERVER_OBJS)

fclean: clean
	rm -f $(SERVER_NAME)

re: fclean all
