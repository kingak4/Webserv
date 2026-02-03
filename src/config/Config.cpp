#include "../../include/config/Config.hpp"
#include <fstream>

Config::Config() { read_config_file(); }

Config::~Config() {}

std::string Config::trim_str(std::string temp_str)
{
    size_t start_idx;
    size_t end_idx;

    start_idx = temp_str.find_first_not_of(" \t\n\r");
    end_idx = temp_str.find_last_not_of(" ;\t\n\r");

    return temp_str.substr(start_idx, end_idx - start_idx + 1);
}

void Config::read_config_file()
{
    std::string filename = "config/default.conf";
    std::string line;
    size_t pos;
    std::string temp_line;
    std::string keywords[5]  = {"server_name", "listen", "host", "client_max_body_size", "root"};
    std::string* attributes[5] = {&server_name, &port, &host, &client_max_body_size, &root};

    std::ifstream config(filename.c_str());
    if (!config.is_open()) // handle inside try/catch ?
    {
        std::cout << "Error while opening the file";
        return ;
    }

    while (getline(config, line))
    {
        for (int i = 0; i < 5; i++)
        {
            if ((pos = line.find(keywords[i])) != std::string::npos)
            {
                temp_line = line.substr(pos + keywords[i].length());
                *(attributes[i]) = trim_str(temp_line);
            }
        }
    }
    // for testing
    // std::cout << "server_name: " << server_name << std::endl;
    // std::cout << "port: " << port << std::endl;
    // std::cout << "host: " << host << std::endl;
    // std::cout << "client_max_body_size: " << client_max_body_size << std::endl;
    // std::cout << "root: " << root << std::endl;
    config.close();
}
