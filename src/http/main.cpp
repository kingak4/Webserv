#include <iostream>
#include "http/Parser.hpp"

// mian to test parsing request line 

//int main()
//{
//    std::string good_request =
//        "GET /hello HTTP/1.1";

//    Parse good(good_request);
//    good.parseFirstLine();

//    std::cout << "=== GOOD REQUEST ===\n";
//    std::cout << "is_valid: " << good.isValid() << "\n";
//    std::cout << "error_code: " << good.getErrorCode() << "\n";
//    std::cout << "path: " << good.getPath() << "\n";
//    std::cout << "version: " << good.getVersion() << "\n\n";

//    std::string bad_request =
//        "GET /hello\r\n"
//        "Host: localhost\r\n"
//        "\r\n";

//    Parse bad(bad_request);
//    bad.parseFirstLine();

//    std::cout << "=== BAD REQUEST ===\n";
//    std::cout << "is_valid: " << bad.isValid() << "\n";
//    std::cout << "error_code: " << bad.getErrorCode() << "\n";
//}

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
#include "../../include/config/ConfigParser.hpp"

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
    // print_parsed_data(); // for testing
    return 0;
}
