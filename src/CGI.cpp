/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 14:52:38 by gborne            #+#    #+#             */
/*   Updated: 2023/06/01 14:41:43 by gborne           ###   ########.fr       */
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

// Execute et construit la reponse CGI
void	CGI::_construct( void ) {

	// On execute le CGI
	std::string	cgi_response = _exec();

	// On verifie si le CGI gere le code HTTP
	if (cgi_response.find("Status: ", 0) == 0) {
		_content = cgi_response;
		_code = atoi(cgi_response.substr(8, 3).c_str());
	}

	// On contruit la reponse CGI
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

// Execute et retourne la reponse du CGI
std::string	CGI::_exec( void ) {

    const std::string& method = _request->get_method();
    const std::string& contentLength = _request->get_ressource("Content-Length");
    const size_t contentSize = _request->get_content().size();

	if (method == "POST" && !contentLength.empty() && std::size_t(std::atoi(contentLength.c_str())) != contentSize) {
        _code = HTTP::BAD_REQUEST;
        return "Content-Length does not match request body size";
    }

    // Vérifiez que la taille de la requête est valide
    const int bodyLimit = _config->get_body_limit();
    if (contentSize > static_cast<std::size_t>(bodyLimit)) {
        _code = HTTP::REQUEST_ENTITY_TOO_LARGE;
        return "Request body size exceeds limit";
    }

	// On creer l'env et les args pour le CGI
	char ** env = _generate_env();

	char ** arg = new char*[3];

	std::string	cgi_response;

	arg[0] = strdup(_request->get_cgi().c_str());
	arg[1] = strdup(_request->get_real_path().c_str());
	arg[2] = NULL;

	int			saveStdin;
	int			saveStdout;

	saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);

	write(fdIn, _request->get_content().c_str(), _request->get_content().size());
	lseek(fdIn, 0, SEEK_SET);

	int	pid = fork();

	if (pid == -1) {
		_code = HTTP::INTERNAL_SERVER_ERROR;
		return ("Status: 500\r\n\r\n");
	}
	else if (pid == 0) {
		// On execute le CGI

		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);

		execve(arg[0], arg, env);
		std::cerr << "CGI process fail: excve errno " << errno << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
		exit(EXIT_FAILURE);
	}
	else {
		// On recupere et traite la reponse du CGI
		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		cgi_response = _read(fdOut);
	}

	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(fIn);
	fclose(fOut);
	close(fdIn);
	close(fdOut);
	close(saveStdin);
	close(saveStdout);
	
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

// Permet de lire un fd en plusieurs morceaux
std::string	CGI::_read( const int & fd ) const {

	char				buffer[BUFF_SIZE];
	ssize_t				bytes_read;
	std::stringstream	ss;

	while((bytes_read = read(fd, buffer, BUFF_SIZE)) != 0) {
		if (bytes_read == (ssize_t)-1) {
			break;
		}
		std::string	string(buffer, bytes_read);
		ss << string;
	}
	return ss.str();
}

// Genere l'environnement pour le CGI
char **	CGI::_generate_env( void ) const {

	std::map<std::string, std::string> env;

	// Server
	env["SERVER_SOFTWARE"] = "webserv/1.0";
	env["SERVER_NAME"] = "0";
	env["GATEWAY_INTERFACE"] = "CGI/1.1";

	// Request
	env["CONTENT_TYPE"] = _request->get_ressource("Content-Type");
	env["CONTENT_LENGTH"] = _request->get_ressource("Content-Length");

	env["REDIRECT_STATUS"] = "200";

	env["SERVER_PROTOCOL"] = _request->get_version();
	env["SERVER_PORT"] = itoa(_config->get_port());
	env["REQUEST_METHOD"] = _request->get_method();
	env["REQUEST_URI"] = _request->get_virtual_path();

	env["PATH_INFO"] = _request->get_virtual_path();
	env["PATH_TRANSLATED"] = get_current_dir() + "/" + _request->get_real_path();
	env["SCRIPT_NAME"] = _request->get_real_path();
	//env["SCRIPT_FILENAME"] = _request->get_real_path();
	env["QUERY_STRING"] = _request->get_query(); // var1=val1&var2=val2&...
	env["REMOTE_ADDR"] = _request->get_client_ip();
	env["REMOTE_USER"] = "";
	env["REMOTE_IDENT"] = "";
	
	// Client
	env["HTTP_ACCEPT"] = _request->get_ressource("Accept");
	env["HTTP_ACCEPT_LANGUAGE"] = _request->get_ressource("Accept-Language");
	env["HTTP_USER_AGENT"] = _request->get_ressource("User-Agent");
	env["HTTP_COOKIE"] = _request->get_ressource("Cookie"); // clef=valeur separate by ";"
	env["HTTP_REFERER"] = _request->get_ressource("Referer");
	if (_request->get_ressource("X-Secret-Header-For-Test").size() > 0)
		env["HTTP_X_SECRET_HEADER_FOR_TEST"] = _request->get_ressource("X-Secret-Header-For-Test");

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
