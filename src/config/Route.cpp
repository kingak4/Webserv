/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikwasni <kikwasni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:30 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/20 11:38:30 by kikwasni         ###   ########.fr       */
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
    request_full_path = request.get_Path();
    request_version = request.get_Version();
    request_headers = request.get_Headers();

    request_path = trim(retrieve_request_path(request_full_path));
    request_query = retrieve_request_query(request_full_path);

    filesystem_path = server.get_root_dir() + request_path;
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
        res = "/";

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
    return request_path.find(url) == 0;
}

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
    if (request_path.find("/cgi-bin") == 0 or url.find(".py") == 0)
        return true;
    return false;
}

FsType Route::get_filesystem_type()
{
    struct stat buffer;
    
    // cout << "filesystem_path: " << filesystem_path << endl;
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
        return error_response("500");

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
    // cout << "index_path: " << index_path << endl;

    if (autoindex == "off")
    {
        if (stat(index_path.c_str(), &st) == 0 && S_ISREG(st.st_mode))
        {
            filesystem_path = index_path;
            return read_static_file(index_path);
        }
        else
        {
            // cout << "ERORR" << endl;
            return error_response("404");   
        }
    }
    else if (autoindex == "on")
        return serve_directory_listing(filesystem_path);

    return error_response("500");
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

string Route::error_response(string error)
{
    string error_file_abs_path;
    string body;

    error_file_abs_path = find_abs_path("www/errors/" + error + ".html");
    
    ifstream file(error_file_abs_path.c_str());
    if (!file.is_open())
    {
        cout << "Coudn't open a file" << endl;
        return NULL;   
    }

    ostringstream buffer;
    buffer << file.rdbuf();
    body = buffer.str();

    ostringstream response;
    response << "HTTP/1.1 " << error << "\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    response << "\r\n";
    response << body;

    return response.str();
}


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

string Route::read_static_file(string filesystem_path)
{
    ifstream file(filesystem_path.c_str(), ios::binary);

    if (!file.is_open())
        return error_response("500");

    ostringstream buffer;
    buffer << file.rdbuf();
    string file_content = buffer.str();
    
    string content_type = get_Mime_Type(filesystem_path.c_str());

    ostringstream headers;
    headers << "HTTP/1.1 200 OK\r\n";
    headers << "Content-Type: " << content_type << "\r\n";
    headers << "Content-Length: " << file_content.size() << "\r\n";
    headers << "Connection: close\r\n\r\n";

    string response = headers.str() + file_content;
    return response;
}

string Route::serve_static_file()
{
    struct stat st;

    if (stat(filesystem_path.c_str(), &st) == 0 && S_ISREG(st.st_mode))
        return read_static_file(filesystem_path);
    else
        return error_response("404");
}

bool Route::is_valid_request()
{
    bool method_is_allowed;
    bool body_matches_size;

    method_is_allowed = is_allowed_method();
    if (!method_is_allowed)
        return false;

    body_matches_size = server.does_body_match_size();
    if (!body_matches_size)
        return false;
    
    return true;
}

string Route::handle_delete()
{
    struct stat st;

    if (stat(filesystem_path.c_str(), &st) != 0)
        return error_response("404");

    if (!S_ISREG(st.st_mode))
        return error_response("403");

    if (unlink(filesystem_path.c_str()) != 0)
        return error_response("403");

    ostringstream headers;
    headers << "HTTP/1.1 204 No Content\r\n";
    headers << "Content-Length: 0\r\n";
    headers << "Connection: close\r\n";
    headers << "\r\n";

    return headers.str();
}

string Route::handle_upload()
{
    cout << "Handle file uploads here" << endl;
    return "Handle file uploads here";
}

string Route::form_response()
{
    FsType filesystem_status;
    string cgi_output;

    if (!is_valid_request())
    {
        if (!is_allowed_method())
            return error_response("405");
        else
            return error_response("413");
    }

    filesystem_status = get_filesystem_type();
    
    if (request.get_Method() == "DELETE")
        return handle_delete();

    if (request.get_Method() == "POST" && !is_cgi())
        return handle_upload();
        
    switch(filesystem_status)
    {
        case FS_NOT_FOUND:
            // cout << "FS_NOT_FOUND" << endl;
            return error_response("404");
        case FS_IS_FILE:
            // cout << "FS_IS_FILE" << endl;
            if (is_cgi())
            {
                CgiHandler cgi(*this, request);
                cgi_output = cgi.run();
                return cgi.build_cgi_response(cgi_output);    
            }
            else
                return serve_static_file();
        case FS_IS_DIR:
            // cout << "FS_IS_DIR" << endl;
            if (is_cgi())
            {
                // cout << "IS_CGI" << endl;
                CgiHandler cgi(*this, request);
                cgi_output = cgi.run();
                return cgi.build_cgi_response(cgi_output);   
            }
            else
                return handle_autoindex(); 
    }
    return error_response("500");
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

const string& Route::get_request_path() const
{
    return request_path;
}
