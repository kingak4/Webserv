#include "../../include/config/ConfigParser.hpp"
#include "../../include/config/Config.hpp"
#include "../../include/config/Route.hpp"

// helper
void print_parsed_data(Config server_block, vector<Route> routes)
{
    cout << "server_name: " << server_block.get_server_name() << endl;
    cout << "port: " << server_block.get_port() << endl;
    cout << "host: " << server_block.get_host() << endl;
    cout << "client_max_body_size: " << server_block.get_client_max_body_size() << endl;
    cout << "root: " << server_block.get_root_dir() << endl;

    map<int, string> error_pages = server_block.get_error_pages();

    map<int, string>::const_iterator it;
    for (it = error_pages.begin(); it != error_pages.end(); ++it)
        cout << "error page: " <<  it->first << " " << it->second << endl;

    cout << endl;

    cout << "routes.size(): " << routes.size() << endl;
    for (size_t i = 0; i < routes.size(); ++i)
    {
        cout << "route_name: " << routes[i].get_route_name() << endl;
        cout << "default_html: " << routes[i].get_default_html() << endl;
        for (size_t j = 0; j < routes[i].get_allowed_methods().size(); ++j)
        {
            cout << "method: " << routes[i].get_allowed_methods()[j] << endl;
        }
        cout << "autoindex: " << routes[i].get_autoindex() << endl;
        cout << endl;
    }
}

int main(int ac, char** av)
{
    ConfigParser config_parser;


    string filename = "config/default.conf";
    if (ac == 1)
        config_parser.parse_config_file(filename); // default config file
    else if (ac == 2)
    {
        string param(av[1]);
        config_parser.parse_config_file(param);
    }
    // else
        // trow exception

    Config server_block(config_parser); // get server block

    vector<Location> locations = config_parser.get_locations(); // get vectore of location blocks
    vector<Route> routes; // store each location into Route object and push all the Route objects into vactore<Route routes>
    for (size_t i  = 0; i < locations.size(); ++i) // iterate over vector<Route> routes
        routes.push_back(locations[i]);

    print_parsed_data(server_block, routes); // for testing

    return 0;
}