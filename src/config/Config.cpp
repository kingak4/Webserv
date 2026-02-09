/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:12 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/09 16:16:50 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config/Config.hpp"

Config::Config(ConfigParser config_parser, Parser request) {
    server_name = config_parser.get_server_name();
    port = config_parser.get_port();
    host = config_parser.get_host();
    client_max_body_size = config_parser.get_client_max_body_size();
    root_dir = config_parser.get_root_dir();
    error_pages = config_parser.get_error_pages();

    // REQUEST DATA
    // request_body = request.get_Body();
    request_body = "name=John&age=42"
}

Config::~Config() {}

// IF THERE'S A BODY, VERIFY ITS SIZE AGAINST CLIENT_MAX_BODY_SIZE:
bool Config::does_body_match_size()
{
    return request_body.length() <= client_max_body_size;
}

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
