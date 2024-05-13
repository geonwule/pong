CXX = c++

GAME_ELEMENT_HEAD = includes/gameElements

HEAD = -Iincludes -I${GAME_ELEMENT_HEAD}

OPNE_GL = ../../mac_opengl
CLIENT_SRCS_DIR = srcs

CLIENT_SRCS = $(wildcard ${CLIENT_SRCS_DIR}/*.cpp)

ifeq ($(shell uname -s), Darwin) #MacOS
LDFLAGS = 
CLIENT_LFLAGS = -L ${OPNE_GL}/lib-x86_64 -lglfw3 -framework Cocoa -framework IOKit -framework OpenGL
else #Linux
LDFLAGS = -lpthread
CLIENT_LFLAGS = -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor
endif

CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)
CLIENT_CFLAGS = -Wall -Wextra -Werror -std=c++11 ${HEAD} -I ${OPNE_GL}/include/ -DGL_SILENCE_DEPRECATION
CLIENT_NAME = pong_client

DEBUG_FLAGS = -g -fsanitize=address

.PHONY: all clean fclean re

all: $(CLIENT_NAME)

%.o: %.cpp
	$(CXX) $(CLIENT_CFLAGS) -c $< -o $@

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CXX) $(CLIENT_OBJS) $(LDFLAGS) $(CLIENT_LFLAGS) -o $(CLIENT_NAME)

debug: $(CLIENT_SRCS)
	$(CXX) $(CLIENT_CFLAGS) $(CLIENT_LFLAGS) $(DEBUG_FLAGS) $(CLIENT_SRCS) $(HEAD) -o $(CLIENT_NAME) 

clean:
	rm -f $(CLIENT_OBJS)

fclean: clean
	rm -rf $(CLIENT_NAME)*

re: fclean all