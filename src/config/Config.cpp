/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alraltse <alraltse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:12 by alraltse          #+#    #+#             */
/*   Updated: 2026/03/05 15:58:20 by alraltse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config/Config.hpp"

Config::Config(ServerData servers, Request& request) {
    server_name = servers.server_name;
    port = servers.port;
    host = servers.host;
    client_max_body_size = servers.client_max_body_size;
    root_dir = servers.root_dir;
    error_pages = servers.error_pages;

    request_body = request.get_Body();
}

Config::~Config() {}

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

const size_t& Config::get_client_max_body_size() const {
    return client_max_body_size;
}

const map<int, string>& Config::get_error_pages() const {
    return error_pages;
}
