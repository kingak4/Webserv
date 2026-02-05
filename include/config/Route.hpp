/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alraltse <alraltse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:13:02 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/05 16:50:08 by alraltse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "ConfigParser.hpp"

using namespace std;

class Route 
{
    string route_name;
    string default_html;
    vector<string> allowed_methods;
    string autoindex;

    public:
        Route(Location& loc);
        ~Route();
};

#endif
