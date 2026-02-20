#!/usr/bin/python3
import cgi
import os
import sys
from urllib.parse import parse_qs

# Determine request method
method = os.environ.get("REQUEST_METHOD", "GET")
# Initialize variable
user_name = "Guest"

if method.upper() == "POST":
    # Read POST data from stdin
    content_length_str = os.environ.get("CONTENT_LENGTH")
    content_length = int(os.environ.get("CONTENT_LENGTH") or 0)
    post_data = sys.stdin.read(content_length)

    # Parse the data
    # post_data = "name=Alice"
    form = parse_qs(post_data)
    user_name = form.get("name", ["Guest"])[0]
    print('USER NAME', user_name)

# Send HTTP header
print("Content-Type: text/html")
print()  # Blank line separates header from body

# Output HTML
print(f"""<html>
<head><title>CGI POST Demo</title></head>
<body>
<h1>Hello, {user_name}!</h1>
<form method="post" action="">
    <label for="name">Enter your name:</label>
    <input type="text" id="name" name="name" value="{user_name}">
    <button type="submit">Submit</button>
</form>
</body>
</html>""")