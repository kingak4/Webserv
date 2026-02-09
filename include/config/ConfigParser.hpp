/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:57 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/09 12:49:24 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cstdlib>
#include <limits.h>

using namespace std;

struct Location
{
    string route_name;
    string url;
    vector<string> allowed_methods;
    string autoindex;
};

class ConfigParser 
{
    private:
        string server_name;
        int port;
        string host;
        int client_max_body_size;
        string root_dir;
        map<int, string> error_pages;
        vector<Location> locations;

    public:
        ConfigParser();
        ~ConfigParser();

        string trim_str(string temp_line);
        vector<string> parse_line(string line);
        void parse_server_block(string line);
        void parse_route_block(string line, Location& loc);
        void parse_config_file(string& filename);
        string get_absolute_path_to_dict(string root);

        const string& get_server_name() const;
        const string& get_host() const;
        const string& get_root_dir() const;
        const int& get_port() const;
        const int& get_client_max_body_size() const;
        const map<int, string>& get_error_pages() const;
        const vector<Location>& get_locations() const;
};

#endif
