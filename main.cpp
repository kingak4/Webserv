#include "./include/config/ConfigParser.hpp"

// helper
void print_parsed_data()
{
    ConfigParser test;

    cout << "server_name: " << test.get_server_name() << endl;
    cout << "port: " << test.get_port() << endl;
    cout << "host: " << test.get_host() << endl;
    cout << "client_max_body_size: " << test.get_client_max_body_size() << endl;
    cout << "root: " << test.get_root_dir() << endl;

    map<int, string> error_pages = test.get_error_pages();
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
    print_parsed_data(); // for testing
    return 0;
}
