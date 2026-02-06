/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alraltse <alraltse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:30 by alraltse          #+#    #+#             */
/*   Updated: 2026/02/06 14:52:42 by alraltse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config/Route.hpp"

Route::Route() {};

Route::Route(Location& loc) 
{
    route_name = loc.route_name;
    default_html = loc.default_html;
    allowed_methods = loc.allowed_methods;
    autoindex = loc.autoindex;
}

Route::~Route() {}

bool Route::is_valid_request(Parser request)
{
    // CHECKS:
    // HTTP METHOD ALLOWED:
    // - check the request's method(GET, POST, DELETE) against the allowed methods of the route
    // - if not allowed - ERROR 405
    // URL / PATH MATCHING: 
    // the request path should match the route rule
    // - exact match for static files
    // - prefix match for directories
    // ROUTE DIRECTORY / FILE LOCATION
    // - route's root + request path to actual file path
    // - if doesn't work - ERROR 404
    // REQUEST BODY SIZE:
    // - if the request has a body, verify its size against client_max_body_size in the config
    // AUTOINDEX:
    // - if the request path points to a dictionary, check if autoindex is enabled in the config.
    // - respond with 403 or 404 or generate a directory listing?
    // CGI / SCRIPT HANDLING :
    // - : If the request points to a file that should be handled by CGI (like .php, .py, etc.), verify the route’s CGI rules.
    // Headers
    // - Host header → must match server_name.
    // - Required headers for CGI or special routes.
    // - Optional: check for forbidden headers or custom rules.
}

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
