# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kkais <kkais@student.42wolfsburg.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/07 13:12:06 by kkais             #+#    #+#              #
#    Updated: 2022/04/07 17:10:07 by kkais            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
INC = ./srcs/pipex.h
W = -Wall -Werror -Wextra
CC = gcc

CFILES = ./srcs/pipex.c ./srcs/find_path.c ./srcs/join_len_str.c ./srcs/split_trim.c ./srcs/substr_dup.c

OFILES = ${CFILES:.c=.o}

YELLOW=\033[1;33m
NC=\033[0m
GREEN=\033[0;32m
RED=\033[0;31m

.PHONY: all clean fclean re print_header_m print_header_b

all : ${NAME}

${NAME} : ${OFILES}
	@$(CC) $(OFILES) $(W) -o $(NAME)
	@echo "${YELLOW}$(NAME) created ${NC}"

%.o:%.c ${INC}
	@${CC} ${W} -o $@ -c $<
	@echo "${GREEN}$@ created${NC}"

clean:
	@rm -f ./srcs/*.o
	@echo "${RED}Deleting objects..."

fclean : clean
	@rm -f .temp
	@rm -f outfile
	@rm -f ${NAME}
	@echo "${RED}Deleting $(NAME) and extra files..."

re : fclean all
