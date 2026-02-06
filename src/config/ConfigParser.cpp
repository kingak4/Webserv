/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alraltse <alraltse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:23 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/05 12:12:26 by alraltse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config/ConfigParser.hpp"

ConfigParser::ConfigParser() { parse_config_file(); }

ConfigParser::~ConfigParser() {}

void ConfigParser::parse_server_block(string line) 
{
    int nb;
    size_t pos;
    string temp_line;
    vector<string> lline;

    string server_keywords[6]  = {"server_name", "listen", "host", "client_max_body_size", "root", "error_page"};

    for (int i = 0; i < 6; i++)
    {
        if ((pos = line.find(server_keywords[i])) != string::npos)
        {
            temp_line = trim_str(line.substr(pos + server_keywords[i].length()));
        
            if (server_keywords[i] == "server_name")
                server_name = temp_line;
            else if (server_keywords[i] == "listen")
            {
                stringstream ss(temp_line);
                ss >> port;
            }
            else if (server_keywords[i] == "host")
                host = temp_line;
            else if (server_keywords[i] == "client_max_body_size")
            {
                stringstream ss(temp_line);
                ss >> client_max_body_size;
            }
            else if (server_keywords[i] == "root")
                root_dir = temp_line;
            else if (server_keywords[i] == "error_page")
            {
                lline = parse_line(line);
                stringstream ss(lline[1]);
                ss >> nb;
                error_pages.insert(make_pair(nb, lline[2]));
            }
        }
    }
}

vector<string> ConfigParser::parse_line(string line)
{
    vector<string> lline;
    istringstream iss(line);
    string word;
    
    while (iss >> word)
        lline.push_back(word);
    
    return lline;
}

void ConfigParser::parse_route_block(string line, Location& loc)
{
    size_t pos;
    string temp_str;

    if ((pos = line.find("location")) != string::npos)
        loc.route_name = trim_str(line.substr(pos + string("location").length()));
    else if ((pos = line.find("default")) != string::npos)
        loc.default_html = trim_str(line.substr(pos + string("default").length()));
    else if ((pos = line.find("allowed_methods")) != string::npos)
        loc.allowed_methods.push_back(trim_str(line.substr(pos + string("allowed_methods").length()))); 
    else if ((pos = line.find("autoindex")) != string::npos)
        loc.autoindex = trim_str(line.substr(pos + string("autoindex").length()));
}

void ConfigParser::parse_config_file()
{
    string filename = "config/default.conf";
    string line;
    Location loc;
    bool in_location;

    locations.clear();
    ifstream config(filename.c_str());
    if (!config.is_open()) // handle inside try/catch ?
    {
        cout << "Error while opening the file" << endl;
        return ;
    }

    in_location = false;
    while (getline(config, line))
    {
        if (line.empty() || line[0] == '#')
            continue ;
        else if (line.find("location") != string::npos)
        {
            loc = Location();
            in_location = true;
            parse_route_block(line, loc);
        }
        else if (line.find("}") != string::npos && in_location)
        {
            locations.push_back(loc);
            in_location = false;
        }
        else if (in_location)
        {
            parse_route_block(line, loc);
        }
        else
            parse_server_block(line);
    }
    config.close();
}

string ConfigParser::trim_str(string temp_str)
{
    size_t start_idx;
    size_t end_idx;

    start_idx = temp_str.find_first_not_of(" \t\n\r");
    end_idx = temp_str.find_last_not_of(" {;\t\n\r");

    return temp_str.substr(start_idx, end_idx - start_idx + 1);    
}

const string& ConfigParser::get_server_name() const {
    return server_name;
}

const string& ConfigParser::get_host() const {
    return host;
}

const string& ConfigParser::get_root_dir() const {
    return root_dir;
}

const int& ConfigParser::get_port() const {
    return port;
}

const int& ConfigParser::get_client_max_body_size() const {
    return client_max_body_size;
}

const map<int, string>& ConfigParser::get_error_pages() const {
    return error_pages;
}

const vector<Location>& ConfigParser::get_locations() const {
    return locations;
}
