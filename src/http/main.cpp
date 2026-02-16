#include <iostream>
#include "http/Parser.hpp"
#include "http/Request.hpp"

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

//ultimate test for all parsing

// int main()
// {
//     // ===== TEST 1: Poprawny GET bez body =====
//     std::string req1 = "GET /hello HTTP/1.1\r\nHost: localhost\r\n\r\n";
//     Parser p1(req1);
//     p1.parse_Request();
//     std::cout << "=== TEST 1: GET bez body ===\n";
//     std::cout << "is_valid: " << p1.is_Valid() << "\n";
//     std::cout << "error_code: " << p1.get_Error_Code() << "\n";
//     std::cout << "body: \"" << p1.get_Body() << "\"\n\n";

//     // ===== TEST 2: Poprawny POST z Content-Length =====
//     std::string req2 = "POST /submit HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\n\r\nhello";
//     Parser p2(req2);
//     p2.parse_Request();
//     std::cout << "=== TEST 2: POST z poprawnym body ===\n";
//     std::cout << "is_valid: " << p2.is_Valid() << "\n";
//     std::cout << "error_code: " << p2.get_Error_Code() << "\n";
//     std::cout << "body: \"" << p2.get_Body() << "\"\n\n";

//     // ===== TEST 3: POST z body krótszym niż Content-Length =====
//     std::string req3 = "POST /submit HTTP/1.1\r\nHost: localhost\r\nContent-Length: 10\r\n\r\nshort";
//     Parser p3(req3);
//     p3.parse_Request();
//     std::cout << "=== TEST 3: body za krótkie ===\n";
//     std::cout << "is_valid: " << p3.is_Valid() << "\n";
//     std::cout << "error_code: " << p3.get_Error_Code() << "\n\n";

//     // ===== TEST 4: POST z body dłuższym niż Content-Length =====
//     std::string req4 = "POST /submit HTTP/1.1\r\nHost: localhost\r\nContent-Length: 3\r\n\r\ntoolong";
//     Parser p4(req4);
//     p4.parse_Request();
//     std::cout << "=== TEST 4: body za długie ===\n";
//     std::cout << "is_valid: " << p4.is_Valid() << "\n";
//     std::cout << "error_code: " << p4.get_Error_Code() << "\n\n";

//     // ===== TEST 5: Chunked body poprawny =====
//     std::string req5 =
//         "POST /upload HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n"
//         "5\r\nhello\r\n0\r\n\r\n";
//     Parser p5(req5);
//     p5.parse_Request();
//     std::cout << "=== TEST 5: poprawny chunked body ===\n";
//     std::cout << "is_valid: " << p5.is_Valid() << "\n";
//     std::cout << "error_code: " << p5.get_Error_Code() << "\n";
//     std::cout << "body: \"" << p5.get_Body() << "\"\n\n";

//     // ===== TEST 6: Chunked + Content-Length (błąd) =====
//     std::string req6 =
//         "POST /upload HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\nTransfer-Encoding: chunked\r\n\r\n"
//         "5\r\nhello\r\n0\r\n\r\n";
//     Parser p6(req6);
//     p6.parse_Request();
//     std::cout << "=== TEST 6: chunked + content-length (błąd) ===\n";
//     std::cout << "is_valid: " << p6.is_Valid() << "\n";
//     std::cout << "error_code: " << p6.get_Error_Code() << "\n\n";

//     // ===== TEST 7: Brak Host (błąd) =====
//     std::string req7 = "GET /hello HTTP/1.1\r\n\r\n";
//     Parser p7(req7);
//     p7.parse_Request();
//     std::cout << "=== TEST 7: brak Host ===\n";
//     std::cout << "is_valid: " << p7.is_Valid() << "\n";
//     std::cout << "error_code: " << p7.get_Error_Code() << "\n\n";

//     // ===== TEST 8: Niepoprawna metoda (błąd) =====
//     std::string req8 = "PUT /update HTTP/1.1\r\nHost: localhost\r\n\r\n";
//     Parser p8(req8);
//     p8.parse_Request();
//     std::cout << "=== TEST 8: niepoprawna metoda ===\n";
//     std::cout << "is_valid: " << p8.is_Valid() << "\n";
//     std::cout << "error_code: " << p8.get_Error_Code() << "\n\n";

