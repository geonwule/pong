SERVER_DIR = srcs/Server
CLIENT_DIR = srcs/Client

SERVER_NAME = pong_server
CLIENT_NAME = pong_client

#color
DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

.PHONY: all server client clean fclean re

all: server client

server:
	@$(MAKE) -C ${SERVER_DIR}
	@mv ${SERVER_DIR}/${SERVER_NAME} .
	@echo "$(GREEN)Server compiled!$(DEF_COLOR)"

client:
	@$(MAKE) -C ${CLIENT_DIR}
	@mv ${CLIENT_DIR}/${CLIENT_NAME} .
	@echo "$(GREEN)Client compiled!$(DEF_COLOR)"

debug:
	@$(MAKE) -C ${SERVER_DIR} debug
	@$(MAKE) -C ${CLIENT_DIR} debug
	@mv ${SERVER_DIR}/${SERVER_NAME}* .
	@mv ${CLIENT_DIR}/${CLIENT_NAME}* .
	@echo "$(GREEN)Debug mode enabled!$(DEF_COLOR)"

clean:
	@$(MAKE) -C ${SERVER_DIR} clean
	@$(MAKE) -C ${CLIENT_DIR} clean
	@echo "$(BLUE)pong object files cleaned!$(DEF_COLOR)"

fclean: clean
	@$(MAKE) -C ${SERVER_DIR} fclean
	@$(MAKE) -C ${CLIENT_DIR} fclean
	@rm -rf ${SERVER_NAME}* ${CLIENT_NAME}*
	@echo "$(YELLOW)pong library files cleaned!$(DEF_COLOR)"

re: fclean all
	@echo "$(MAGENTA)Cleaned and rebuilt everything for pong!$(DEF_COLOR)"