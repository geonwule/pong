HEAD = ../../mac_opengl
CC = c++

#for mac
CFLAGS = -Wall -Wextra -Werror -std=c++11 -I ${HEAD}/include/ -DGL_SILENCE_DEPRECATION
LFLAGS = -L ${HEAD}/lib-x86_64 -lglfw3 -framework Cocoa -framework IOKit -framework OpenGL

SRCS = srcs/main.cpp
OBJS = $(SRCS:.cpp=.o)
NAME = pong_client

all: $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re