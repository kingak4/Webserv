/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alraltse <alraltse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:12:30 by alraltse          #+#    #+#             */
/*   Updated: 2026/03/03 12:03:29 by alraltse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config/Route.hpp"

Route::Route(Location& loc, Request& request, Config& server_block) : server(server_block), request(request)
{
    route_name = loc.route_name;
    url = loc.url;
    
    vector<string> method = loc.allowed_methods;
    allowed_methods = loc.allowed_methods;
    autoindex = loc.autoindex;
    root = loc.root;
	redir_code = loc.redir_code;

    request_method = request.get_Method();
    request_full_path = request.get_Path();
    request_version = request.get_Version();
    request_headers = request.get_Headers();

    request_path = trim(retrieve_request_path(request_full_path));
    request_query = retrieve_request_query(request_full_path);

    cout << "request_path: " << request_path << endl; 
    filesystem_path = server.get_root_dir() + request_path;
}

Route::~Route() {}

string Route::retrieve_request_path(const string& request_full_path)
{
    size_t pos;
    string res;

    if ((pos = request_full_path.find('?')) != string::npos)
        res = request_full_path.substr(0, pos);
    else
        res = request_full_path;

    if (res.empty())
        res = "/";

    return res;
}


string Route::retrieve_request_query(const string& request_full_path)
{
    int len;
    size_t pos;
    string res;
    
    len = request_full_path.length();
    if ((pos = request_full_path.find('?')) != string::npos)
        res = request_full_path.substr(pos + 1, len);
    else
        res = "";
    return res;
}

bool Route::is_valid_request_path()
{
    cout << "url: " << url << endl;
    return request_path.find(url) == 0;
}

bool Route::is_allowed_method()
{
    for (size_t i = 0; i < allowed_methods.size(); i++)
    {
        if (allowed_methods[i] == request_method)
            return true;       
    }
    return false;
}

bool Route::is_cgi()
{
    if (request_path.find("/cgi-bin") == 0 or url.find(".py") == 0)
        return true;
    return false;
}

FsType Route::get_filesystem_type()
{
    struct stat buffer;

    if (stat(filesystem_path.c_str(), &buffer) != 0)
        return FS_NOT_FOUND;
    
    if (S_ISREG(buffer.st_mode))
        return FS_IS_FILE;

    if (S_ISDIR(buffer.st_mode))
        return FS_IS_DIR;
        
    return FS_NOT_FOUND;
}

string Route::trim(const string& str)
{
    size_t start;
    size_t end;
    
    if (str.empty())
        return "";

    start = 0;
    end = str.size() - 1;

    while (start <= end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n'))
        ++start;

    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n'))
        --end;

    return str.substr(start, end - start + 1);
}

string Route::serve_directory_listing(string& filesystem_path)
{
    if (root != "")
        filesystem_path = root + request_path;

    DIR *dir = opendir(filesystem_path.c_str());
    if (!dir)
        return error_response("500");

    ostringstream body;
    body << "<html>\n";
    body << "<head><title>Index of " << url << "</title></head>\n";
    body << "<body>\n";
    body << "<h1>Index of " << url << "</h1>\n";
    body << "<hr>\n";
    body << "<ul>\n";

    struct dirent *entry = NULL;
    while ((entry = readdir(dir)) != NULL)
    {
        string name = entry->d_name;

        if (name == ".")
            continue;

        string dir_url = url;

        if (!dir_url.empty() && dir_url[dir_url.length() - 1] != '/')
        {
            size_t pos = dir_url.find_last_of('/');
            if (pos != string::npos)
                dir_url = dir_url.substr(0, pos + 1);
            else
                dir_url = "/";
        }

        string href = dir_url + name;
        body << "<li><a href=\"" << href << "\">" << name << "</a></li>\n";
    }
    
    body << "</ul>\n";
    body << "<hr>\n";
    body << "</body>\n";
    body << "</html>\n";

    closedir(dir);

    string body_str = body.str();

    ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << body_str.size() << "\r\n";
    response << "Connection: close\r\n\r\n";
    response << body_str;

    return response.str();
}

string Route::handle_autoindex()
{
    struct stat st;
    string index_path;

    if (root != "")
        index_path = root + route_name + "/" + url;
    else
        index_path = filesystem_path + "/" + url;
    
    if (autoindex == "off")
    {
        if (stat(index_path.c_str(), &st) == 0 && S_ISREG(st.st_mode))
        {
            filesystem_path = index_path;
            return read_static_file(index_path);
        }
        else
        {
            return error_response("404");   
        }
    }
    else if (autoindex == "on")
        return serve_directory_listing(filesystem_path);

    return error_response("500");
}

string Route::find_abs_path(string file)
{
    char abs_path[PATH_MAX];

    if (realpath(file.c_str(), abs_path) != NULL)
    {
        string file_abs_path(abs_path);
        return file_abs_path;
    }
    throw runtime_error("realpath failed for '" + file + "': " + strerror(errno));      
}

string Route::error_response(string error)
{
    string error_file_abs_path;
    string body;

    error_file_abs_path = find_abs_path("www/errors/" + error + ".html");

	Console::message("error code: " + error, WARNING, false);
    
    ifstream file(error_file_abs_path.c_str());
    if (!file.is_open())
    {
		Console::message("Coudn't open a file", ERROR, false);
        return "";   
    }

    ostringstream buffer;
    buffer << file.rdbuf();
    body = buffer.str();

    ostringstream response;
    response << "HTTP/1.1 " << error << "\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    response << "\r\n";
    response << body;

    return response.str();
}

string get_Mime_Type(const string &path)
{
	if (path.empty())
		return (string("text/plain"));
	size_t pos = path.find_last_of('.');
	if (pos == string::npos)
		return (string("application/octet-stream"));
	string line = path.substr(pos + 1);
	if (line == "html")
		return (string("text/html"));
	else if (line == "css")
		return (string("text/css"));
	else if (line == "js")
		return (string("application/javascript"));
	else if (line == "png")
		return (string("image/png"));
	else if (line == "jpg" || line == "jpeg")
		return (string("image/jpeg"));
	else if (line == "txt")
		return (string("text/plain"));
	return (string("application/octet-stream"));
}

string Route::read_static_file(string filesystem_path)
{
    ifstream file(filesystem_path.c_str(), ios::binary);

    if (!file.is_open())
        return error_response("500");

    ostringstream buffer;
    buffer << file.rdbuf();
    string file_content = buffer.str();
    
    string content_type = get_Mime_Type(filesystem_path.c_str());

    ostringstream headers;
    headers << "HTTP/1.1 200 OK\r\n";
    headers << "Content-Type: " << content_type << "\r\n";
    headers << "Content-Length: " << file_content.size() << "\r\n";
    headers << "Connection: close\r\n\r\n";

    string response = headers.str() + file_content;
    return response;
}

string Route::serve_static_file()
{
    struct stat st;

    if (root != "")
        filesystem_path = root + request_path;
    if (stat(filesystem_path.c_str(), &st) == 0 && S_ISREG(st.st_mode))
        return read_static_file(filesystem_path);
    else
        return error_response("404");
}

bool Route::is_valid_request()
{
    bool method_is_allowed;
    bool body_matches_size;

    method_is_allowed = is_allowed_method();
    if (!method_is_allowed)
        return false;

    body_matches_size = server.does_body_match_size();
    if (!body_matches_size)
        return false;
    
    return true;
}

string Route::handle_delete() 
{
    string root = server.get_root_dir(); 
    map<string, string> h_map = request.get_Headers();
    string filename = "";

    if (h_map.count("File-To-Delete") > 0)
        filename = h_map["File-To-Delete"];
    else if (h_map.count("file-to-delete") > 0)
        filename = h_map["file-to-delete"];
    if (filename.empty())
    {
        string path = request.get_Path();
        size_t last_slash = path.find_last_of('/');
        if (last_slash != string::npos && last_slash < path.length() - 1) 
        {
            filename = path.substr(last_slash + 1);
        }
    }
    if (filename.empty() || filename == "uploads" || filename == "uploads/") 
    {
        return error_response("400");
    }
    string full_path = root + "/uploads/" + filename;
    struct stat st;
    if (stat(full_path.c_str(), &st) != 0) 
    {
        return error_response("404");
    }
    if (S_ISDIR(st.st_mode)) 
    {
        return error_response("403");
    }
    if (unlink(full_path.c_str()) != 0) 
    {
        return error_response("500");
    }
	map<int, Server *> servers = g_epoll_manager->get_Servers_Running();

	for(map<int, Server *>::iterator it = servers.begin(); it != servers.end(); ++it)
	{
		if (it->second->get_port() == this->server.get_port())
		{
			it->second->remove_uploaded_file_name(filename);
			break;
		}	
	}

    std::ostringstream resp;
    resp << "HTTP/1.1 204 No Content\r\n";
    resp << "Access-Control-Allow-Origin: *\r\n";
    resp << "Content-Length: 0\r\n";
    resp << "Connection: close\r\n";
    resp << "\r\n";
    return (resp.str());
}

string set_unique_filename(string &actual_filename, map<string, string> &headers)
{

	map<string, string>::iterator it = headers.find("host");
	if (it == headers.end())
	{
		Console::message("POST request header is missing host header.", WARNING, false);
		return NULL;
	}
	string client_str = it->second;

	int port_index = client_str.find(":") + 1;
	stringstream ss;
	int port;
	ss << client_str.substr(port_index, client_str.length());
	ss >> port;

	map<int, Server *> servers = g_epoll_manager->get_Servers_Running();

	int period_index = actual_filename.find(".");
	string extention = actual_filename.substr(period_index, actual_filename.length());

	string server_filename; 
	for (map<int, Server *>::iterator it = servers.begin(); it != servers.end(); ++it)
	{
		Server *serv = it->second;
		if (serv->get_port() == port)
		{
			stringstream index;
			server_filename = "server-upload-" + get_Current_Date_RFC(false) + extention;
			serv->create_uploaded_file_pair(server_filename, actual_filename);
			break ;
		}
	}
	return server_filename;	
}

string Route::handle_post()
{
    string body = request.get_Body();

    if (body.empty())
        return error_response("400");

    if (body.size() > server.get_client_max_body_size())
        return error_response("413");

    map<string, string> headers = request.get_Headers();
    string content_type = "";
    if (headers.find("Content-Type") != headers.end())
        content_type = headers["Content-Type"];

    if (content_type.find("multipart/form-data") == string::npos)
    {
        string upload_dir = server.get_root_dir() + "/uploads";
        struct stat st;
        if (stat(upload_dir.c_str(), &st) != 0)
            mkdir(upload_dir.c_str(), 0777);

        string filename;

        size_t name_start = body.find("filename=") + 10;
		size_t name_end = body.find("\"", name_start);
        if (name_start != string::npos && name_end != string::npos)
        filename = body.substr(name_start, name_end - name_start);

		string server_filename = set_unique_filename(filename, headers);
        
        string file_path = upload_dir + "/" + server_filename;
        ofstream file(file_path.c_str(), ios::binary);
        if (!file.is_open())
            return error_response("500");

		string headerEnd = "\r\n\r\n";
		size_t dataStart = body.find(headerEnd, name_end) + headerEnd.length();
		// 2. Find the start of the closing boundary
		string boundary = "------geckoformboundary";
		size_t dataEnd = body.find(boundary, dataStart);
		// 3. Calculate length (subtract 2 to remove the \r\n before the boundary)
		size_t fileLength = (dataEnd - 2) - dataStart;
		// 4. Extract and Write as BINARY
		string fileData = body.substr(dataStart, fileLength);
        if (!file.is_open())
            return error_response("500");
        file.write(fileData.c_str(), body.size());
        file.close();
        ostringstream response;
        response << "HTTP/1.1 201 Created\r\n";
        response << "Content-Length: 0\r\n";
        response << "Connection: close\r\n\r\n";
        return response.str();
    }
    size_t boundary_pos = content_type.find("boundary=");
    if (boundary_pos == string::npos)
        return error_response("400");
    string boundary = "--" + content_type.substr(boundary_pos + 9);
    size_t pos = 0;
    while ((pos = body.find(boundary, pos)) != string::npos)
    {
        size_t start = pos + boundary.size() + 2;
        size_t end = body.find(boundary, start);
        if (end == string::npos)
            break;
        string part = body.substr(start, end - start);
        size_t header_end = part.find("\r\n\r\n");
        if (header_end == string::npos)
        {
            pos = end;
            continue;
        }
        string part_header = part.substr(0, header_end);
        string part_body = part.substr(header_end + 4);
        time_t now = time(NULL);
        char timestamp[32];
        sprintf(timestamp, "%ld", now);
        string filename = "upload_";
        filename += timestamp;
        filename += ".dat";
        size_t fn_pos = part_header.find("filename=\"");
        if (fn_pos != string::npos)
        {
            size_t fn_end = part_header.find("\"", fn_pos + 10);
            if (fn_end != string::npos)
                filename = part_header.substr(fn_pos + 10, fn_end - (fn_pos + 10));
        }

        string upload_dir = server.get_root_dir() + "/uploads";
        struct stat st;
        if (stat(upload_dir.c_str(), &st) != 0)
            mkdir(upload_dir.c_str(), 0777);

        string full_path = upload_dir + "/" + filename;
        ofstream file(full_path.c_str(), ios::binary);
        if (!file.is_open())
            return error_response("500");
        file.write(part_body.c_str(), part_body.size());
        file.close();
        pos = end;
    }
    ostringstream response;
    response << "HTTP/1.1 201 Created\r\n";
    response << "Content-Length: 0\r\n";
    response << "Connection: close\r\n\r\n";
    return (response.str());
}

FsType Route::get_root_type()
{
    struct stat path;
    string final_root;

    final_root = root + request_path;
    if (stat(final_root.c_str(), &path) != 0)
        return FS_NOT_FOUND;
    
    if (S_ISREG(path.st_mode))
        return FS_IS_FILE;

    if (S_ISDIR(path.st_mode))
        return FS_IS_DIR;

    return FS_NOT_FOUND;
}

string Route::form_response()
{
    FsType filesystem_status;
    string cgi_output;
	string response;

    cout << "filesystem_path: " << filesystem_path << endl;
	if (redir_code == 301 || redir_code == 302)
	{
		stringstream ss;

		ss << "HTTP/1.1 " << redir_code << endl
		<< "Location: " << url << endl
		<< "Conten-Length: 0" << endl
		<< "Connection: close";

		return ss.str();
	}

    if (!is_valid_request())
    {
        if (!is_allowed_method())
            return error_response("405");
        else
            return error_response("413");
    }

    if (root != "")
        filesystem_status = get_root_type();
    else
        filesystem_status = get_filesystem_type();
    
    if (request.get_Method() == "DELETE")
        return handle_delete();

    if (request.get_Method() == "POST" && !is_cgi())
        return handle_post();
 
    switch(filesystem_status)
    {
        case FS_NOT_FOUND:
            return error_response("404");
        case FS_IS_FILE:
            if (is_cgi())
            {
                CgiHandler cgi(*this, request);
                cgi_output = cgi.run();
                if (cgi_output == "404")
                    return error_response("404");
                return cgi.build_cgi_response(cgi_output);    
            }
            else
                return serve_static_file();
        case FS_IS_DIR:
            if (is_cgi())
            {
                CgiHandler cgi(*this, request);
                cgi_output = cgi.run();
                if (cgi_output == "404")
                    return error_response("404");
                return cgi.build_cgi_response(cgi_output);   
            }
            else
                return handle_autoindex();
    }
    if (request.get_Method() == "HEAD")
    {
        size_t separator = response.find("\r\n\r\n");
        if (separator != string::npos)
        {
            return response.substr(0, separator + 4);
        }
    }
    return error_response("500");
}

const string& Route::get_route_name() const
{
    return route_name;
}

const string& Route::get_url() const
{
    return url;
}

const vector<string>& Route::get_allowed_methods() const
{
    return allowed_methods;
}

const string& Route::get_autoindex() const
{
    return autoindex;
}

const string& Route::get_root() const
{
    return root;
}

const string& Route::get_filesystem_path() const
{
    return filesystem_path;
}

const string& Route::get_request_query() const
{
    return request_query;
}

const string& Route::get_request_path() const
{
    return request_path;
}
