/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apple <apple@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:30 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/05 23:04:26 by apple            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config/Route.hpp"

Route::Route(Location& loc) 
{
    route_name = loc.route_name;
    default_html = loc.default_html;
    allowed_methods = loc.allowed_methods;
    autoindex = loc.autoindex;
}

Route::~Route() {}

const string& Route::get_route_name() const
{
    return route_name;
}

const string& Route::get_default_html() const
{
    return default_html;
}
const vector<string>& Route::get_allowed_methods() const
{
    return allowed_methods;
}
const string& Route::get_autoindex() const
{
    return autoindex;
}
