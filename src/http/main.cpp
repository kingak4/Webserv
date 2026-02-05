#include "../../include/config/ConfigParser.hpp"
#include "../../include/config/Config.hpp"
#include "../../include/config/Route.hpp"

// helper
void print_parsed_data(Config server_block, Route loc)
{
    cout << "server_name: " << server_block.get_server_name() << endl;
    cout << "port: " << server_block.get_port() << endl;
    cout << "host: " << server_block.get_host() << endl;
    cout << "client_max_body_size: " << server_block.get_client_max_body_size() << endl;
    cout << "root: " << server_block.get_root_dir() << endl;

    map<int, string> error_pages = server_block.get_error_pages();

    map<int, string>::const_iterator it;
    for (it = error_pages.begin(); it != error_pages.end(); ++it)
    {
        cout << "error page: " <<  it->first << " " << it->second << endl;
    }

    cout << endl;

    vector<Location> locations = test.get_locations();
    cout << "locations.size(): " << locations.size() << endl;
    for (size_t i = 0; i < locations.size(); ++i)
    {
        cout << "route_name: " << locations[i].route_name << endl;
        cout << "default_html: " << locations[i].default_html << endl;
        for (size_t j = 0; j < locations[i].allowed_methods.size(); ++j)
        {
            cout << "method: " << locations[i].allowed_methods[j] << endl;
        }
        cout << "autoindex: " << locations[i].autoindex << endl;
        cout << endl;
    }
}

int main()
{
    ConfigParser config_parser;

    string filename = "config/default.conf";
    config_parser.parse_config_file(filename); // default config file

    Config server_block(config_parser);

    vector<Location> loc = config_parser.get_locations();
    Route loc(loc[0]);
    Route loc(loc[1]);

    print_parsed_data(server_block, loc); // for testing

    return 0;
}