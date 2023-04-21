/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 14:52:38 by gborne            #+#    #+#             */
/*   Updated: 2023/04/22 00:40:38 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/CGI.hpp"

namespace HTTP {

CGI::CGI( const ConfigServer * config, const Request * request ) : _config(config), _request(request), _code(200) {
	_construct();
	return ;
}

CGI::CGI( const CGI & src ) { *this = src; return ; }

CGI::~CGI() { return ; }

CGI &	CGI::operator=( const CGI & rhs ) {
	_config = rhs._config;
	_request = rhs._request;
	_code = rhs._code;
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

	std::string	cgi_response = _exec();

	if (cgi_response.find("Status: ", 0) == 0) {
		_content = cgi_response;
		_code = atoi(cgi_response.substr(8, 3).c_str());
	}
	if (_code < 400) {
		size_t	delim = cgi_response.find("\r\n\r\n");
		std::vector<std::string>	tokens = split(cgi_response.substr(0, delim));
		std::map<std::string, std::string>	header;

		std::vector<std::string>::iterator	it = tokens.begin();
		std::vector<std::string>::iterator	ite = tokens.end();

		while (it != ite) {
			header.insert(std::make_pair(get_key(*it), get_value(*it)));
			it++;
		}
		_type = header["Content-type"];
		_content = cgi_response.substr(delim + 4, cgi_response.size() - (delim + 4));
	}
	else
		_content = cgi_response;
}

/*void	CGI::_send_content( int fd, const std::string & content ) {
	
    const size_t chunk_size = 100000;
    const char* buffer = &content[0];
    size_t remaining_size = content.size();
    ssize_t bytes_sent = 0;

    std::cout << content.size() << std::endl;

    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    char* tmp_buffer = new char[chunk_size];

    while (remaining_size > 0) {
        size_t bytes_to_send = std::min(remaining_size, chunk_size);
        memcpy(tmp_buffer, buffer, bytes_to_send);
        bytes_sent = write(fd, tmp_buffer, bytes_to_send);
        if (bytes_sent == -1) {
            std::cout << ERROR << "[CGI.cpp] write() : code " << errno << std::endl;
            _code = HTTP::INTERNAL_SERVER_ERROR;
			break;
        }
        remaining_size -= bytes_sent;
        buffer += bytes_sent;
        std::cout << "send_content: " << bytes_sent << std::endl;
    }

    delete[] tmp_buffer;
}*/

