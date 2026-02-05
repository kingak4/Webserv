/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alraltse <alraltse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:52 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/05 16:54:58 by alraltse         ###   ########.fr       */
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

        

};

#endif
