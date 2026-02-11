/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:30 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/11 16:05:08 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config/Route.hpp"

// Route::Route(Config& server_block) : server(server_block) {};

Route::Route(Location& loc, Request& request, Config& server_block) : server(server_block)
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
    if (url.find("/usr/bin/") == 0 or url.find(".py") == 0)
        return true;
    return false;
}

FsType Route::get_filesystem_type()
{
    struct stat buffer; // stat holds metadata about file
    
    if (stat(filesystem_path.c_str(), &buffer) != 0) // buffer.st_mode contains data about file type and permissions;
        return FS_NOT_FOUND; // 0
    
    if (S_ISREG(buffer.st_mode))
        return FS_IS_FILE; // 1

    if (S_ISDIR(buffer.st_mode))
        return FS_IS_DIR; // 2
        
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

// void Route::serve_directory_listing(string& filesystem_path)
// {
    
// }

void Route::handle_autoindex()
{
    struct stat st;
    string index_path;

    index_path = filesystem_path + "/" + url;

    if (autoindex == "off") // serve index.html
    {
        if (stat(filesystem_path.c_str(), &st) == 0 && S_ISREG(st.st_mode))
            filesystem_path = index_path;
        else
            // 403 or 404 error
            return ;
    }
    // else if (autoindex == "on")
    //     serve_directory_listing(filesystem_path)
}

void Route::serve_static_file()
{
    struct stat st;

    if (stat(filesystem_path.c_str(), &st) == 0 && S_ISREG(st.st_mode))
    {
        // form and send response
        cout << "Filepath is normal file" << endl;
    }
    else
        // 404 error;
        return;
}

bool Route::is_valid_request()
{
    FsType filesystem_status;
    bool method_is_allowed;
    bool body_matches_size;

    // 1. CHECK ALLOWED METHODS:
    method_is_allowed = is_allowed_method();
    cout << "method_is_allowed: " << method_is_allowed << endl;
    // if (!method_is_allowed)
    // send response with 405 ERROR

    // 2. BUILD THE FILESYSTEM PATH

    filesystem_status = get_filesystem_type();
    cout << "filesystem_status: " << filesystem_status << endl;
    // switch(filesystem_status)
    // {
    //     case FS_NOT_FOUND:
    //         // 404 ERROR
    //     case FS_IS_FILE:
    //         if (is_cgi())
    //             // got to CgiHndler
    //         else
    //             serve_static_file(); // serve static file
    //     case FS_IS_DIR:
    //         handle_autoindex(); // handle autoindex
    // }

    // 3. IF THERE'S A BODY, VERIFY ITS SIZE AGAINST CLIENT_MAX_BODY_SIZE:
    body_matches_size = server.does_body_match_size();
    cout << "body_matches_size: " << body_matches_size << endl;
    // if (!body_matches_size)
    // send response with 413 ERROR
    
    return true;
}

void Route::form_response()
{
    bool request_is_valid;

    request_is_valid = is_valid_request();
    cout << "request_is_valid: " << request_is_valid << endl;
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
