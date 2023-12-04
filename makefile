# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/03 20:32:59 by ajeannin          #+#    #+#              #
#    Updated: 2023/12/04 18:30:40 by pkorsako         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

########## V A R I A B L E S ###########

NAME			= minishell
INCLUDES		= -Iincludes -Ilibft
LIBFT			= -Llibft -lft
SRC_DIR			= srcs
OBJ_DIR			= obj
OBJ_DIR_WOF		= obj
CC				= gcc
CFLAGS			=  -g3 #-fsanitize=leak,address,undefined -Wall -Werror -Wextra
READLINE		= -lreadline



############# S O U R C E S ############

SRC_FILES		:= $(shell find $(SRC_DIR) -type f -name "*.c")
SRCS			:= $(SRC_FILES)
OBJS			:= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
#OBJSWOF			:= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR_WOF)/%.o, $(SRCS))


################ R U L E S #############

all				: $(NAME)

$(NAME)			: $(OBJS)
	@make -s -C libft/
	@$(CC) $(OBJS) $(CFLAGS) $(INCLUDES) $(LIBFT) -o $(NAME) $(READLINE)
	@echo "Compilation Minishell: done"
	@ctags -R
	@echo "Tags are available"

wof			: $(OBJSWOF)
	@make -s -C libft/
	@$(CC) $(OBJSWOF) $(INCLUDES) $(LIBFT) -o $(NAME) $(READLINE)
	@echo ":WARNING:"
	@echo "Compilation Minishell w/o flag: done"
	@ctags -R
	@echo "Tags are available"

$(OBJ_DIR)/%.o	: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR_WOF)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean			:
	@$(MAKE) clean -s -C libft/
	@$(RM) -r $(OBJ_DIR)
	@echo "Clean obj_dir: done"

fclean			: clean
	@$(MAKE) fclean -s -C libft/
	@$(RM) -r  $(NAME)
	@echo "Fclean minishell: done"

re				: fclean all

rewof			: fclean wof

norme			:
	@echo "\n\nNorme Includes\n"
	@norminette includes
	@echo "\n\nNorme Sources\n"
	@norminette srcs