std::string	CGI::_exec( void ) {

    // Vérifiez que la méthode est POST et que la taille de la requête est correcte
    const std::string& method = _request->get_method();
    const std::string& contentLength = _request->get_ressource("Content-Length");
    const size_t contentSize = _request->get_content().size();

	//std::cout << contentLength << std::endl;
	//std::cout << contentSize << std::endl;

    //if (method == "POST" && contentLength.empty()) {
    //    _code = HTTP::BAD_REQUEST;
    //    return "Missing Content-Length header";
    //}
    //else 
	if (method == "POST" && !contentLength.empty() && std::size_t(std::atoi(contentLength.c_str())) != contentSize) {
        _code = HTTP::BAD_REQUEST;
        return "Content-Length does not match request body size";
    }

    // Vérifiez que la taille de la requête est valide
    const int bodyLimit = _config->get_body_limit();
    if (contentSize > static_cast<std::size_t>(bodyLimit)) {
        _code = HTTP::BAD_REQUEST;
        return "Request body size exceeds limit";
    }

	char ** env = _generate_env();

	/*
	char ** arg = new char*[3];

	std::string	cgi_response;

	arg[0] = strdup(std::string(get_current_dir() + "/" + _request->get_cgi()).c_str());
	arg[1] = strdup(std::string(get_current_dir() + "/" + _request->get_real_path()).c_str());
	arg[2] = NULL;
	*/

	char ** arg = new char*[3];

	std::string	cgi_response;

	arg[0] = strdup(_request->get_cgi().c_str());
	arg[1] = strdup(_request->get_real_path().c_str());
	arg[2] = NULL;

	int fd[2], fd_err[2];


    if (pipe(fd) == -1) {
        _code = HTTP::INTERNAL_SERVER_ERROR;
        return "Failed to create pipe";
    }
    if (pipe(fd_err) == -1) {
        close(fd[0]);
        close(fd[1]);
        _code = HTTP::INTERNAL_SERVER_ERROR;
        return "Failed to create error pipe";
    }

	//PRINT FOR DEBUG
	//std::cout << "FILE:" << std::endl << arg[0] << std::endl;
	//std::cout << "ARGV:" << std::endl << arg[0] << std::endl << arg[1] << std::endl;
	//std::cout << "ENVP:" << std::endl;
	//for (int i = 0; env[i] != NULL; i++)
	//	std::cout << env[i] << std::endl;

	int	pid = fork();

	if (pid == -1) {
		close(fd[0]);
		close(fd[1]);
		close(fd_err[0]);
		close(fd_err[1]);
		_code = HTTP::INTERNAL_SERVER_ERROR;
	}
	else if (pid == 0) {
		close(fd_err[0]);
		dup2(fd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		dup2(fd_err[1], STDERR_FILENO);
		close(fd[0]);
		close(fd[1]);
		close(fd_err[1]);
		if (execve(arg[0], NULL, env) == -1)
			std::cerr << "CGI process fail: excve errno " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	else {

		std::cout << "write" << std::endl;

		std::cout << "request.size(): " << (int)_request->get_content().size() << std::endl;
		
		//size_t bytes_send = write(fd[1], _request->get_full_request().c_str(), _request->get_full_request().size());

		//size_t bytes_send = write(fd[1], _request->get_content().c_str(), _request->get_content().size());
		//write(fd[1], _request->get_content().c_str(), _request->get_content().size());
		size_t bytes_send = write(fd[1], "\0", 2);
		//lseek(fd[1], 0, SEEK_SET);

		std::cout << "bytes_send: " << (int)bytes_send << std::endl;
		
		if (bytes_send == (size_t)-1 || bytes_send != _request->get_full_request().size()) {
			std::cout << ERROR << "[CGI.cpp] write() : code " << errno << std::endl;
			_code = HTTP::INTERNAL_SERVER_ERROR;
		}

	std::cout << "write2" << std::endl;

		close(fd[1]);

		close(fd_err[1]);

		int status = 0;

		std::cout << "wait" << std::endl;

		std::cout << "code: " << _code << std::endl;
		
		if (_code >= 400 || waitpid(-1, NULL, 0) == -1) {
			std::cout << "wait2" << std::endl;
			kill(pid, SIGTERM);
			close(fd[0]);
			close(fd_err[0]);
			_code = HTTP::INTERNAL_SERVER_ERROR;
			return "CGI process fail";
		}

		std::cout << "wait3" << std::endl;
		std::cout << "status: " << status << std::endl;
		
		if (status == 0) {
			close(fd_err[0]);

			std::cout << "_read" << std::endl;
			cgi_response = _read(fd[0]);
			std::cout << "_read2" << std::endl;
			
			if (cgi_response.empty())
				_code = HTTP::INTERNAL_SERVER_ERROR;
			std::cout << "response: " << cgi_response << std::endl;
			close(fd[0]);
		}
		else {
			_code = HTTP::INTERNAL_SERVER_ERROR;;
			close(fd[0]);
			cgi_response = _read(fd_err[0]);
			close(fd_err[0]);
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
			return std::string();
		}
		std::string	string(buffer, bytes_read);
		ss << string;
	}
	return ss.str();
}

char **	CGI::_generate_env( void ) const {

	std::map<std::string, std::string> env;

	// YoupiBanane/youpi.bla
	// /YoupiBanane/youpi.bla
	// /youpi.bla
	// youpi.bla
	// ""
	// /directory/youpi.bla (get_virtual_path())
	// directory/youpi.bla
	// www/YoupiBanane/youpi.bla (get_real_path())
	// /www/YoupiBanane/youpi.bla
	// /home/gborne/Bureau/webserv/www/YoupiBanane/youpi.bla
	// /home/gborne/Bureau/webserv/YoupiBanane/youpi.bla
	// YoupiBanane/youpi.bla
	// /YoupiBanane/youpi.bla

	//env["PATH_INFO"] = get_current_dir() + "/" + _request->get_real_path();



	/*
	CONTENT_LENGTH=0
	CONTENT_TYPE=
	GATEWAY_INTERFACE=CGI/1.1
	PATH_INFO=/directory/youpi.bla
	PATH_TRANSLATED=/directory/youpi.bla
	QUERY_STRING=
	REDIRECT_STATUS=200
	REMOTE_IDENT=
	REMOTE_USER=
	REMOTEaddr=0
	REQUEST_METHOD=GET
	REQUEST_URI=/directory/youpi.bla
	SCRIPT_FILENAME=YoupiBanane/directory/youpi.bla
	SCRIPT_NAME=YoupiBanane/directory/youpi.bla
	SERVER_NAME=0
	SERVER_PORT=8000
	SERVER_PROTOCOL=HTTP/1.1
	SERVER_SOFTWARE=Weebserv/1.0
	script_name: test_mac/macos_cgi_tester
	*/


	// Server
	env["SERVER_SOFTWARE"] = "webserv/1.0";
	env["SERVER_NAME"] = "0";
	env["GATEWAY_INTERFACE"] = "CGI/1.1";

	// Request
	env["SERVER_PROTOCOL"] = _request->get_version();
	env["SERVER_PORT"] = itoa(_config->get_port());
	env["REQUEST_METHOD"] = _request->get_method();
	env["REQUEST_URI"] = _request->get_virtual_path();

	env["PATH_INFO"] = _request->get_virtual_path();

	//std::cout << "PATH_INFO: " << env["PATH_INFO"] << std::endl;

	env["PATH_TRANSLATED"] = get_current_dir() + "/" + _request->get_real_path();
	//std::cout << "PATH_TRANSLATED: " << env["PATH_TRANSLATED"] << std::endl;
	env["SCRIPT_NAME"] = _request->get_real_path();
	//std::cout << "SCRIPT_NAME: " << env["SCRIPT_NAME"] << std::endl;
	env["SCRIPT_FILENAME"] = _request->get_real_path();
	//std::cout << "SCRIPT_FILENAME: " << env["SCRIPT_FILENAME"] << std::endl;
	env["QUERY_STRING"] = _request->get_query(); // var1=val1&var2=val2&...
	//std::cout <<  _request->get_query() << std::endl;
	//env["REMOTE_HOST"] = "";
	env["REMOTE_ADDR"] = _request->get_client_ip();
	//env["AUTH_TYPE"] = "";
	env["REMOTE_USER"] = "";
	env["REMOTE_IDENT"] = "";
	env["CONTENT_TYPE"] = _request->get_ressource("Content-Type");
	env["CONTENT_LENGTH"] = itoa(_request->get_content().size());

	env["REDIRECT_STATUS"] = "200";
	
	// Client
	env["HTTP_ACCEPT"] = _request->get_ressource("Accept");
	env["HTTP_ACCEPT_LANGUAGE"] = _request->get_ressource("Accept-Language");
	env["HTTP_USER_AGENT"] = _request->get_ressource("User-Agent");
	env["HTTP_COOKIE"] = _request->get_ressource("Cookie"); // clef=valeur separate by ";"
	env["HTTP_REFERER"] = _request->get_ressource("Referer");

	char **	env_char = new char*[env.size() + 1];

	std::map<std::string, std::string>::const_iterator	it = env.begin();
	int i = 0;
	while (it != env.end()) {
		std::string str = it->first + "=" + it->second;
		std::cout << str << std::endl;
		env_char[i] = strdup(str.c_str());
		it++, i++;
	}
	env_char[i] = NULL;
	return env_char;
}

} // namespace HTTP
