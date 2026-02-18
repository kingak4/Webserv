/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alraltse <alraltse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:31 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/17 13:47:31 by alraltse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/Request.hpp"

//constructors

Request::Request()
{
	method = "";
	path = "";
	version = "";
	body = "";
	is_valid = false;
	error_code = 0;
}

Request::Request(const Request &other)
{
	if(this != &other)
	{
		method = other.method;
		path = other.path;
		version = other.version;
		body = other.body;
		headers = other.headers;
		is_valid = other.is_valid;
		error_code = other.error_code;
	}
}

Request::~Request() {};

//getters

string Request::get_Method() const {return method;}
string Request::get_Path() const { return path; }
string Request::get_Version() const { return version; }
string Request::get_Body() const { return body; }
map<string, string> Request::get_Headers() const { return headers; }
bool Request::is_Valid() const { return is_valid; }
int Request::get_Error_Code() const { return error_code; }

//functions
void  Request::reset()
{
	method = "";
	path = "";
	version = "";
	body = "";
	headers.clear();
	is_valid = false;
	error_code = 0;
}

void Request::buildFromParser(const Parser& parser)
{
	reset();
	if(!parser.is_Valid())
	{
		is_valid = false;
		error_code = parser.get_Error_Code();
		return;
	}
	is_valid = true;
	error_code = 0;
	method = parser.get_Method();
	path  = parser.get_Path();
	version = parser.get_Version();
	body = parser.get_Body();
	headers = parser.get_Headers();
}
