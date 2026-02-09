/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:30 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/09 17:44:50 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config/Route.hpp"

Route::Route() {};

Route::Route(Location& loc, Parser request) 
{
    (void)request;
    route_name = loc.route_name;
    url = loc.url;
    allowed_methods = loc.allowed_methods;
    autoindex = loc.autoindex;

    // request_method = request.get_Method();
    // request_path = request.get_Path();
    // request_version = request.get_Version();
    // request_headers = request.get_Headers();
    request_full_path = "/uploads?something=1";

    request_path = retrieve_request_path(request_full_path);
    request_query = retrieve_request_query(request_full_path);

    filesystem_path = server.get_root_dir() + request_path
    // TEST
    cout << "request_path: " << request_path << endl;
    cout << "request_query: " << request_query << endl;
    cout << "filesystem_path: " << filesystem_path << endl;
    
    request_method = "GET";
    request_version = "HTTP/1.1";
    
}

Route::~Route() {}

string Route::retrieve_request_path(string request_full_path)
{
    if ((pos = request_full_path.find('?')) != string::npos)
        return request_full_path.substr(0, pos);
}

string Route::retrieve_request_query(string request_full_path)
{
    int len;
    
    len = request_full_path.length();
    if ((pos = request_full_path.find('?')) != string::npos)
        return request_full_path.substr(pos, len)
}

bool Route::is_valid_request_path()
{
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



FsType Route::get_filesustem_type()
{
    struct stat buffer; // stat holds metadata about file
    bool res;
    
    if (stat(filesystem_path.c_str(), &buffer) != 0) // buffer.st_mode contains data about file type and permissions;
        return FS_NOT_FOUND;
    
    if (S_ISREG(buffer.st_mode))
        return FS_IS_FILE;

    if (S_ISDIR(buffer.st_mode))
        return FS_IS_DIR;
        
    return FS_NOT_FOUND;
}

bool Route::is_valid_request()
{
    bool request_path_is_valid;
    bool method_is_allowed;
    bool url_is_cgi;
    FsType filesystem_status;
    bool body_matches_size;

    // 1. SPLIT REQUEST_FULL_PATH INTO REQUEST_PATH AND REQUEST_QUERY
    
    // 2. COMPARE REQUEST_PATH AGAINS LOCATION_URL
    request_path_is_valid = is_valid_request_path();
    // if (!request_path_is_valid)
    // send error response

    // 3. CHECK ALLOWED METHODS:
    method_is_allowed = is_allowed_method();
    // if (!method_is_allowed)
    // send response with 405 ERROR

    // 4. BUILD THE FILESYSTEM PATH
    
    
    // IS THE REQUEST TARGET CGI:
    is_cgi();
    // if (is_cgi)
    // move to CgiHandler class
    
    // FOR STATIC FILES: DOES THE FILE EXIST:
    filesystem_status = get_filesystem_type();
    switch(filesystem_status)
    {
        case FS_NOT_FOUND:
            // 404 ERROR
        case FS_IS_FILE:
            if (is_cgi())
                // got to CgiHndler
            else
                // serve static file
        case FS_IS_DIR
            // handle autoindex
    }

    // IF THE REQUEST PATH POINTS TO A DICTIONARY, CHECK IF AUTOINDEX IS ENABLES IN THE CONFIG
    

    // IF THERE'S A BODY, VERIFY ITS SIZE AGAINST CLIENT_MAX_BODY_SIZE:
    body_matches_size = server.does_body_match_size();
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
