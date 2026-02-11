/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:34 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/03 12:34:32 by kikwasni         ###   ########.fr       */
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
std::string Response::get_Raw_Response() const {return raw_response;}

//functions

void Response::reset()
{
	status_code = 0;
	status_text = "";
	headers.clear();
	body = "";
	raw_response = "";
}