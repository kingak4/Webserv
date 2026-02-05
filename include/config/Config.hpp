/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:52 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/05 22:59:59 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "ConfigParser.hpp"

using namespace std;

class Config {
    string server_name;
    int port;
    string host;
    int client_max_body_size;
    string root_dir;
    map<int, string> error_pages;

    public:
        Config(ConfigParser config_parser);
        ~Config();

        const string& get_server_name() const;
        const string& get_host() const;
        const string& get_root_dir() const;
        const int& get_port() const;
        const int& get_client_max_body_size() const;
        const map<int, string>& get_error_pages() const;

};

#endif
