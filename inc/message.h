/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 20:21:47 by gborne            #+#    #+#             */
/*   Updated: 2022/12/02 20:50:26 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_H
# define MESSAGE_H

#define	RED			"\033[1;31m"
#define	GREEN		"\033[1;32m"
#define	YELLOW		"\033[1;33m"
#define	BLUE		"\033[1;34m"
#define	MAGENTA		"\033[1;35m"
#define	DEF			"\033[0m"

#define ERROR		"\033[1;31mError \033[0m"
#define RECV		"\033[1;31m[<-] \033[0m"
#define	SEND		"\033[1;32m[->] \033[0m"
#define REQUEST		"\033[1;33mNew request \033[0m"

#endif
