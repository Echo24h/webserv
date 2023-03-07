# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gborne <gborne@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/27 16:29:59 by gborne            #+#    #+#              #
#    Updated: 2023/03/07 20:40:16 by gborne           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS			=	main.cpp \
					src/Config.cpp \
					src/ConfigServer.cpp \
					src/ConfigLocation.cpp \
					src/Response.cpp \
					src/Request.cpp \
					src/Server.cpp \
					src/CGI.cpp \
					src/Utils.cpp

SRCS_C			=	test/Client.cpp

OBJS			= $(SRCS:.cpp=.o)
OBJS_C			= $(SRCS_C:.cpp=.o)

CXX				= g++
RM				= rm -f
CXXFLAGS		= -Wall -Wextra -Werror -std=c++98

NAME			= webserv
NAME_C			= client

%.o : %.c
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

all:			$(NAME)

$(NAME):		$(OBJS)
				$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
				$(CXX) -o request test/Request.cpp -lcurl
				$(CXX) -o stress test/Stress.cpp

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)
				rm request stress

re:				fclean $(NAME)

client:			$(OBJS_C)
				$(CXX) $(CXXFLAGS) -o $(NAME_C) $(OBJS_C)

leaks:			all
				valgrind ./$(NAME)

.PHONY:			all clean fclean re leaks
