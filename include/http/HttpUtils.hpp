/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 10:24:28 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/13 09:18:59 by kikwasni         ###   ########.fr       */
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

using namespace std;

bool file_Exists(const string& path);
bool is_Directory(const string& path);
string read_File(const string& path);
string get_Mime_Type(const string& path);
string get_Current_Date_RFC();
string generateAutoindex(const std::string& directoryPath, const std::string& requestUri);
string resolve_Path(const Request &request, const Route &route);



#endif