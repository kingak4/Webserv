/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:30 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/09 19:28:25 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config/Route.hpp"

Route::Route(Config& server_block) : server(server_block) {};

Route::Route(Location& loc, Parser request, Config& server_block) : server(server_block)
{
    route_name = loc.route_name;
    url = loc.url;
    allowed_methods = loc.allowed_methods;
    autoindex = loc.autoindex;

    request_method = request.get_Method();
    cout << "request_method: " << request_method << endl;
    request_full_path = request.get_Path();
    cout << "request_full_path: " << request_full_path << endl;
    request_version = request.get_Version();
    cout << "request_version: " << request_version << endl;
    request_headers = request.get_Headers();
    
    
    for (std::map<std::string, std::string>::iterator it = request_headers.begin();
        it != request_headers.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    request_path = trim(retrieve_request_path(request_full_path));
    cout << "request_path: " << request_path << endl;
    request_query = retrieve_request_query(request_full_path);
    cout << "request_query: " << request_query << endl;

    filesystem_path = server.get_root_dir() + "/" + request_path;
    cout << "filesystem_path: " << filesystem_path << endl;
    
    // request_method = "GET";
    // request_version = "HTTP/1.1";
    
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

// check if request method coincide with config file allowed method
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
        return FS_NOT_FOUND;
    
    if (S_ISREG(buffer.st_mode))
        return FS_IS_FILE;

    if (S_ISDIR(buffer.st_mode))
        return FS_IS_DIR;
        
    return FS_NOT_FOUND;
}

string Route::trim(string str)
{
    size_t start_idx;
    size_t end_idx;

    start_idx = str.find_first_not_of("/");
    end_idx = str.find_last_not_of("/");

    return str.substr(start_idx, end_idx - start_idx + 1);    
}

bool Route::is_valid_request()
{
    FsType filesystem_status;
    bool request_path_is_valid;
    bool method_is_allowed;
    bool body_matches_size;

    // 1. SPLIT REQUEST_FULL_PATH INTO REQUEST_PATH AND REQUEST_QUERY
    
    // 2. COMPARE REQUEST_PATH AGAINS LOCATION_URL
    request_path_is_valid = is_valid_request_path();
    cout << "request_path_is_valid: " << request_path_is_valid << endl;
    // if (!request_path_is_valid)
    // send error response

    // 3. CHECK ALLOWED METHODS:
    method_is_allowed = is_allowed_method();
    cout << "method_is_allowed: " << method_is_allowed << endl;
    // if (!method_is_allowed)
    // send response with 405 ERROR

    // 4. BUILD THE FILESYSTEM PATH

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
    //             // serve static file
    //     case FS_IS_DIR
    //         // handle autoindex
    // }

    // IF THE REQUEST PATH POINTS TO A DICTIONARY, CHECK IF AUTOINDEX IS ENABLED IN THE CONFIG
    

    // IF THERE'S A BODY, VERIFY ITS SIZE AGAINST CLIENT_MAX_BODY_SIZE:
    body_matches_size = server.does_body_match_size();
    cout << "body_matches_size: " << body_matches_size << endl;
    // if (!body_matches_size)
    // send response with 413 ERROR
    
    // CHECKS:
    // AUTOINDEX:
    // - if the request path points to a dictionary, check if autoindex is enabled in the config.
    // - respond with 403 or 404 or generate a directory listing?
    // CGI / SCRIPT HANDLING :
    // - : If the request points to a file that should be handled by CGI (like .php, .py, etc.), verify the route’s CGI rules.
    // Headers
    // - Host header → must match server_name.
    // - Required headers for CGI or special routes.
    // - Optional: check for forbidden headers or custom rules.
    return true;
}

void Route::form_response()
{
    is_valid_request();
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
