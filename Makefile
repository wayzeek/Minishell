# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/02 08:32:06 by lpupier           #+#    #+#              #
#    Updated: 2023/04/05 14:19:46 by lpupier          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Constants
NAME			=	minishell

CFLAGS			=	-Wall -Wextra -Werror

RM				=	rm -rf

# Libft support
DIR_LIBFT		=	libft/
LIBFT			=	${DIR_LIBFT}libft.a

# Directories path
DIR_SRCS		=	sources/
DIR_HEADERS		=	includes/

# Builtins directories path
DIR_BUILTINS	=	${DIR_SRCS}builtins/
DIR_CD			=	${DIR_BUILTINS}cd/
DIR_ECHO		=	${DIR_BUILTINS}echo/
DIR_ENV			=	${DIR_BUILTINS}env/
DIR_EXIT		=	${DIR_BUILTINS}exit/
DIR_EXPORT		=	${DIR_BUILTINS}export/
DIR_PWD			=	${DIR_BUILTINS}pwd/
DIR_UNSET		=	${DIR_BUILTINS}unset/
DIR_ECHO		=	${DIR_BUILTINS}echo/

# Parsing directories path
DIR_PARSING		=	${DIR_SRCS}parsing/

# Pipe directories path
DIR_PIPE		=	${DIR_SRCS}pipe/

# Utils directories path
DIRE_UTILS		=	${DIR_SRCS}utils/

# Builtins files
CD_FILES		=	${DIR_CD}ft_cd.c

ECHO_FILES		=	${DIR_ECHO}ft_echo.c

ENV_FILES		=	${DIR_ENV}ft_env.c

EXIT_FILES		=	${DIR_EXIT}ft_exit.c

EXPORT_FILES	=	${DIR_EXPORT}ft_export.c		\
					${DIR_EXPORT}ft_check_export.c	\
					${DIR_EXPORT}ft_export_utils.c 	\
					${DIR_EXPORT}ft_more_error.c \
					${DIR_EXPORT}ft_export_extanded.c \
					${DIR_EXPORT}ft_export_expansion.c

PWD_FILES		=	${DIR_PWD}ft_pwd.c

UNSET_FILES		=	${DIR_UNSET}ft_unset.c

# Parsing files
PARSING_FILES	=	${DIR_PARSING}parsing.c			\
					${DIR_PARSING}heredoc.c			\
					${DIR_PARSING}get_variable.c	\
					${DIR_PARSING}quotes.c

# Pipe files
PIPE_FILES		=	${DIR_PIPE}ft_pipe.c			\
					${DIR_PIPE}pipe_utils.c 		\
					${DIR_PIPE}redirections_utils.c \
					${DIR_PIPE}infiles_utils.c \
					${DIR_PIPE}ft_pipe_extended.c

# Utils files
UTILS_FILES		=	${DIRE_UTILS}decoration.c	\
					${DIRE_UTILS}ft_tab.c		\
					${DIRE_UTILS}list_utils.c	\
					${DIRE_UTILS}signals.c		\
					${DIRE_UTILS}utils.c

# Sources and headers
SRCS			=	${DIR_SRCS}main.c		\
					${DIR_SRCS}commands.c	\
					${CD_FILES}				\
					${ECHO_FILES}			\
					${ENV_FILES}			\
					${EXIT_FILES}			\
					${EXPORT_FILES}			\
					${PWD_FILES}			\
					${UNSET_FILES}			\
					${PARSING_FILES}		\
					${PIPE_FILES}			\
					${UTILS_FILES}

HEADERS			=	${DIR_HEADERS}minishell.h

# Objects
DIR_OBJS		=	.objs/
OBJS			=	${patsubst %.c, ${DIR_OBJS}%.o, ${SRCS}}

# Rules
.PHONY :	all re clean fclean FORCE watermark

all :	 	${NAME}

${DIR_OBJS}%.o: %.c	${HEADERS}
	@mkdir -p ${shell dirname $@}
	${CC} ${CFLAGS} -c $< -o $@ -I ${DIR_HEADERS}

${DIR_OBJS}: ${MKDIR} ${DIR_OBJS}

${NAME}:	${LIBFT} ${OBJS}
			${CC} ${CFLAGS} ${OBJS} -lreadline ${LIBFT} -o ${NAME}

${LIBFT}:	FORCE
			${MAKE} -C ${DIR_LIBFT}

clean:
			${RM} ${OBJS} ${DIR_OBJS} 
			${MAKE} clean -C ${DIR_LIBFT}

fclean:		clean
			${RM} ${NAME}
			${MAKE} fclean -C ${DIR_LIBFT}

re :		fclean all

watermark:
			@echo "$$WATERMARK"

FORCE:

# Text art
define WATERMARK

██╗  ██╗██████╗ 
██║  ██║╚════██╗ name:  Leon Pupier • Valentin Cart
███████║ █████╔╝ login: lpupier	    • vcart
╚════██║██╔═══╝  
     ██║███████╗
     ╚═╝╚══════╝lyon.fr

endef
export WATERMARK