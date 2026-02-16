/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:30 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/16 10:49:44 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config/Route.hpp"

Route::Route(Location& loc, Request& request, Config& server_block) : server(server_block), request(request)
{
    route_name = loc.route_name;
    url = loc.url;
    
    vector<string> method = loc.allowed_methods;
    allowed_methods = loc.allowed_methods;
    autoindex = loc.autoindex;

    request_method = request.get_Method();
    cout << "request_method: " << request_method << endl;
    request_full_path = request.get_Path();
    cout << "request_full_path: " << request_full_path << endl;
    request_version = request.get_Version();
    cout << "request_version: " << request_version << endl;
    request_headers = request.get_Headers();
    
    // for (map<string, string>::iterator it = request_headers.begin();
    //     it != request_headers.end(); ++it)
    // {
    //     cout << it->first << ": " << it->second << endl;
    // }

    request_path = trim(retrieve_request_path(request_full_path));
    cout << "request_path: " << request_path << endl;
    request_query = retrieve_request_query(request_full_path);
    cout << "request_query: " << request_query << endl;

    filesystem_path = server.get_root_dir() + request_path;
    cout << "filesystem_path: " << filesystem_path << endl;
}

Route::~Route() {}

string Route::retrieve_request_path(const string& request_full_path)
{
    size_t pos;
    string res;

    if ((pos = request_full_path.find('?')) != string::npos)
        res = request_full_path.substr(0, pos);
    else
        res = request_full_path;

    if (res.empty())
        res = "/"; // default to root path

    return res;
}


string Route::retrieve_request_query(const string& request_full_path)
{
    int len;
    size_t pos;
    string res;
    
    len = request_full_path.length();
    if ((pos = request_full_path.find('?')) != string::npos)
        res = request_full_path.substr(pos + 1, len);
    else
        res = "";
    return res;
}

bool Route::is_valid_request_path()
{
    cout << "url: " << url << endl;
    return request_path == url;
}

// check if request method coincides with config file allowed method
bool Route::is_allowed_method()
{
    for (size_t i = 0; i < allowed_methods.size(); i++)
    {
        if (allowed_methods[i] == request_method)
            return true;       
    }
    return false;
}

bool Route::is_cgi()
{
    if (url.find("/usr/bin/") == 0 or request_path.find(".py") == 0)
        return true;
    return false;
}

FsType Route::get_filesystem_type()
{
    struct stat buffer;
    
    if (stat(filesystem_path.c_str(), &buffer) != 0)
        return FS_NOT_FOUND;
    
    if (S_ISREG(buffer.st_mode))
        return FS_IS_FILE;

    if (S_ISDIR(buffer.st_mode))
        return FS_IS_DIR;
        
    return FS_NOT_FOUND;
}

string Route::trim(const string& str)
{
    size_t start;
    size_t end;
    
    if (str.empty())
        return "";

    start = 0;
    end = str.size() - 1;

    while (start <= end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n'))
        ++start;

    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n'))
        --end;

    return str.substr(start, end - start + 1);
}

string Route::serve_directory_listing(string& filesystem_path)
{
    DIR *dir = opendir(filesystem_path.c_str());
    if (!dir)
        return error_response("www/errors/500.html");

    ostringstream body;
    body << "<html>\n";
    body << "<head><title>Index of " << url << "</title></head>\n";
    body << "<body>\n";
    body << "<h1>Index of " << url << "</h1>\n";
    body << "<hr>\n";
    body << "<ul>\n";

    struct dirent *entry = NULL;
    while ((entry = readdir(dir)) != NULL)
    {
        string name = entry->d_name;

        if (name == ".")
            continue ;

        body << "<li><a href=\"" << url;

        if (url[url.length() - 1] != '/')
            body << "/";

        body << name << "\">" << name << "</a></li>\n";
    }
    
    body << "</ul>\n";
    body << "<hr>\n";
    body << "</body>\n";
    body << "</html>\n";

    closedir(dir);

    string body_str = body.str();

    ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << body_str.size() << "\r\n";
    response << "Connection: close\r\n\r\n";
    response << body_str;

    return response.str();
}

string Route::handle_autoindex()
{
    struct stat st;
    string index_path;

    index_path = filesystem_path + "/" + url;

    if (autoindex == "off")
    {
        if (stat(index_path.c_str(), &st) == 0 && S_ISREG(st.st_mode))
        {
            filesystem_path = index_path;
            return read_static_file(index_path);
        }
        else
            return error_response("www/errors/404.html");
    }
    else if (autoindex == "on")
        return serve_directory_listing(filesystem_path);

    return error_response("www/errors/500.html");
}

string Route::find_abs_path(string file)
{
    char abs_path[PATH_MAX];

    if (realpath(file.c_str(), abs_path) != NULL)
    {
        string file_abs_path(abs_path);
        return file_abs_path;
    }
    cout << "Path to the error file not found" << endl;
    return NULL;        
}

string Route::error_response(string error_file)
{
    string error_file_abs_path;

    error_file_abs_path = find_abs_path(error_file);
    
    ifstream file(error_file_abs_path.c_str());
    if (!file.is_open())
    {
        cout << "Coudn't open a file" << endl;
        return NULL;   
    }

    ostringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

string Route::read_static_file(string filesystem_path)
{
    ifstream file(filesystem_path.c_str());

    if (!file.is_open())
        return error_response("www/errors/500.html");

    ostringstream buffer;
    buffer << file.rdbuf();
    string file_content = buffer.str();
    
    string content_type = "text/html";

    ostringstream headers;
    headers << "HTTP/1.1 200 OK\r\n";
    headers << "Content-Type: " << content_type << "\r\n";
    headers << "Content-Length: " << file_content.size() << "\r\n";
    headers << "Connection: close\r\n\r\n";
    headers << "\r\n";

    string response = headers.str() + file_content;
    return response;
}

string Route::serve_static_file()
{
    struct stat st;

    if (stat(filesystem_path.c_str(), &st) == 0 && S_ISREG(st.st_mode))
        return read_static_file(filesystem_path);
    else
        return error_response("www/errors/404.html");
}

void Route::is_valid_request()
{
    bool method_is_allowed;
    bool body_matches_size;

    method_is_allowed = is_allowed_method();
    // cout << "method_is_allowed: " << method_is_allowed << endl;
    if (!method_is_allowed)
        error_response("www/errors/405.html");

    body_matches_size = server.does_body_match_size();
    // cout << "body_matches_size: " << body_matches_size << endl;
    if (!body_matches_size)
        error_response("www/errors/413.html");
}

string Route::form_response()
{
    FsType filesystem_status;

    is_valid_request();
    // cout << "request_is_valid: " << request_is_valid << endl;

    filesystem_status = get_filesystem_type();
    // cout << "filesystem_status: " << filesystem_status << endl;
    switch(filesystem_status)
    {
        case FS_NOT_FOUND:
            return error_response("www/errors/404.html");
        case FS_IS_FILE:
            if (is_cgi())
            {
                CgiHandler cgi(*this, request);
                return cgi.run();   
            }
            else
                return serve_static_file();
        case FS_IS_DIR:
            return handle_autoindex();
    }
    return "";
}

const string& Route::get_route_name() const
{
    return route_name;
}

const string& Route::get_url() const
{
    return url;
}

const vector<string>& Route::get_allowed_methods() const
{
    return allowed_methods;
}

const string& Route::get_autoindex() const
{
    return autoindex;
}

const string& Route::get_filesystem_path() const
{
    return filesystem_path;
}

const string& Route::get_request_query() const
{
    return request_query;
}
