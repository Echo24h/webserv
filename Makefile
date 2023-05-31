# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gborne <gborne@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/27 16:29:59 by gborne            #+#    #+#              #
#    Updated: 2023/05/31 17:08:06 by gborne           ###   ########.fr        #
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


ifeq ($(shell uname), Darwin)
# Instructions pour Mac
test:
	./test/mac_tester http://localhost:8000
else ifeq ($(shell uname), Linux)
# Instructions pour Ubuntu
test:
	./test/ubuntu_tester http://localhost:3002
else
    $(error Unsupported operating system: $(shell uname))
endif

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
	valgrind ./$(NAME) conf/tester_ubuntu.conf

.PHONY: all clean fclean re leaks test
