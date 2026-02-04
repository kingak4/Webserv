/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:19 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/04 14:22:34 by kikwasni         ###   ########.fr       */
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

class Parse
{
	private:
			std::string raw_request;
			std::map<std::string, std::string> headers;
			std::string method, path, version;
			std::string body;
			bool is_valid;
			int error_code;
	public:
		Parse();
		Parse(const std::string &request);
		Parse(const Parse &other);
		~Parse();
		std::string getMethod() const;
		std::string getPath() const;
		std::string getVersion() const;
		std::string getBody() const;
		std::map<std::string, std::string> getHeaders() const;
		std::string getRawRequest() const;
		bool isValid() const;
		int getErrorCode() const;
		void parseFirstLine();
		std::vector<std::string> splitBySpace(const std::string &line) const;
		bool validateVersion() const;
		bool validateMethod() const;
};
			

#endif