#!/usr/bin/env python3
import os
import json

current_port = os.environ.get('SERVER_PORT', '8080')
current_host = os.environ.get('HTTP_HOST', 'localhost')

script_dir = os.path.dirname(os.path.realpath(__file__))
db_path = os.path.join(script_dir, "../../", "files.db")

result = []

def is_image(filename):
    allowed_ext = {".jpg", ".jpeg", ".png", ".gif"}
    _, ext = os.path.splitext(filename.lower())
    return ext in allowed_ext

try:
    if os.path.exists(db_path):
        with open(db_path, "r") as f:
            for line in f:
                line = line.strip()
                if not line: 
                    continue
                
                parts = line.split("|")
                if len(parts) == 4:
                    port, host, server_name, user_name = parts
                    if str(port) == str(current_port) and host == current_host:
                        if is_image(user_name):
                            result.append({
                                "serverID": server_name,
                                "userLabel": user_name,
                                "url": f"/uploads/{user_name}"
                            })
        print("Content-Type: application/json\r\n")
        print(json.dumps(result))
    else:
        print("Content-Type: application/json\r\n")
        print(json.dumps([{"error": "DB file not found", "path_tried": db_path}]))

except Exception as e:
    print("Content-Type: application/json\r\n")
    print(json.dumps([{"error": str(e)}]))