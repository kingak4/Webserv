#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include <fstream>

class Config {
    private:
        std::string port;
        std::string client_max_body_size;
        std::string server_name;
        std::string host;
        std::string root;

    public:
        Config();
        ~Config();

        std::string trim_str(std::string temp_line);
        void read_config_file();
};

#endif
