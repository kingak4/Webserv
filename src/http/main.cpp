#include <iostream>
#include "http/Parser.hpp"

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