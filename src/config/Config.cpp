/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alraltse <alraltse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:12 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/05 14:48:33 by alraltse         ###   ########.fr       */
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
