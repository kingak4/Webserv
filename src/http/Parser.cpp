/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:29 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/05 14:59:04 by kikwasni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/Parser.hpp"

//constructors

Parser::Parser()
{
	raw_request = "";
	method = "";
	path = "";
	version = "";
	body = "";
	is_valid = false;
	error_code = 0;
}

Parser::Parser(const string &request)
{
	raw_request = request;
	method = "";
	path = "";
	version = "";
	body = "";
	is_valid = false;
	error_code = 0;
}
Parser::Parser(const Parser &other)
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

Parser::~Parser() {};

//getters

string Parser::get_Method() const {return method;}
string Parser::get_Path() const { return path; }
string Parser::get_Version() const { return version; }
string Parser::get_Body() const { return body; }
map<string, string> Parser::get_Headers() const { return headers; }
string Parser::get_Raw_Request() const { return raw_request; }
bool Parser::is_Valid() const { return is_valid; }
int Parser::get_Error_Code() const { return error_code; }

// parsing the first line

void Parser::parse_First_Line()
{
	if(raw_request.empty())
	{
		is_valid = false;
		 error_code = 400;
		return ;
	}
	size_t pos = raw_request.find("\r\n");
	string first_line = raw_request.substr(0, pos);
	if (pos == string::npos)
	{
		is_valid = false;
		error_code = 400;
		return;
	}
	vector<string> parts = split_By_Space(first_line);
	if(parts.size() != 3) 
	{
		is_valid = false;
		error_code = 400;
		return ;
	}
	method = parts[0];
	path = parts[1];
	version = parts[2];
	if(!validate_Method()) 
	{
		is_valid = false;
		error_code = 405;
		return;
	}
	if(!validate_Version()) 
	{
		is_valid = false;
		error_code = 400;
		return;
	}
	is_valid = true;
	error_code = 0;
}
	
bool Parser::validate_Method() const
{
	if(method == "GET" || method == "POST" || method == "DELETE")
		return(true);
	return(false);
}

bool Parser::validate_Version() const
{
	if(version == "HTTP/1.1" || version == "HTTP/1.0")
		return(true);
	return(false);
}

vector<string> Parser::split_By_Space(const string &line) const
{
	vector<string> v;
	
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

//second header line parsing

string Parser::trim(const string &s) const
{
	if(s.empty())
		return(s);
	size_t start = 0;
	size_t end = s.size() - 1;
	while(start < s.size() && (s[start] == ' ' || s[start] == '\t'))
		start++;
	while(end >= start && (s[end] == ' ' || s[end] == '\t'))
			end--;
	if(start > end)
		return(string());
	size_t len = end - start + 1;
	return(s.substr(start, len));
}


bool Parser::is_Header_Line_Valid(const string &line) const
{
	if(line.empty())
		return(false);
	size_t pos = line.find(":");
	if (pos == string::npos || pos == 0)
		return(false);
	string s = line.substr(0, pos);
	if(trim(s).empty())
		return(false);
	return(true);
}


pair<string, string> Parser::split_Header(const string &line) const
{
	size_t pos = line.find(":");
	string key_raw = line.substr(0, pos);
	string value_raw = line.substr(pos + 1, line.size() - (pos + 1));
	string key = trim(key_raw);
	string value = trim(value_raw);
	return (make_pair(key, value));

}


vector<string> Parser::split_Lines(const string &block) const
{
	vector<string> lines;
	size_t start = 0;
	while (start < block.size())
	{
		size_t pos = block.find("\r\n", start);
		if (pos == string::npos)
		{
			lines.push_back(block.substr(start, block.size() - start));
			break;
		}
		lines.push_back(block.substr(start, pos - start));
		start = pos + 2;
	}
	return(lines);
}


string Parser::extract_Headers_Block() const
{
	if(raw_request.empty())
		return(string());
	size_t pos = raw_request.find("\r\n");
	if (pos == string::npos)
		return(string());
	size_t headers_start = pos + 2;
	size_t po = raw_request.find("\r\n\r\n", headers_start);
	if (po == string::npos)
		return(string());
	string headers_block = raw_request.substr(headers_start, po - headers_start);
	return(headers_block);
}

bool Parser::has_Required_Headers() const
{
	if(headers.empty())
		return(false);
	map<string, string>::const_iterator it = headers.find("host");
	if (it == headers.end())
		return false;
	if (it->second.empty())
		return false;
	return(true);
}

void Parser::parse_Headers()
{
	headers.clear();
	string headers_block = extract_Headers_Block();
	if(headers_block.empty())
	{
		is_valid = false;
		error_code = 400;
		return;
	}
	vector<std::string> lines = split_Lines(headers_block);
	for (size_t i = 0; i < lines.size(); i++)
	{
		if(!is_Header_Line_Valid(lines[i]))
		{
			is_valid = false;
			error_code = 400;
			return;
		}
		std::pair<std::string, std::string> kv = split_Header(lines[i]);
		std::string key = kv.first;
		for (size_t j = 0; j < key.size(); j++)
			key[j] = std::tolower(key[j]);
		headers[key] = kv.second;
	}
	if(!has_Required_Headers())
	{
		is_valid = false;
		error_code = 400;
		return;
	}
	is_valid = true;
	error_code = 0;
}

// body parsing 

int Parser::get_Content_Length()
{
	if(headers.empty())
		return(false);
	map<string, string>::const_iterator it = headers.find("content-length");
	if (it == headers.end())
		return (0);
	string s = it->second;
	if(s.empty())
	{
		is_valid = false;
		error_code = 400;
		return(0);
	}
	char *end;
	errno = 0;
	long len = std::strtol(s.c_str(), &end, 10);
	if (*end != '\0')
	{
		is_valid = false;
		error_code = 400;
		return(0);
	}
	if (errno != 0 || len <= 0 || len > 2147483647)
	{
		is_valid = false;
		error_code = 400;
		return(0);
	}
	return(len);
}

string Parser::extract_Body_Block() 
{
	if(raw_request.empty())
		return(string());
	size_t pos = raw_request.find("\r\n\r\n");
	if (pos == string::npos)
		return(string());
	size_t body_start = pos + 4;
	map<string, string>::const_iterator it =  headers.find("content-length");
	if (it != headers.end())
	{
		int len = get_Content_Length();
		size_t body_end = body_start + len;
		if(body_end > raw_request.size())
			body_end  = raw_request.size();
		 return(raw_request.substr(body_start, body_end - body_start));
	}
	return raw_request.substr(body_start);
}

bool Parser::validate_Body_Length()
{
	string body = extract_Body_Block();
	int len = get_Content_Length();
	if(body.size() != static_cast<size_t>(len))
		return(false);
	return(true);
}

void Parser::parse_Body()
{
	body = extract_Body_Block();
	if(validate_Body_Length() == false)
	{
		is_valid = false;
		error_code = 400;
		return;
	}
	is_valid = true;
	error_code = 0;
}





