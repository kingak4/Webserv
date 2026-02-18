/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alraltse <alraltse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 10:24:28 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/18 14:05:24 by alraltse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_UTILS_HPP
# define HTTP_UTILS_HPP

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
# include "../../include/http/Request.hpp"
# include "../../include/config/Route.hpp"

using namespace std;

bool file_Exists(const string& path);
bool is_Directory(const string& path);
string read_File(const string& path);
string get_Mime_Type(const string& path);
string get_Current_Date_RFC();
string generateAutoindex(const std::string& directoryPath, const std::string& requestUri);
string resolve_Path(const Request &request, const Route &route);



#endif