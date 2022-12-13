/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 14:52:38 by gborne            #+#    #+#             */
/*   Updated: 2022/12/13 17:32:08 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/CGI.hpp"

namespace HTTP {

CGI::CGI( void ) : _config(NULL), _request(NULL) { return ; }

CGI::CGI( const ConfigServer * config, const Request * request ) : _config(config), _request(request), _code(200) {
	_construct();
	return ;
}

CGI::CGI( const CGI & src ) { *this = src; return ; }

CGI::~CGI() { return ; }

CGI &	CGI::operator=( const CGI & rhs ) {
	_config = rhs._config;
	_request = rhs._request;
	return *this;
}

// GETTERS

int			CGI::get_code( void ) const {
	return _code;
}

std::string	CGI::get_type( void ) const {
	return _type;
}

std::string	CGI::get_content( void ) const {
	return _content;
}

// FUNCTIONS

void	CGI::_construct( void ) {

	std::cout << "ici" << std::endl;

	std::string	cgi_response = _exec();

	std::cout << "ici" << std::endl;

	if (_code < 500) {
		size_t	delim = cgi_response.find("\r\n\r\n");
		_type = cgi_response.substr(14, delim - 14);
		_content = cgi_response.substr(delim + 4, cgi_response.size() - (delim + 4));
	}
	else {
		_type = _config->get_type("html");
		_content += html_header();
		_content += "<h3 id=\"error\">Server internal error</h3>\n";
		_content += "<p>";
		_content += cgi_response;
		_content += "</p>\n";
		_content += html_footer();
	}
}

std::string	CGI::_exec( void ) {

	char ** env = _generate_env();

	char ** arg = new char*[3];

	std::string	cgi_response;

	arg[0] = strdup(_request->get_cgi().c_str());
	arg[1] = strdup(_request->get_real_path().c_str());
	arg[2] = NULL;

	int	fd[2];
	int	fd_err[2];

	if (pipe(fd) == -1)
		std::cerr << ERROR << "[CGI.cpp] pipe() : can't create new pipe" << std::endl;
	else {
		if (pipe(fd_err) == -1) {
			std::cerr << ERROR << "[CGI.cpp] pipe() : can't create new pipe" << std::endl;
			close(fd_err[0]);
			close(fd_err[1]);
		}
		else {
			int	pid = fork();

			if (pid == -1) {
				close(fd[0]);
				close(fd[1]);
				close(fd_err[0]);
				close(fd_err[1]);
				std::cerr << ERROR << "[CGI.cpp] exec() : can't create new process" << std::endl;
			}
			else {
				if (pid == 0) {
					close(fd[0]);
					close(fd_err[0]);
					dup2(fd[1], 1);
					dup2(fd_err[1], 2);
					close(fd[1]);
					close(fd_err[1]);
					if (execve(arg[0], arg, env) == -1)
						write(1, "KO\n", 4);
					std::cout << "ERROR" << std::endl;
					exit(0);
				}
				else {
					std::cout << "ici4" << std::endl;
					if (waitpid(pid, &_code, 0) == -1) {
						close(fd[0]);
						close(fd[1]);
						close(fd_err[0]);
						close(fd_err[1]);
						std::cerr << ERROR << "[CGI.cpp] exec() : execve process fail" << std::endl;
						_code = 500;
					}
					else {
						std::cout << _code << std::endl;
						if (_code == 0) {
							_code = 200;
							close(fd_err[0]);
							close(fd_err[1]);
							close(fd[1]);
							std::cout << "ici3" << std::endl;
							cgi_response = _read(fd[0]);
							std::cout << "ici3" << std::endl;
							//std::cout << cgi_response << std::endl;
							close(fd[0]);
						}
						else {
							_code = 500;
							close(fd[0]);
							close(fd[1]);
							close(fd_err[1]);
							std::cout << "ici2" << std::endl;
							cgi_response = _read(fd_err[0]);
							std::cout << "ici2" << std::endl;
							//std::cout << cgi_response << std::endl;
							close(fd_err[0]);
						}
					}
				}
			}
		}
	}
	int	i = 0;
	while(arg[i])
		free(arg[i++]);
	delete [] arg;
	i = 0;
	while(env[i])
		free(env[i++]);
	delete [] env;

	return cgi_response;
}

std::string	CGI::_read( const int & fd ) const {

	char				buffer[BUFF_SIZE];
	ssize_t				bytes_read;
	std::stringstream	ss;

	while((bytes_read = read(fd, buffer, BUFF_SIZE)) != 0) {
		if (bytes_read == (ssize_t)-1) {
			std::cerr << ERROR << "[CGI.cpp] _recv() : read error" << std::endl;
			return std::string();
		}
		buffer[bytes_read] = '\0';
		ss << buffer;
	}
	return ss.str();
}

char **	CGI::_generate_env( void  ) const {

	std::map<std::string, std::string> env;

	env["REDIRECT_STATUS"] = "200";

	// Server
	env["SERVER_SOFTWARE"] = "webserv/1.0";
	env["SERVER_NAME"] = _config->get_host();
	env["GATEWAY_INTERFACE"] = "CGI/1.1";

	// Request
	env["SERVER_PROTOCOL"] = _request->get_version();
	env["SERVER_PORT"] = _config->get_port();
	env["REQUEST_METHOD"] = _request->get_method();
	env["PATH_INFO"] = get_current_dir() + _request->get_real_path();
	env["PATH_TRANSLATED"] = _request->get_real_path();
	env["SCRIPT_NAME"] = _request->get_cgi();
	//env["SCRIPT_FILENAME"] = "cgi/php-cgi-ubuntu";
	env["QUERY_STRING"] = _request->get_query(); // var1=val1&var2=val2&...
	//env["REMOTE_HOST"] = "";
	env["REMOTE_ADDR"] = _request->get_client_ip();
	//env["AUTH_TYPE"] = "";
	//env["REMOTE_USER"] = "";
	//env["REMOTE_IDENT"] = "";
	if (_request->get_method() == "POST") {
		//std::cout << _request->get_ressource("Content-Type") << std::endl;
		env["CONTENT_TYPE"] = _request->get_ressource("Content-Type");
		//env["CONTENT_TYPE"] = "image/png";
		env["CONTENT_LENGTH"] = _request->get_ressource("Content-Length");
	}

	// Client
	env["HTTP_ACCEPT"] = _request->get_ressource("Accept");
	env["HTTP_ACCEPT_LANGUAGE"] = _request->get_ressource("Accept-Language");
	env["HTTP_USER_AGENT"] = _request->get_ressource("User-Agent");
	//env["HTTP_COOKIE"] = ""; // clef=valeur separate by ";"
	env["HTTP_REFERER"] = _request->get_ressource("Referer");

	char **	env_char = new char*[env.size() + 1];

	std::map<std::string, std::string>::const_iterator	it = env.begin();
	int i = 0;
	while (it != env.end()) {
		std::string str = it->first + "=" + it->second;
		env_char[i] = strdup(str.c_str());
		it++, i++;
	}
	env_char[i] = NULL;
	return env_char;
}

} // namespace HTTP
