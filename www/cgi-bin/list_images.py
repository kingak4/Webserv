#!/usr/bin/env python3
import os
import json
import sys

# 1. Mandatory CGI Header
# print("Content-Type: application/json\r\n\r\n", end="")

# 2. Get environment variables with defaults
# Note: If C++ doesn't pass these, they fallback to 8080/localhost
current_port = os.environ.get('SERVER_PORT', '8080')
current_host = os.environ.get('HTTP_HOST', 'localhost')

# 3. Robust Pathing: Find config/files.db relative to this script
# os.path.realpath(__file__) gets the absolute path to this .py file
script_dir = os.path.dirname(os.path.realpath(__file__))
# If your structure is: project/cgi-bin/list_images.py and project/config/files.db
db_path = os.path.join(script_dir, "../../", "files.db")

result = []

try:
    if os.path.exists(db_path):
        with open(db_path, "r") as f:
            for line in f:
                line = line.strip()
                if not line: continue
                
                parts = line.split("|")
                if len(parts) == 4:
                    port, host, server_name, user_name = parts
                    
                    # STRICT FILTERING
                    if str(port) == str(current_port) and host == current_host:
                        result.append({
                            "serverID": server_name,
                            "userLabel": user_name
                        })
        print(json.dumps(result))
    else:
        # DB File not found - return error info inside the list for debugging
        print(json.dumps([{"error": "DB file not found", "path_tried": db_path}]))

except Exception as e:
    print(json.dumps([{"error": str(e)}]))