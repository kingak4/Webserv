/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alraltse <alraltse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:13:02 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/20 12:19:23 by alraltse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <dirent.h>
#include <map>
#include <vector>
#include <sys/stat.h>
#include "ConfigParser.hpp"
#include "Config.hpp"
#include "../http/Request.hpp"
#include "../cgi/CgiHandler.hpp"

using namespace std;

enum FsType
{
    FS_NOT_FOUND,
    FS_IS_FILE,
    FS_IS_DIR
};

class Route 
{
    private: 
        Config& server;
        Request& request;

        // location data
        string route_name;
        string url;
        vector<string> allowed_methods;
        string autoindex;

        string filesystem_path;

        // request data
        string request_method;
        string request_full_path;
        string request_path;
        string request_query;
        string request_version;
        map<string, string> request_headers;

    public:
        Route(Location& loc, Request& request, Config& server_block);
        ~Route();

        string retrieve_request_path(const string& request_full_path);
        string retrieve_request_query(const string& request_full_path);
        
        string find_abs_path(string file);
        string read_static_file(string filesystem_path);
        string error_response(string error_file);
        string serve_static_file();
        string handle_autoindex();
        string serve_directory_listing(string& filesystem_path);
        string handle_post();

        // VALIDATION CHECKS
        bool is_valid_request_path();
        bool is_allowed_method();
        bool is_cgi();
        FsType get_filesystem_type();
        string trim(const string& str);
        bool is_valid_request();
        string form_response();
        string handle_delete();

        // GETTERS
        const string& get_route_name() const;
        const string& get_url() const;
        const vector<string>& get_allowed_methods() const;
        const string& get_autoindex() const;
        const string& get_filesystem_path() const;
        const string& get_request_query() const;
        const string& get_request_path() const;
};

#endif
