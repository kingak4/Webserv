/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 10:22:55 by kikwasni          #+#    #+#             */
/*   Updated: 2026/02/17 12:35:38 by kikwasni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpUtils.hpp"

// bool file_Exists(const string &path)
// {
// 	struct stat file_info;
// 	if (path.empty())
// 		return (false);
// 	if (stat(path.c_str(), &file_info) == 0)
// 		return (true);
// 	return (false);
// }

// bool is_Directory(const string &path)
// {
// 	struct stat file_info;
// 	if (path.empty())
// 		return (false);
// 	if (stat(path.c_str(), &file_info) == -1)
// 		return (false);
// 	if (S_ISDIR(file_info.st_mode))
// 		return (true);
// 	return (false);
// }

// string read_File(const string &path)
// {
// 	if (path.empty())
// 		return (string());
// 	int fd = open(path.c_str(), O_RDONLY);
// 	if (fd == -1)
// 		return (string());
// 	string content;
// 	char c;
// 	while (read(fd, &c, 1) > 0)
// 	{
// 		content += c;
// 	}
// 	close(fd);
// 	return (content);
// }

string get_Mime_Type(const string &path)
{
	if (path.empty())
		return (string("text/plain"));
	size_t pos = path.find_last_of('.');
	if (pos == string::npos)
		return (string("application/octet-stream"));
	string line = path.substr(pos + 1);
	if (line == "html")
		return (string("text/html"));
	else if (line == "css")
		return (string("text/css"));
	else if (line == "js")
		return (string("application/javascript"));
	else if (line == "png")
		return (string("image/png"));
	else if (line == "jpg" || line == "jpeg")
		return (string("image/jpeg"));
	else if (line == "txt")
		return (string("text/plain"));
	return (string("application/octet-stream"));
}

string get_Current_Date_RFC()
{
	time_t current_time = time(NULL);
	struct tm *time_struct = gmtime(&current_time);
	if (!time_struct)
		return (std::string());
	const char *days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
							  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	char buffer[100];
	sprintf(buffer, "%s, %02d %s %04d %02d:%02d:%02d GMT",
			days[time_struct->tm_wday],
			time_struct->tm_mday,
			months[time_struct->tm_mon],
			time_struct->tm_year + 1900,
			time_struct->tm_hour,
			time_struct->tm_min,
			time_struct->tm_sec);
	return (std::string(buffer));
}

// string generateAutoindex(const std::string &directoryPath, const std::string &requestUri)
// {

// 	DIR *dir = opendir(directoryPath.c_str());
// 	if (dir == NULL)
// 		return (string());
// 	string html = "";
// 	html += "<html>";
// 	html += "<head><title>Index of " + requestUri + "</title></head>";
// 	html += "<body>";
// 	html += "<h1>Index of " + requestUri + "</h1>";
// 	html += "<ul>"; 
// 	struct dirent *entry;
// 	while ((entry = readdir(dir)) != NULL)
// 	{
// 		string fileName = entry->d_name;
// 		if (fileName == "." || fileName == "..")
// 			continue;
// 		string fullPath = directoryPath + "/" + fileName;
// 		struct stat fileInfo;
// 		if (stat(fullPath.c_str(), &fileInfo) == 0 && S_ISDIR(fileInfo.st_mode))
// 		{
// 			fileName += "/";
// 		}
// 		string fullLink = requestUri + fileName; 
// 		html += "<li>";
// 		html += "<a href=\"" + fullLink + "\">";
// 		html += fileName;
// 		html += "</a>";
// 		html += "</li>";
// 	}
// 	closedir(dir);
// 	html += "</ul>";
// 	html += "</body>";
// 	html += "</html>";
// 	return (html);
// }
