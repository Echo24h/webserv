# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gborne <gborne@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/27 16:29:59 by gborne            #+#    #+#              #
#    Updated: 2023/04/14 18:30:14 by gborne           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.cpp \
	src/Config.cpp \
	src/ConfigServer.cpp \
	src/ConfigLocation.cpp \
	src/Response.cpp \
	src/Request.cpp \
	src/Server.cpp \
	src/CGI.cpp \
	src/Utils.cpp

OBJS = $(SRCS:.cpp=.o)

CXX = g++
RM = rm -f
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = webserv

%.o : %.c
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

all: $(NAME)

test:
	netstat -tlnp | grep 3001
	./test/ubuntu_tester http://localhost:3001

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
	$(CXX) -o request test/Request.cpp

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)
	rm -f request

re: fclean $(NAME)

leaks: all
	valgrind ./$(NAME)

.PHONY: all clean fclean re leaks test
