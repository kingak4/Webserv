
# import os

# """
# Docstring for www.tests
# Simple cgi script for testing webserv
# Prints HTMl response
# """

# query_string = os.environ["QUERY_STRING"]

# print('Hello', query_string)
# print()
# print('Welcome to CGI')

#!/usr/bin/python3

import os
import sys
import urllib.parse

print("Content-Type: text/html\n")

if os.environ["REQUEST_METHOD"] == "POST":
    content_length = int(os.environ.get("CONTENT_LENGTH", 0))
    post_data = sys.stdin.read(content_length)

    data = urllib.parse.parse_qs(post_data)

    email = data.get("email", [""])[0]
    subject = data.get("subject", [""])[0]
    message = data.get("message", [""])[0]

    # Store in file
    with open("messages.txt", "a") as f:
        f.write(f"Email: {email}\n")
        f.write(f"Subject: {subject}\n")
        f.write(f"Message: {message}\n")
        f.write("-----\n")

    print("<h1>Message received</h1>")
else:
    print("<h1>Invalid request</h1>")
