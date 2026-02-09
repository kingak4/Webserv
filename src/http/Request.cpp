/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:31 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/03 12:34:27 by kikwasni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/Request.hpp"

//constructors

Request::Request()
{
	raw_request = "";
	method = "";
	path = "";
	version = "";
	body = "";
	is_valid = false;
	error_code = 0;
}

Request::Request(const string &request) 
{
	raw_request = request;
	method = "";
	path = "";
	version = "";
	body = "";
	headers.clear();
	is_valid = false;
	error_code = 0;
}

Request::Request(const Request &other)
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

Request::~Request() {};

//getters

string Request::get_Method() const {return method;}
string Request::get_Path() const { return path; }
string Request::get_Version() const { return version; }
string Request::get_Body() const { return body; }
map<string, string> Request::get_Headers() const { return headers; }
string Request::get_Raw_Request() const { return raw_request; }
bool Request::is_Valid() const { return is_valid; }
int Request::get_Error_Code() const { return error_code; }

