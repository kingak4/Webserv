#!/usr/bin/env python3
import os
import json

# 1. Header
# print("Content-Type: application/json\r\n\r\n", end="")

# 2. Robust Pathing
# __file__ is the location of list_images.py
# os.path.dirname(__file__) is the cgi-bin folder
# Then we go up one level to the project root, then into uploads
script_dir = os.path.dirname(os.path.realpath(__file__))
upload_dir = os.path.join(script_dir, "..", "uploads")

result = []

try:
    if os.path.exists(upload_dir):
        # List files and filter for images
        files = os.listdir(upload_dir)
        valid_exts = ('.png', '.jpg', '.jpeg', '.gif', '.webp')
        result = [f for f in files if f.lower().endswith(valid_exts)]
        
        # Sort so they appear in a consistent order
        result.sort()
    
    print(json.dumps(result))

except Exception as e:
    # If this prints, your JS console will show the error string
    print(json.dumps([f"Error: {str(e)}", f"Looked in: {upload_dir}"]))