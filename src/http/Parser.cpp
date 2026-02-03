/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:29 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/03 14:21:50 by kikwasni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/Parser.hpp"

Parse::Parse()
{
	raw_request = "";
	method = "";
	path = "";
	version = "";
	body = "";
	is_valid = false;
}

Parse::Parse(const std::string &request)
{
	raw_request = request;
	method = "";
	path = "";
	version = "";
	body = "";
	is_valid = false;
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
	}
}

Parse::~Parse() {};


std::string Parse::getPath() const { return path; }
std::string Parse::getVersion() const { return version; }
std::string Parse::getBody() const { return body; }
std::map<std::string, std::string> Parse::getHeaders() const { return headers; }
std::string Parse::getRawRequest() const { return raw_request; }
bool Parse::isValid() const { return is_valid; }

void Pars::parseFirstLine()
{
	if(!raw_request.empty())
	{
		is_valid = false;
		return ;
	}
	
}

//    znajdź pozycję pierwszego znaku nowej linii (\r\n lub \n)
//    wyciągnij substring od początku do tej pozycji → to jest first_line

//    podziel first_line po spacjach na trzy części:
//        części[0] → metoda
//        części[1] → ścieżka (path)
//        części[2] → wersja (HTTP/1.1)

//    jeśli liczba części != 3
//        ustaw is_valid = false
//        zakończ funkcję

//    przypisz części do zmiennych obiektu:
//        method = części[0]
//        path = części[1]
//        version = części[2]

//    sprawdź poprawność method i version:
//        jeśli metoda nie jest GET/POST/DELETE
//            is_valid = false
//        jeśli wersja nie jest HTTP/1.1 lub HTTP/1.0
//            is_valid = false

//    jeśli wszystko poprawne
//        is_valid = true
//koniec funkcji

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
