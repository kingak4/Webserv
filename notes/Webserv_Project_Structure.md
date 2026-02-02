👤 Person A — Core Server & Network Layer
Main Responsibility:
Low-level server logic, networking, and non-blocking I/O.
Description:
Person A is responsible for the foundation of the server. This includes handling sockets, managing multiple clients, and implementing the event loop using poll() (or an equivalent function).
All communication with clients must be non-blocking and driven strictly by polling mechanisms, as required by the subject.

Main Tasks:
Create and configure listening sockets (socket, bind, listen)
Support multiple ports defined in the configuration file
Set all sockets to non-blocking mode
Implement the main event loop using poll() / select() / epoll()
Accept new client connections
Read data from clients only after readiness notification
Write responses only when sockets are writable
Handle client disconnections gracefully
Prevent hanging or infinite connections (timeouts)
Properly close and clean file descriptors
Secondary Tasks:
Client connection abstraction (Client class/struct)
Basic logging for connections and errors
Stability and stress testing

Why this role matters:
This layer is critical. Any blocking I/O or incorrect poll usage leads directly to project failure during evaluation.

👤 Person B — HTTP Protocol & Request/Response Handling
Main Responsibility:
HTTP request parsing and HTTP response generation.
Description:
Person B implements the HTTP protocol logic, ensuring that the server correctly understands client requests and sends valid, browser-compatible responses.

Main Tasks:
Parse raw HTTP requests
Extract method, path, headers, and body
Validate request correctness

Implement HTTP methods:
GET
POST
DELETE
Generate valid HTTP responses
Handle correct HTTP status codes
Serve default error pages
Enforce request body size limits
Secondary Tasks:
Content-Type detection
Chunked request body handling (unchunking)
Browser compatibility testing (Chrome, Firefox, etc.)

Why this role matters:
Browsers strictly follow the HTTP specification. A single malformed header or response can cause incorrect behavior or complete failure.

👤 Person C — Configuration, Routing & CGI
Main Responsibility:
Server configuration, routing logic, filesystem rules, and CGI execution.
Description:
Person C ensures that the server behaves dynamically based on the configuration file, similarly to NGINX.
This includes routing rules, file serving behavior, uploads, and CGI execution.

Main Tasks:
Parse the configuration file (NGINX-like syntax)
Store server and route configurations in structured objects
Match incoming URLs to route rules
Enforce allowed HTTP methods per route
Handle HTTP redirections (301 / 302)
Resolve root directories and index files
Enable or disable directory listing
Handle file uploads and storage paths
Execute CGI scripts:
fork and execve
environment variable setup
stdin/stdout handling via pipes
Secondary Tasks:
Error page resolution
Upload directory management
Correct working directory handling for CGI

Why this role matters:
Most advanced subject requirements (uploads, CGI, routing rules) are implemented here. Without this layer, the project cannot pass.

Project Folder Structure
webserv/
│
├── Makefile
├── README.md
│
├── config/
│   └── default.conf
│
├── include/
│   ├── core/
│   │   ├── Server.hpp
│   │   ├── Client.hpp
│   │   └── Poller.hpp
│   │
│   ├── http/
│   │   ├── HttpRequest.hpp
│   │   ├── HttpResponse.hpp
│   │   └── HttpParser.hpp
│   │
│   ├── config/
│   │   ├── Config.hpp
│   │   └── Route.hpp
│   │
│   └── cgi/
│       └── CgiHandler.hpp
│
├── src/
│   ├── core/
│   │   ├── Server.cpp
│   │   ├── Client.cpp
│   │   └── Poller.cpp
│   │
│   ├── http/
│   │   ├── HttpRequest.cpp
│   │   ├── HttpResponse.cpp
│   │   └── HttpParser.cpp
│   │
│   ├── config/
│   │   ├── Config.cpp
│   │   └── Route.cpp
│   │
│   ├── cgi/
│   │   └── CgiHandler.cpp
│   │
│   └── main.cpp
│
├── www/
│   ├── index.html
│   ├── 404.html
│   └── upload/
│
└── tests/
    └── requests/
    
Folder Ownership

Person A
include/core/
src/core/
main.cpp (shared integration)

Person B
include/http/
src/http/
tests/requests/

Person C
include/config/
src/config/
include/cgi/
src/cgi/
config/
www/

Final Notes
Each folder has a clear owner, but collaboration and code reviews are encouraged.
Any cross-module changes should be communicated before implementation.
This structure is designed to scale and to match evaluator expectations.
