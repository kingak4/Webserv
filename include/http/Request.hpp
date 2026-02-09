/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:22 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/03 11:34:02 by kikwasni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

# include <iostream>
# include <list>
# include <map>
# include <sstream>
# include <string>
# include <sys/stat.h>
# include "Parser.hpp"
# include <vector>
# include <cstdlib>

using namespace std;

class Request
{
	private:
			string raw_request;
			map<string, string> headers;
			string method, path, version;
			string body;
			bool is_valid;
			int error_code;
	public:
			//constructors
			Request();
			Request(const string &request);
			Request(const Request &other);
			~Request();
			//getters
			string get_Method() const;
			string get_Path() const;
			string get_Version() const;
			string get_Body() const;
			map<string, string> get_Headers() const;
			string get_Raw_Request() const;
			bool is_Valid() const;
			int get_Error_Code() const;


}

#endif