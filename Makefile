# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/26 23:29:42 by spayeur           #+#    #+#              #
#    Updated: 2023/02/01 16:50:21 by spayeur          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                 Informations                                 #
# **************************************************************************** #

NAME		=	webserv

CC			=	c++
CFLAGS		=	-Wall -Wextra -Werror -std=c++98
INCS_DIR	=	incs
SRCS_DIR	=	srcs

# **************************************************************************** #
#                                    C0l0r$                                    #
# **************************************************************************** #

# Regular Colors
BLACK		:= $(shell tput -Txterm setaf 0)
RED			:= $(shell tput -Txterm setaf 1)
GREEN		:= $(shell tput -Txterm setaf 2)
YELLOW		:= $(shell tput -Txterm setaf 3)
BLUE		:= $(shell tput -Txterm setaf 4)
MAGENTA		:= $(shell tput -Txterm setaf 5)
CYAN		:= $(shell tput -Txterm setaf 6)
WHITE		:= $(shell tput -Txterm setaf 7)

# Bold
BOLD		:= $(shell tput -Txterm bold)
BBLACK		= $(BOLD)$(BLACK)
BRED		= $(BOLD)$(RED)
BGREEN		= $(BOLD)$(GREEN)
BYELLOW		= $(BOLD)$(YELLOW)
BBLUE		= $(BOLD)$(BLUE)
BMAGENTA	= $(BOLD)$(MAGENTA)
BCYAN		= $(BOLD)$(CYAN)
BWHITE		= $(BOLD)$(WHITE)
END			:= $(shell tput -Txterm sgr0)

# Clear
CLEAR		:= $(shell tput -Txterm clear)
CLEAR_LINE	:= $(shell tput -Txterm el)

# **************************************************************************** #
#                                 Source files                                 #
# **************************************************************************** #

# *********************************** Main *********************************** #

SRCS_DEFAULT_FILES	=	main

SRCS_DEFAULT		=	$(addprefix $(SRCS_DIR)/, $(SRCS_DEFAULT_FILES))

# ********************************* Classes ********************************** #

SRCS_CLASSES_DEFAULT_FILES	=	Http_block \
								Server_block \
								Location_block

SRCS_CLASSES_DEFAULT		=	$(addprefix $(SRCS_DIR)/classes/, $(SRCS_CLASSES_DEFAULT_FILES))

SRCS_CLASSES	=	$(SRCS_CLASSES_DEFAULT)

# ********************************* Classes ********************************** #

SRCS_STRUCTURES_DEFAULT_FILES	=	response

SRCS_STRUCTURES_DEFAULT			=	$(addprefix $(SRCS_DIR)/structures/, $(SRCS_STRUCTURES_DEFAULT_FILES))

SRCS_STRUCTURES	=	$(SRCS_STRUCTURES_DEFAULT)

# ********************************* Parsing ********************************** #

SRCS_PARSING_DEFAULT_FILES	=	parse \
								core_parsing \
								tokens_extraction \
								is_duplicate \
								fix_parsing \
								choose_location

SRCS_PARSING_DEFAULT		=	$(addprefix $(SRCS_DIR)/parsing/, $(SRCS_PARSING_DEFAULT_FILES))

SRCS_PARSING_CONTEXT_FILES	=	context \
								http \
								server \
								location

SRCS_PARSING_CONTEXT		=	$(addprefix $(SRCS_DIR)/parsing/context/, $(SRCS_PARSING_CONTEXT_FILES))

SRCS_PARSING_DIRECTIVE_FILES	=	directive \
									listen \
									server_name \
									error_page \
									client_max_body_size \
									limit_except \
									return \
									root \
									autoindex \
									index \
									cgi

SRCS_PARSING_DIRECTIVE			=	$(addprefix $(SRCS_DIR)/parsing/directive/, $(SRCS_PARSING_DIRECTIVE_FILES))

SRCS_PARSING_UTILITY_FILES	=	error \
								utils

SRCS_PARSING_UTILITY		=	$(addprefix $(SRCS_DIR)/parsing/utility/, $(SRCS_PARSING_UTILITY_FILES))

SRCS_PARSING	=	$(SRCS_PARSING_DEFAULT) $(SRCS_PARSING_CONTEXT) $(SRCS_PARSING_DIRECTIVE) $(SRCS_PARSING_UTILITY)

# ********************************* Request ********************************** #

SRCS_REQUEST_DEFAULT_FILES	=	http_request_parse_core \
								http_request_parse_line \
								http_request_parse_headers \
								process_request

SRCS_REQUEST_DEFAULT		=	$(addprefix $(SRCS_DIR)/request/, $(SRCS_REQUEST_DEFAULT_FILES))

SRCS_REQUEST	=	$(SRCS_REQUEST_DEFAULT)

# ********************************* Response ********************************* #

SRCS_RESPONSE_DEFAULT_FILES	=	generate_response \
								get \
								post \
								delete \
								utils

SRCS_RESPONSE_DEFAULT		=	$(addprefix $(SRCS_DIR)/response/, $(SRCS_RESPONSE_DEFAULT_FILES))

SRCS_RESPONSE	=	$(SRCS_RESPONSE_DEFAULT)

# ********************************** Server ********************************** #

SRCS_SERVER_DEFAULT_FILES	=	server_loop \
								server_utils \
								cgi

SRCS_SERVER_DEFAULT		=	$(addprefix $(SRCS_DIR)/server/, $(SRCS_SERVER_DEFAULT_FILES))

SRCS_SERVER		=	$(SRCS_SERVER_DEFAULT)

# ********************************* Project ********************************** #

SRCS_PROJECT	=	$(addsuffix .cpp, $(SRCS_DEFAULT) $(SRCS_CLASSES) $(SRCS_STRUCTURES) $(SRCS_PARSING) $(SRCS_REQUEST) $(SRCS_RESPONSE) $(SRCS_SERVER))

# **************************************************************************** #
#                                 Object files                                 #
# **************************************************************************** #

OBJS_PROJECT	=	$(SRCS_PROJECT:.cpp=.o)

# **************************************************************************** #
#                                    Rules                                     #
# **************************************************************************** #

%.o:		%.cpp
			@printf "$(CLEAR_LINE)$(BMAGENTA)$@\r$(END)"
			@$(CC) $(CFLAGS) -I $(INCS_DIR) -c $< -o $@ -DDEBUG=1

all:		$(NAME)

$(NAME):	$(OBJS_PROJECT)
			@$(CC) $(CFLAGS) $(OBJS_PROJECT) -o $(NAME)
			@echo "$(CLEAR_LINE)$(CYAN)$(NAME) $(BGREEN)has been build !$(END)"

clean:
			@echo "$(BBLUE)Cleaning...$(END)"
			@echo "$(CYAN)$(words $(sort $(wildcard $(OBJS_PROJECT)))) object file(s) $(BGREEN)has/have been removed.$(END)"
			@rm -f $(OBJS_PROJECT)

fclean:		clean
ifneq (,$(wildcard $(NAME)))
			@rm -f $(NAME)
			@echo "$(CYAN)$(NAME)$(END) $(BGREEN)has been removed.$(END)"
endif

re:			fclean all

.PHONY:		all clean fclean re
