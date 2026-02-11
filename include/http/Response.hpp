/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:25 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/03 11:34:28 by kikwasni         ###   ########.fr       */
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

using namespace std;

class Response
{
	private:
		int status_code;
		string status_text;
		map<string, string> headers;
		string body;
		string raw_response;
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
		//functions
		void reset();

};


#endif