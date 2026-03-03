#!/usr/bin/python3
import os
import sys
from urllib.parse import parse_qs

method = os.environ.get("REQUEST_METHOD", "GET")
user_name = "Guest"
age = 0
form = {}

if method.upper() == "GET":
    query_string = os.environ.get("QUERY_STRING", "")
    form = parse_qs(query_string)
    # print(form)

elif method.upper() == "POST":
    content_length = int(os.environ.get("CONTENT_LENGTH") or 0)
    post_data = sys.stdin.read(content_length)
    form = parse_qs(post_data)
    # print(form)
    
if "name" in form:
    user_name = form.get("name", ["Guest"])[0]
    age = form.get("age", [0])[0]
    # print('USER NAME', user_name)

print("Content-Type: text/html")
print()

print(f"""<html>
<head><title>CGI POST Demo</title></head>
<body>
<h1>Hello, {user_name} {age}!</h1>
<form method="post" action="">
    <label for="name">Enter your name:</label>
    <input type="text" id="name" name="name" value="{user_name}">
    <input type="text" id="age" name="age" value="{age}">
    <button type="submit">Submit</button>
</form>
</body>
</html>""")
