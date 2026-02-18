#!/usr/bin/env python3
import os

"""
Docstring for www.tests
Simple cgi script for testing webserv
Prints HTMl response
"""

query_string = os.environ["QUERY_STRING"]

print('Hello', query_string)
print()
print('Welcome to CGI')
