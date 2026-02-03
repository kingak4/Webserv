/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 11:25:19 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/03 12:37:52 by kikwasni         ###   ########.fr       */
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

class Parse
{
	private:
			std::string raw_request;
			std::map<std::string, std::string> headers;
			std::string method, path, version;
			std::string body;
			bool is_valid;
	public:
		Parse();
		
};
			

#endif