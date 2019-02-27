# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/12 13:39:53 by ldedier           #+#    #+#              #
#    Updated: 2019/02/27 17:51:23 by ldedier          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CC      = gcc

PWD = \"$(shell pwd)\"

DEBUG ?= 0

SRCDIR   = srcs
OBJDIR   = objs
BINDIR   = .
INCLUDESDIR = includes
LIBFTDIR = libft
SPEED = -j1
LIBFT_INCLUDEDIR = includes
LIBFT = $(LIBFTDIR)/libft.a

OK_COLOR = \x1b[32;01m
EOC = \033[0m

SRCS_NO_PREFIX		=	main.c builtins.c env.c execute.c init.c tools.c\
						set_env.c other_builtins.c expansions.c cd.c\
						free_all.c print_env.c cd_tools.c expansions_tools.c\
						execute_tools.c sanitize_path.c home.c init_term.c\
						signals.c get_command.c is_printable_utf8.c\
						utf8_tools.c edit_command.c cursor_motion.c\
						auto_completion.c populate_word_by_index.c\
						add_choices_from_dir.c preprocess_choice_add.c\
						populate_choices.c keys.c shell_tools.c\
						process_execute.c

INCLUDES_NO_PREFIX	= minishell.h

SOURCES = $(addprefix $(SRCDIR)/, $(SRCS_NO_PREFIX))
OBJECTS = $(addprefix $(OBJDIR)/, $(SRCS_NO_PREFIX:%.c=%.o))
INCLUDES = $(addprefix $(INCLUDESDIR)/, $(INCLUDES_NO_PREFIX))

INC = -I $(INCLUDESDIR) -I $(LIBFTDIR)/$(LIBFT_INCLUDEDIR)\

CFLAGS = -DPATH=$(PWD) -Wall -Wextra -Werror $(INC)
LFLAGS = -L $(LIBFTDIR) -lft -ltermcap

ifeq ($(DEBUG), 1)
	LFLAGS += -fsanitize=address
	CFLAGS += -DDEBUG
	CC += -g3
else
	SPEED = -j8
endif

all:
	@make -C $(LIBFTDIR) $(SPEED)
	@make $(BINDIR)/$(NAME) $(SPEED)

debug:
	@make all DEBUG=1

$(LIBFT):
	@make -C $(LIBFTDIR)

$(BINDIR)/$(NAME): $(OBJECTS) $(LIBFT)
	@$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS) 
	@echo "$(OK_COLOR)$(NAME) linked with success !$(EOC)"

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDES)
	@mkdir -p $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@make clean -C $(LIBFTDIR)
	@rm -f $(OBJECTS)
	@rm -rf $(OBJDIR)

fclean: clean
	@make fclean -C $(LIBFTDIR)
	@rm -f $(BINDIR)/$(NAME)

re: fclean all

.PHONY: all clean fclean re
