*This project has been created as part of the 42 curriculum by [kikwasni](https://github.com/kingak4), [alraltse](https://github.com/alrltgit), [korzecho](https://github.com/Fistxszek)*

<p align="center">
  <img src="webserv.png" alt="Webserv Preview" width="230"/>
</p>


## 📌 Description

Webserv is a custom HTTP server written entirely in C++ 98 as part of the 42 curriculum.

The objective of this project is to build a fully functional, non-blocking web server from scratch, without using any external web server libraries. The server must handle multiple clients simultaneously using a single I/O multiplexing mechanism such as `poll()`, `select()`, `epoll()`, or `kqueue()`.

The program is executed as: ./webserv [configuration_file]

This server is designed to:

- Parse and apply a configuration file inspired by NGINX
- Handle multiple ports and multiple server blocks
- Serve static websites
- Support file uploads
- Implement at least the GET, POST, and DELETE HTTP methods
- Execute CGI scripts (e.g. PHP or Python)
- Provide accurate HTTP status codes
- Return default error pages when necessary
- Remain fully non-blocking at all times
- Properly handle client disconnections
- Manage multiple simultaneous connections through a single event loop

The architecture is event-driven and built around non-blocking sockets. All socket I/O operations are performed only after readiness is confirmed through a single multiplexing call. The server must never hang indefinitely and must remain resilient under stress conditions.

Special attention is given to:

- Correct HTTP request parsing
- Header validation
- Chunked transfer decoding
- CGI communication through environment variables
- Proper file descriptor management
- Memory safety and resource cleanup

The configuration file allows:

- Defining interface and port pairs
- Setting default error pages
- Limiting client body size
- Configuring routes (allowed methods, redirections, root directories)
- Enabling/disabling directory listing
- Defining default index files
- Enabling file uploads
- Configuring CGI execution based on file extension

This project demonstrates low-level network programming, event-driven architecture, protocol handling, and system-level resource management in C++ 98.

---

## 🛠️ Instructions
### Compilation

The project is written in **C++ 98**. Use the provided `Makefile` to compile:

```bash
make
```

Available rules:
- `make clean` – remove object files
- `make fclean` – remove object and executable files
- `make re` – recompile everything

### Execution

```bash
./webserv [path_to_configuration_file]
```

> If no configuration file is provided, the server will attempt to load `default.conf` from the project root.

### Testing

- **Web Browser:** `http://localhost:8080` (or port defined in config)
- **Curl:**
```bash
curl -v http://localhost:8080
```
- **Telnet:** manually simulate raw HTTP requests

---

## ⚙️ Configuration File

The configuration file defines server behavior and is inspired by NGINX. It allows hosting multiple virtual servers and defining rules per route.

### Configurable options:
- **Interfaces & Ports:** define IP addresses and ports
- **Client Limits:** `client_max_body_size` for uploads
- **Error Handling:** custom HTML pages for HTTP errors (404, 500, etc.)
- **Location Blocks:**
  - Allowed HTTP methods
  - Root & index files
  - Autoindex (directory listing)
  - HTTP redirects (301/302)
  - CGI script execution based on file extension
  - Upload directory

---


## 📚 Resources
### Documentation & Articles

- [Socket Programming in C++ (GeeksforGeeks)](https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/) — comprehensive guide on socket concepts and usages
- [Webserv — Building a Non-Blocking Web Server in C++ 98 (Medium)](https://m4nnb3ll.medium.com/webserv-building-a-non-blocking-web-server-in-c-98-a-42-project-04c7365e4ec7) — article oriented on the 42 Webserv project

### Video Tutorials

- [Non-Blocking Sockets & I/O Multiplexing in C/C++ (YouTube)](https://www.youtube.com/watch?v=V6ArZlHzZ6w) — tutorial explaining key concepts
- [HTTP Server Concepts (YouTube)](https://www.youtube.com/watch?v=9J1nJOivdyw) — video overview related to basic HTTP server mechanisms