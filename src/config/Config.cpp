/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:12 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/05 23:01:26 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config/Config.hpp"

Config::Config(ConfigParser config_parser) {
    server_name = config_parser.get_server_name();
    port = config_parser.get_port();
    host = config_parser.get_host();
    client_max_body_size = config_parser.get_client_max_body_size();
    root_dir = config_parser.get_root_dir();
    error_pages = config_parser.get_error_pages();
}

Config::~Config() {}

const string& Config::get_server_name() const {
    return server_name;
}

const string& Config::get_host() const {
    return host;
}

const string& Config::get_root_dir() const {
    return root_dir;
}

const int& Config::get_port() const {
    return port;
}

const int& Config::get_client_max_body_size() const {
    return client_max_body_size;
}

const map<int, string>& Config::get_error_pages() const {
    return error_pages;
}
