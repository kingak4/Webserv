/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:34 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/12 11:22:06 by kikwasni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/Response.hpp"

Response::Response()
{
	status_code = 0;
	status_text = "";
	body = "";
	raw_response = "";
}

Response::Response(const Response &other)
{
	if(this != &other)
	{
		status_code = other.status_code;
		status_text = other.status_text;
		headers = other.headers;
		body = other.body;
		raw_response = other.raw_response;
	}
}

Response::~Response() {};

//getters

int Response::get_Status_Code() const {return status_code;}
string Response::get_Status_Text() const {return status_text;}
string Response::get_Body() const {return body;}
map<string, string> Response::get_Headers() const {return headers;}
string Response::get_Raw_Response() const {return raw_response;}

//helpers

void Response::reset()
{
	status_code = 0;
	status_text = "";
	headers.clear();
	body = "";
	raw_response = "";
}

void Response::add_Header(const string& key , const string& value)
{
	if(key.empty())
		return;
	headers[key] = value;
}

void Response::set_Body(const string& content)
{
	body = content;
	std::stringstream ss;
	ss << content.size();
	add_Header("Content-Length", ss.str());
}

string  Response::build_Status_Line() const
{
	int code = get_Status_Code();
	string text = get_Status_Text();

	std::stringstream ss;
	ss << "HTTP/1.1 " << code << " " << text << "\r\n";
	return ss.str();
}

//functions