//     // ===== TEST 9: Niepoprawna wersja HTTP (błąd) =====
//     std::string req9 = "GET /hello HTTP/2.0\r\nHost: localhost\r\n\r\n";
//     Parser p9(req9);
//     p9.parse_Request();
//     std::cout << "=== TEST 9: niepoprawna wersja ===\n";
//     std::cout << "is_valid: " << p9.is_Valid() << "\n";
//     std::cout << "error_code: " << p9.get_Error_Code() << "\n\n";

//     // ===== TEST 10: GET z Content-Length (body ignorowane) =====
//     std::string req10 = "GET /hello HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\n\r\nhello";
//     Parser p10(req10);
//     p10.parse_Request();
//     std::cout << "=== TEST 10: GET z Content-Length (body ignorowane) ===\n";
//     std::cout << "is_valid: " << p10.is_Valid() << "\n";
//     std::cout << "error_code: " << p10.get_Error_Code() << "\n";
//     std::cout << "body: \"" << p10.get_Body() << "\"\n\n";

//     return 0;
// }


// test for making request for Alina 

// int main()
// {
//     // ===== TEST 1: Poprawny GET =====
//     std::string req1 = "GET /hello HTTP/1.1\r\nHost: localhost\r\n\r\n";
//     Parser p1(req1);
//     p1.parse_Request();

//     Request r1;
//     r1.buildFromParser(p1);

//     std::cout << "=== REQUEST TEST 1: GET ===\n";
//     std::cout << "is_valid: " << r1.is_Valid() << "\n";
//     std::cout << "error_code: " << r1.get_Error_Code() << "\n";
//     std::cout << "method: " << r1.get_Method() << "\n";
//     std::cout << "path: " << r1.get_Path() << "\n";
//     std::cout << "version: " << r1.get_Version() << "\n";
//     std::cout << "body: \"" << r1.get_Body() << "\"\n\n";

//     // ===== TEST 2: Poprawny POST z Content-Length =====
//     std::string req2 =
//         "POST /submit HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\n\r\nhello";
//     Parser p2(req2);
//     p2.parse_Request();

//     Request r2;
//     r2.buildFromParser(p2);

//     std::cout << "=== REQUEST TEST 2: POST Content-Length ===\n";
//     std::cout << "is_valid: " << r2.is_Valid() << "\n";
//     std::cout << "error_code: " << r2.get_Error_Code() << "\n";
//     std::cout << "method: " << r2.get_Method() << "\n";
//     std::cout << "path: " << r2.get_Path() << "\n";
//     std::cout << "body: \"" << r2.get_Body() << "\"\n\n";

//     // ===== TEST 3: Poprawny POST chunked =====
//     std::string req3 =
//         "POST /upload HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n"
//         "5\r\nhello\r\n0\r\n\r\n";
//     Parser p3(req3);
//     p3.parse_Request();

//     Request r3;
//     r3.buildFromParser(p3);

//     std::cout << "=== REQUEST TEST 3: POST chunked ===\n";
//     std::cout << "is_valid: " << r3.is_Valid() << "\n";
//     std::cout << "error_code: " << r3.get_Error_Code() << "\n";
//     std::cout << "method: " << r3.get_Method() << "\n";
//     std::cout << "path: " << r3.get_Path() << "\n";
//     std::cout << "body: \"" << r3.get_Body() << "\"\n\n";

//     // ===== TEST 4: Błąd –  =====
//     std::string req4 = "GET /hello HTTP/1.1\r\n\r\n";
//     Parser p4(req4);
//     p4.parse_Request();

//     Request r4;
//     r4.buildFromParser(p4);

//     std::cout << "=== REQUEST TEST 4:   ===\n";
//     std::cout << "is_valid: " << r4.is_Valid() << "\n";
//     std::cout << "error_code: " << r4.get_Error_Code() << "\n\n";

//     // ===== TEST 5: Błąd – body too short =====
//     std::string req5 =
//         "POST /submit HTTP/1.1\r\nHost: localhost\r\nContent-Length: 10\r\n\r\nshort";
//     Parser p5(req5);
//     p5.parse_Request();

//     Request r5;
//     r5.buildFromParser(p5);

//     std::cout << "=== REQUEST TEST 5: body too short  ===\n";
//     std::cout << "is_valid: " << r5.is_Valid() << "\n";
//     std::cout << "error_code: " << r5.get_Error_Code() << "\n\n";

