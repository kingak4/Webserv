#include <iostream>
#include "http/Parser.hpp"
#include "../../include/config/ConfigParser.hpp"
#include "../../include/config/Config.hpp"
#include "../../include/config/Route.hpp"
#include "../../include/cgi/CgiHandler.hpp"

// mian to test parsing request line 

// int main()
// {
//    std::string good_request =
//        "GET /hello HTTP/1.1";

//    Parser good(good_request);
//    good.parse_First_Line();

//    std::cout << "=== GOOD REQUEST ===\n";
//    std::cout << "is_valid: " << good.is_Valid() << "\n";
//    std::cout << "error_code: " << good.get_Error_Code() << "\n";
//    std::cout << "path: " << good.get_Path() << "\n";
//    std::cout << "version: " << good.get_Version() << "\n\n";

//    std::string bad_request =
//        "GET /hello\r\n"
//        "Host: localhost\r\n"
//        "\r\n";

//    Parse bad(bad_request);
//    bad.parseFirstLine();

//    std::cout << "=== BAD REQUEST ===\n";
//    std::cout << "is_valid: " << bad.isValid() << "\n";
//    std::cout << "error_code: " << bad.getErrorCode() << "\n";
// }

// mian to test parsing header line 

// int main()
// {
//     // ===== TEST 1: Poprawna pierwsza linia bez nagłówków =====
//     std::string request1 =
//         "GET /hello HTTP/1.1\r\n\r\n";

//     Parser p1(request1);
//     p1.parse_First_Line();
//     p1.parse_Headers();

//     std::cout << "=== TEST 1 ===\n";
//     std::cout << "is_valid: " << p1.is_Valid() << "\n";
//     std::cout << "error_code: " << p1.get_Error_Code() << "\n";
//     std::cout << "method: " << p1.get_Method() << "\n";
//     std::cout << "path: " << p1.get_Path() << "\n";
//     std::cout << "version: " << p1.get_Version() << "\n";
//     std::cout << "headers size: " << p1.get_Headers().size() << "\n\n";

//     // ===== TEST 2: Poprawna pierwsza linia + poprawny nagłówek =====
//     std::string request2 =
//         "GET /hello HTTP/1.1\r\n"
//         "Host: localhost\r\n"
//         "\r\n";

//     Parser p2(request2);
//     p2.parse_First_Line();
//     p2.parse_Headers();

//     std::cout << "=== TEST 2 ===\n";
//     std::cout << "is_valid: " << p2.is_Valid() << "\n";
//     std::cout << "error_code: " << p2.get_Error_Code() << "\n";
//     std::cout << "headers[host]: " << p2.get_Headers()["host"] << "\n\n";

//     // ===== TEST 3: Błędny nagłówek (pusty key) =====
//     std::string request3 =
//         "GET /hello HTTP/1.1\r\n"
//         ": localhost\r\n"
//         "\r\n";

//     Parser p3(request3);
//     p3.parse_First_Line();
//     p3.parse_Headers();

//     std::cout << "=== TEST 3 ===\n";
//     std::cout << "is_valid: " << p3.is_Valid() << "\n";
//     std::cout << "error_code: " << p3.get_Error_Code() << "\n\n";

//     // ===== TEST 4: Brak nagłówka Host =====
//     std::string request4 =
//         "GET /hello HTTP/1.1\r\n"
//         "Content-Length: 5\r\n"
//         "\r\n";

//     Parser p4(request4);
//     p4.parse_First_Line();
//     p4.parse_Headers();

//     std::cout << "=== TEST 4 ===\n";
//     std::cout << "is_valid: " << p4.is_Valid() << "\n";
//     std::cout << "error_code: " << p4.get_Error_Code() << "\n";
// }

// body tests 

// int main()
// {
//     // ===== TEST 1: Brak body, brak Content-Length =====
//     std::string request1 =
//         "GET /hello HTTP/1.1\r\n"
//         "Host: localhost\r\n"
//         "\r\n";

//     Parser p1(request1);
//     p1.parse_First_Line();
//     p1.parse_Headers();
//     p1.parse_Body();

