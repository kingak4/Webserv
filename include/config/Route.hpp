/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:13:02 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/11 15:14:55 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "ConfigParser.hpp"
#include "Config.hpp"
#include "../http/Parser.hpp"
#include <sys/stat.h>

using namespace std;

enum FsType
{
    FS_NOT_FOUND,
    FS_IS_FILE,
    FS_IS_DIR
};

class Route 
{
    Config &server;

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
        Route(Config& server_block);
        Route(Location& loc, Parser request, Config& server_block);
        ~Route();

        string retrieve_request_path(const string& request_full_path);
        string retrieve_request_query(const string& request_full_path);
        
        void serve_static_file();
        void handle_autoindex();
        void serve_directory_listing(string& filesystem_path);

        // VALIDATION CHECKS
        bool is_valid_request_path();
        bool is_allowed_method();
        bool is_cgi();
        FsType get_filesystem_type();
        string trim(const string& str);
        bool is_valid_request();
        void form_response();

        // GETTERS
        const string& get_route_name() const;
        const string& get_url() const;
        const vector<string>& get_allowed_methods() const;
        const string& get_autoindex() const;
};

#endif
