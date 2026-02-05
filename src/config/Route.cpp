/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alraltse <alraltse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:30 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/05 16:49:47 by alraltse         ###   ########.fr       */
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
