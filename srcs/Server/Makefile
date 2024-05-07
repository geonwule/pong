CXX = c++

HEAD = ./includes

SERVER_SRCS_DIR = srcs
SERVER_SRCS = $(wildcard ${SERVER_SRCS_DIR}/*.cpp)
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)
SERVER_NAME = pong_server
SERVER_CFLAGS = -Wall -Wextra -Werror -std=c++11

DEBUG_FLAGS = -g -fsanitize=address

.PHONY: all clean fclean re

all: $(SERVER_NAME)

%.o: %.cpp
	$(CXX) $(SERVER_CFLAGS) -I $(HEAD) -c $< -o $@

$(SERVER_NAME): $(SERVER_OBJS)
	$(CXX) $(SERVER_OBJS) -o $(SERVER_NAME)

debug: $(SERVER_SRCS)
	$(CXX) $(SERVER_CFLAGS) $(DEBUG_FLAGS) $(SERVER_SRCS) -I $(HEAD) -o $(SERVER_NAME) 

clean:
	rm -f $(SERVER_OBJS)

fclean: clean
	rm -rf $(SERVER_NAME)*

re: fclean all