//     return 0;
// }

// helper
// void print_parsed_data(Config server_block, vector<Route> routes)
// {
//     cout << "server_name: " << server_block.get_server_name() << endl;
//     cout << "port: " << server_block.get_port() << endl;
//     cout << "host: " << server_block.get_host() << endl;
//     cout << "client_max_body_size: " << server_block.get_client_max_body_size() << endl;
//     cout << "root: " << server_block.get_root_dir() << endl;

//     map<int, string> error_pages = server_block.get_error_pages();

//     map<int, string>::const_iterator it;
//     for (it = error_pages.begin(); it != error_pages.end(); ++it)
//         cout << "error page: " <<  it->first << " " << it->second << endl;

//     cout << endl;

//     cout << "routes.size(): " << routes.size() << endl;
//     for (size_t i = 0; i < routes.size(); ++i)
//     {
//         cout << "route_name: " << routes[i].get_route_name() << endl;
//         cout << "url: " << routes[i].get_url() << endl;
//         for (size_t j = 0; j < routes[i].get_allowed_methods().size(); ++j)
//         {
//             cout << "method: " << routes[i].get_allowed_methods()[j] << endl;
//         }
//         cout << "autoindex: " << routes[i].get_autoindex() << endl;
//         cout << endl;
//     }
// }

Route* get_route_block(ServerData server, Request& request, Config server_block)
{
    vector<Location>& locations = server.locations;
    string path = request.get_Path();
    string route_path;
    size_t pos;

    if ((pos = path.find("?")) != string::npos)
        route_path = path.substr(0, pos);
    else
        route_path = path;

    for (size_t i = 0; i < locations.size(); ++i)
    {
        if (route_path == locations[i].route_name)
            return new Route(locations[i], request, server_block);
    }
    return NULL;
}

int find_requested_server(Request& request, ConfigParser& config_parser)
{
    string response;

    map<string, string> headers = request.get_Headers();
    vector<ServerData> servers = config_parser.get_config_servers();
    
    Config* server_block = NULL;
    Route* route_block = NULL;

    cout << headers["host"] << endl;

    // STEP 1: filter servers by port
    // vector<ServerData> candidates;
    // for (size_t i = 0; i < servers.size(); ++i)
    // {
    //     if (servers[i].port == client_port)
    //         candidates.push_back(servers[i]);
    // }

    // STEP 2: Try to find exact host match
    // for (size_t i = 0; i < candidates.size(); i++)
    // {
    //     if (candidates[i].server_name == headers["host"])
    //     {
    //         server_block = new Config(candidates[i], request);
    //         route_block = get_route_block(candidates[i], request, *server_block);
    //         break ;
    //     }
    // }

    for (size_t i = 0; i < servers.size(); i++)
    {
        if (servers[i].server_name == headers["host"])
        {
            server_block = new Config(servers[i], request);
            route_block = get_route_block(servers[i], request, *server_block);
            break ;
        }
    }

    // STEP 3: If no server that matches the headers["host"] name is found, cho0se the first server from the list
    // if (!server_block && !candidates.empty())
    // {
    //     server_block = new Config(candidates[0], request);
    //     route_block = get_route_block(candidates[0], request, *server_block);
    // }

    if (server_block && route_block)
    {
        response = route_block->form_response();
        if (response == "")
        {
            cout << "response is NULL" << endl;
        }
        cout << response << endl;

        delete server_block;
        delete route_block;
    }
    else
        return 1; // no server found
    
    return 0;
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
    else
    {
        cout << "Wrong number of argument" << endl;
        return 1;
    }

    // string req1 = "GET /index.html?user=Alice HTTP/1.1\r\nHost: super_webserv\r\n\r\n";
    // string req1 = "GET /uploads HTTP/1.1\r\nHost: super_webserv\r\n\r\n";
    string req1 = "GET /cgi/test.py HTTP/1.1\r\nHost: cgi_webserv\r\n\r\n";

    Parser parser(req1);
    parser.parse_Request();

    Request request;
    request.buildFromParser(parser);

    // GET ROUTE BLOCK AND LOCATION
    if (find_requested_server(request, config_parser) == 1)
    {
        cout << "Requested server or location is not found" << endl;
        return 0;
    }

    return 0;
}
