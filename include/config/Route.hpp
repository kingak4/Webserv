/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alraltse <alraltse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:13:02 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/06 13:33:58 by alraltse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "ConfigParser.hpp"
#include "../http/Parser.hpp"

using namespace std;

class Route 
{
    string route_name;
    string default_html;
    vector<string> allowed_methods;
    string autoindex;

    public:
        Route();
        Route(Location& loc);
        ~Route();

        bool is_valid_request(Parser request);

        const string& get_route_name() const;
        const string& get_default_html() const;
        const vector<string>& get_allowed_methods() const;
        const string& get_autoindex() const;
};

#endif