//     std::cout << "=== BODY TEST 1: brak body ===\n";
//     std::cout << "is_valid: " << p1.is_Valid() << "\n";
//     std::cout << "error_code: " << p1.get_Error_Code() << "\n";
//     std::cout << "body: \"" << p1.get_Body() << "\"\n";
//     std::cout << "body length: " << p1.get_Body().size() << "\n\n";

//     // ===== TEST 2: Body zgodne z Content-Length =====
//     std::string request2 =
//         "POST /submit HTTP/1.1\r\n"
//         "Host: localhost\r\n"
//         "Content-Length: 5\r\n"
//         "\r\n"
//         "hello";

//     Parser p2(request2);
//     p2.parse_First_Line();
//     p2.parse_Headers();
//     p2.parse_Body();

//     std::cout << "=== BODY TEST 2: poprawne body ===\n";
//     std::cout << "is_valid: " << p2.is_Valid() << "\n";
//     std::cout << "error_code: " << p2.get_Error_Code() << "\n";
//     std::cout << "body: \"" << p2.get_Body() << "\"\n";
//     std::cout << "body length: " << p2.get_Body().size() << "\n\n";

//     // ===== TEST 3: Body krótsze niż Content-Length =====
//     std::string request3 =
//         "POST /submit HTTP/1.1\r\n"
//         "Host: localhost\r\n"
//         "Content-Length: 10\r\n"
//         "\r\n"
//         "short";

//     Parser p3(request3);
//     p3.parse_First_Line();
//     p3.parse_Headers();
//     p3.parse_Body();

//     std::cout << "=== BODY TEST 3: body za krótkie ===\n";
//     std::cout << "is_valid: " << p3.is_Valid() << "\n";
//     std::cout << "error_code: " << p3.get_Error_Code() << "\n";
//     std::cout << "body: \"" << p3.get_Body() << "\"\n";
//     std::cout << "body length: " << p3.get_Body().size() << "\n\n";

//     // ===== TEST 4: Body dłuższe niż Content-Length =====
//     std::string request4 =
//         "POST /submit HTTP/1.1\r\n"
//         "Host: localhost\r\n"
//         "Content-Length: 3\r\n"
//         "\r\n"
//         "toolong";

//     Parser p4(request4);
//     p4.parse_First_Line();
//     p4.parse_Headers();
//     p4.parse_Body();

//     std::cout << "=== BODY TEST 4: body za długie ===\n";
//     std::cout << "is_valid: " << p4.is_Valid() << "\n";
//     std::cout << "error_code: " << p4.get_Error_Code() << "\n";
//     std::cout << "body: \"" << p4.get_Body() << "\"\n";
//     std::cout << "body length: " << p4.get_Body().size() << "\n\n";

//     return 0;
// }

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
        cout << "url: " << routes[i].get_url() << endl;
        for (size_t j = 0; j < routes[i].get_allowed_methods().size(); ++j)
        {
            cout << "method: " << routes[i].get_allowed_methods()[j] << endl;
        }
        cout << "autoindex: " << routes[i].get_autoindex() << endl;
        cout << endl;
    }
}

Route get_route_block(ConfigParser config_parser, Parser request)
{
    Route res;
    vector<Location> locations = config_parser.get_locations();

    string route_path = request.get_Path();

    // WHEN I GET A ROUTE FROM PARSER, I NEED TO VALIDATE DATA FROM THIS REQUEST WITH CONFIG FILE RULES FOR THIS ROUTE
    for (size_t i = 0; i < locations.size(); ++i)
    {
        if (route_path.find(locations[i].route_name) == 0)
            res = Route(locations[i], request);
    }
    return res;
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

    Parser request;
    // GET ROUTE BLOCK
    Config server_block(config_parser, request);

    // // GET ROUTE BLOCK
    Route route_block = get_route_block(config_parser, request);
    // CgiHandler cgi;
    // cgi.is_cgi(route_block);



    // FOR TESTING
    // vector<Location> locations =  config_parser.get_locations();
    // vector<Route> routes; // store each location into Route object and push all the Route objects into vactore<Route routes>
    // for (size_t i = 0; i < locations.size(); ++i) // iterate over vector<Route> routes
    // {
    //     routes.push_back(Route(locations[i], request));
    // }
    // print_parsed_data(server_block, routes);

    return 0;
}
