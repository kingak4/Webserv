/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:57 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/10 20:04:31 by apple            ###   ########.fr       */
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

struct ServerData
{
    string server_name;
    int port;
    string host;
    int client_max_body_size;
    string root_dir;
    map<int, string> error_pages;

    vector<Location> locations;
};

class ConfigParser 
{
    private:
        vector<ServerData> config_servers;

    public:
        ConfigParser();
        ~ConfigParser();

        string trim_str(string temp_str);
        vector<string> parse_line(string line);
        void parse_server_block(string line, ServerData& server);
        void parse_route_block(string line, Location& loc);
        void parse_config_file(string& filename);
        string get_absolute_path_to_dict(string root);

        const vector<ServerData>& get_config_servers() const;
};

#endif
