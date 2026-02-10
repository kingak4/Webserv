/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:23 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/10 20:07:37 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config/ConfigParser.hpp"

ConfigParser::ConfigParser() {}

ConfigParser::~ConfigParser() {}

void ConfigParser::parse_server_block(string line, ServerData& server) 
{
    int nb;
    size_t pos;
    string temp_line;
    vector<string> lline;

    string server_keywords[6]  = {
        "server_name", 
        "listen", 
        "host", 
        "client_max_body_size", 
        "root", 
        "error_page"
    };

    for (int i = 0; i < 6; i++)
    {
        if ((pos = line.find(server_keywords[i])) != string::npos)
        {
            temp_line = trim_str(line.substr(pos + server_keywords[i].length()));
        
            if (server_keywords[i] == "server_name")
                server.server_name = temp_line;
            else if (server_keywords[i] == "listen")
            {
                stringstream ss(temp_line);
                ss >> server.port;
            }
            else if (server_keywords[i] == "host")
                server.host = temp_line;
            else if (server_keywords[i] == "client_max_body_size")
            {
                stringstream ss(temp_line);
                ss >> server.client_max_body_size;
            }
            else if (server_keywords[i] == "root")
            {
                server.root_dir = get_absolute_path_to_dict(temp_line);
                // if (!server.root_dir)
                //     throw exception
            }
                
            else if (server_keywords[i] == "error_page")
            {
                lline = parse_line(line);
                stringstream ss(lline[1]);
                ss >> nb;
                server.error_pages.insert(make_pair(nb, lline[2]));
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
        loc.url = trim_str(line.substr(pos + string("default").length()));
    else if ((pos = line.find("cgi")) != string::npos)
        loc.url = trim_str(line.substr(pos + string("cgi").length()));
    else if ((pos = line.find("allowed_methods")) != string::npos)
        loc.allowed_methods.push_back(trim_str(line.substr(pos + string("allowed_methods").length())));
    else if ((pos = line.find("autoindex")) != string::npos)
        loc.autoindex = trim_str(line.substr(pos + string("autoindex").length())); 
}

void ConfigParser::parse_config_file(string& filename)
{
    ifstream config(filename.c_str());
    if (!config.is_open()) // handle inside try/catch ?
    {
        cout << "Error while opening the file" << endl;
        return ;
    }

    string line;
    bool in_server;
    bool in_location;

    ServerData current_server;
    Location current_location;

    in_server = false;
    in_location = false;

    config_servers.clear();

    while (getline(config, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;   
        }
        if (line.find("server") != string::npos && line.find("{") != string::npos)
        {
            current_server = ServerData();
            in_server = true;
            continue;
        }

        if (line.find("location") != string::npos)
        {
            current_location = Location();
            in_location = true;
            parse_route_block(line, current_location);
            continue;
        }

        // END LOCATION BLOCK
        if (line.find("}") != string::npos && in_location)
        {
            current_server.locations.push_back(current_location);
            in_location = false;
            continue;
        }

        // END SERVER BLOCK
        if (line.find("}") != string::npos && in_server)
        {
            config_servers.push_back(current_server);
            in_server = false;
            continue;
        }

        if (in_location)
        {
            parse_route_block(line, current_location);
            continue;
        }

        if (in_server)
        {
            parse_server_block(line, current_server);
            continue;
        }
    }

    config.close();
}

// string ConfigParser::trim_str(const string& temp_str)
// {
//     if (temp_str.empty())
//         return "";

//     size_t start_idx = temp_str.find_first_not_of(" \t\n\r");
//     if (start_idx == string::npos)
//         return ""; // entire string is whitespace

//     size_t end_idx = temp_str.find_last_not_of(" {;\t\n\r");
//     if (end_idx == string::npos || end_idx < start_idx)
//         return ""; // nothing valid

//     return temp_str.substr(start_idx, end_idx - start_idx + 1);
// }


string ConfigParser::trim_str(string temp_str)
{
    size_t start_idx;
    size_t end_idx;

    start_idx = temp_str.find_first_not_of(" \t\n\r");
    end_idx = temp_str.find_last_not_of(" {;\t\n\r");

    return temp_str.substr(start_idx, end_idx - start_idx + 1);    
}

// TODO: CHECK IF ROOT IS DIRECTORY FIRST
string ConfigParser::get_absolute_path_to_dict(string root) {
    char abs_path[PATH_MAX];

    if (realpath(root.c_str(), abs_path) != NULL)
    {
        string root_path(abs_path);
        return root_path;
    }
    cout << "path to the root not found" << endl;
    return NULL;
}

const vector<ServerData>& ConfigParser::get_config_servers() const
{
    return config_servers;
}
