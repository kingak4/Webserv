/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:29 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/04 16:14:15 by kikwasni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/Parser.hpp"

//constructors

Parse::Parse()
{
	raw_request = "";
	method = "";
	path = "";
	version = "";
	body = "";
	is_valid = false;
	error_code = 0;
}

Parse::Parse(const std::string &request)
{
	raw_request = request;
	method = "";
	path = "";
	version = "";
	body = "";
	is_valid = false;
	error_code = 0;
}
Parse::Parse(const Parse &other)
{
	if(this != &other)
	{
		raw_request = other.raw_request;
		method = other.method;
		path = other.path;
		version = other.version;
		body = other.body;
		headers = other.headers;
		is_valid = other.is_valid;
		error_code = other.error_code;
	}
}

Parse::~Parse() {};

//getters

std::string Parse::getPath() const { return path; }
std::string Parse::getVersion() const { return version; }
std::string Parse::getBody() const { return body; }
std::map<std::string, std::string> Parse::getHeaders() const { return headers; }
std::string Parse::getRawRequest() const { return raw_request; }
bool Parse::isValid() const { return is_valid; }
int Parse::getErrorCode() const { return error_code; }

// parsing the first line

void Parse::parseFirstLine()
{
	if(raw_request.empty())
	{
		is_valid = false;
		 error_code = 400;
		return ;
	}
	size_t pos = raw_request.find("\r\n");
	std::string first_line = raw_request.substr(0, pos);
	if (pos == std::string::npos)
	{
		is_valid = false;
		error_code = 400;
		return;
	}
	std::vector<std::string> parts = splitBySpace(first_line);
	if(parts.size() != 3) 
	{
		is_valid = false;
		error_code = 400;
		return ;
	}
	method = parts[0];
	path = parts[1];
	version = parts[2];
	if(!validateMethod()) 
	{
		is_valid = false;
		error_code = 405;
		return;
	}
	if(!validateVersion()) 
	{
		is_valid = false;
		error_code = 400;
		return;
	}
	is_valid = true;
	error_code = 0;
}
	
bool Parse::validateMethod() const
{
	if(method == "GET" || method == "POST" || method == "DELETE")
		return(true);
	return(false);
}

bool Parse::validateVersion() const
{
	if(version == "HTTP/1.1" || version == "HTTP/1.0")
		return(true);
	return(false);
}

std::vector<std::string> Parse::splitBySpace(const std::string &line) const
{
	std::vector<std::string> v;
	
	int start = 0;
	for(int i = 0; line[i]; i++)
	{
		if(line[i] == ' ')
		{
			  if (i > start)
				v.push_back(line.substr(start, i - start));
			start = i + 1;
		}
	}
	if (start < (int)line.size())
		v.push_back(line.substr(start));
	return(v);
}

//second line parsing

std::string Parse::trim(const std::string &s) const
{
	if(s.empty())
		return(s);

	int start = 0;
	int end = s.size() - 1;
	
	while(start < s.size() && (s[start] == ' ' || s[start] == '\t'))
		start++;
	while(end >= start && (s[end] == ' ' || s[end] == '\t'))
	{
			end--;
	}
	if(start > end)
		return(std::string());
	int len = end - start + 1;
	return(s.substr(start, len));
}


