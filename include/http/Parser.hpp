/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:19 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/09 17:53:21 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

# include <algorithm>
# include <fstream>
# include <iostream>
# include <map>
# include <sstream>
# include <string>
# include <vector>
# include <string>

class Parser
{
	private:
			std::string raw_request;
			std::map<std::string, std::string> headers;
			std::string method, path, version;
			std::string body;
			bool is_valid;
	public:
		Parser();

		string get_Path() const;
		string get_Version() const;
		string get_Body() const;
		map<string, string> get_Headers() const;
		string get_Raw_Request() const;
		bool is_Valid() const;
		int get_Error_Code() const;
		//parsing the first line
		void parse_First_Line();
		vector<string> split_By_Space(const string &line) const;
		bool validate_Version() const;
		bool validate_Method() const;
		//second header line parsing
		string trim(const string &s) const;
		bool is_Header_Line_Valid(const string &line) const;
		pair<string, string> split_Header(const string &line) const;
		vector<string> split_Lines(const string &block) const;
		string extract_Headers_Block() const;
		bool has_Required_Headers() const;
		void parse_Headers();
		// body parsing 
		int get_Content_Length();
		string extract_Body_Block();
		bool validate_Body_Length();
		void parse_Body();
		// chunked body parsing 
		int hex_To_Int(const std::string &hex);
		std::string toLower(const std::string &s);
		bool is_Chunked();
		std::string extract_Chunked_Body();
		bool validate_Chunks();
		void parse_Chunked_Body();
		// final function 
		void parse_Request();
};
			

#endif