/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:25 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/12 16:18:34 by kikwasni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <dirent.h>
# include <iostream>
# include <string>
# include <vector>
# include <cstdlib>
# include <list>
# include <map>
# include <sstream>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <cstdio> 
# include <ctime>
# include "HttpUtils.hpp"

using namespace std;

class Response
{
	private:
		int status_code;
		string status_text;
		map<string, string> headers;
		string body;
		string raw_response;
		bool is_built;
	public:
		//constructors
		Response();
		Response(const Response &other);
		~Response();
		//getters
		int get_Status_Code() const;
		string get_Status_Text() const;
		map<string, string> get_Headers() const;
		map<string, string> get_Headers() const;
		string get_Body() const;
		string get_Raw_Response() const;
		bool is_Valid() const;
		int get_Status_Code() const;
		string get_Status_text() const;
		bool Response::get_is_Built() const;
		//helpers
		void reset();
		void add_Header(const string& key , const string& value);
		void set_Body(const string& content);
		string build_Status_Line() const;
		//functions
		void finalize_Response();
		void build_Error_Response(int code, const ServerConfig&);
		void buildResponse(const Request&, const Route&, const ServerConfig&);
		void handleGet(const Request&, const Route&);
		void handlePost(const Request&, const Route&);
		void handleDelete(const Request&, const Route&);
		string getRawResponse() const;

};


#endif