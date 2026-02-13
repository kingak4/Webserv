/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:34 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/13 11:04:28 by kikwasni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/Response.hpp"

Response::Response()
{
	status_code = 0;
	status_text = "";
	body = "";
	raw_response = "";
	is_built = false;
}

Response::Response(const Response &other)
{
	if (this != &other)
	{
		status_code = other.status_code;
		status_text = other.status_text;
		headers = other.headers;
		body = other.body;
		raw_response = other.raw_response;
		is_built = other.is_built;
	}
}

Response::~Response() {};

// getters

int Response::get_Status_Code() const { return status_code; }
string Response::get_Status_Text() const { return status_text; }
string Response::get_Body() const { return body; }
map<string, string> Response::get_Headers() const { return headers; }
string Response::get_Raw_Response() const { return raw_response; }
bool Response::get_is_Built() const { return is_built };
// helpers

void Response::reset()
{
	status_code = 0;
	status_text = "";
	headers.clear();
	body = "";
	raw_response = "";
	is_built = false;
}

void Response::add_Header(const string &key, const string &value)
{
	if (key.empty())
		return;
	headers[key] = value;
}

void Response::set_Body(const string &content)
{
	body = content;
	std::stringstream ss;
	ss << content.size();
	add_Header("Content-Length", ss.str());
}

string Response::build_Status_Line() const
{
	int code = get_Status_Code();
	string text = get_Status_Text();

	std::stringstream ss;
	ss << "HTTP/1.1 " << code << " " << text << "\r\n";
	return ss.str();
}

// functions

void Response::finalize_Response()
{
	string raw = build_Status_Line();
	raw += "Date: " + get_Current_Date_RFC() + "\r\n";
	raw += "Server: webserv\r\n";
	raw += "Connection: close\r\n";
	if (headers.find("Content-Length") == headers.end())
	{
		std::stringstream ss;
		ss << body.size();
		add_Header("Content-Length", ss.str());
	}
	map<string, string>::const_iterator it;
	for (it = headers.begin(); it != headers.end(); ++it)
		raw += it->first + ": " + it->second + "\r\n";
	raw += "\r\n";
	raw += body;
	raw_response = raw;
	is_built = true;
}

// void Response::build_Error_Response(int code, const ServerConfig& conf)
// {
// }

//void build_Response(const Request& request, const Route& route, const ServerConfig&)
{
	// reset();
	

	// if (method == GET)
	// 	handle_Get();
	// else if (method == POST)
	// 	handle_Post()
	// else if (metoda == DELETE)
	// 	handle_Delete()
	// else
	// 	build_Error_Response(405);
	// if (status_code  == 0)
	// 	build_Error_Response(500)
	// finalize_Response();
}

// void Response::handle_Get(const Request& request, const Route& route)
// {
// 	string path;
// 	//  path = route.root + request.uri
// 	if (path.empty())
// 	{
// 		//         build_Error_Response(404)
// 		return;
// 	}
// }

// void Response::handle_Post(const Request& request, const Route& route)
// {
	
// }

// void Response::handle_Delete(const Request& request, const Route& route)
// {

// }

// need to do this functions with Alina