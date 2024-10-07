print("<html>")
print("<head>")
print("<title>CGI Form Example</title>")
print("<style>")
print("body {")
print("    background-color: #f0f0f0;")  # Light gray background color
print("    font-family: Arial, sans-serif;")  # Use Arial font
print("    padding: 20px;")  # Add padding to the body
print("}")
print("form {")
print("    background-color: #ffffff;")  # White background color for the form
print("    padding: 20px;")  # Add padding to the form
print("    border-radius: 10px;")  # Add rounded corners to the form
print("    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);")  # Add a subtle shadow to the form
print("}")
print("</style>")
print("</head>")
print("<body>")
print("<h1>Enter Your Information</h1>")
print("<form action=\"/cgi/geeks.py\" method=\"post\">")
print("<label for=\"your_name\">Your Name:</label>")
print("<input type=\"text\" id=\"your_name\" name=\"your_name\" placeholder=\"Your Name\"><br>")
print("<label for=\"company_name\">Company Name:</label>")
print("<input type=\"text\" id=\"company_name\" name=\"company_name\" placeholder=\"Company Name\"><br>")
print("<input type=\"submit\" value=\"Submit\" />")
print("</form>")
print("</body>")
print("</html>")