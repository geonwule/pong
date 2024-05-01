CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++11

SRCS = srcs/main.cpp
OBJS = $(SRCS:.cpp=.o)
NAME = pong_client

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re