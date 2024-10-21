import cgi
import html

form = cgi.FieldStorage()

first_name = html.escape(form.getvalue('first_name', ''))
last_name = html.escape(form.getvalue('last_name', ''))

content = f"""<!DOCTYPE html>
<html>
<head>
<title>Hello - Second CGI Program</title>
</head>
<body>
<h2>Hello {first_name} {last_name}</h2>
</body>
</html>
"""

length = len(content.encode('utf-8'))

print("HTTP/1.1 200 OK")
print("Content-type:text/html")
print(f"Content-Length: {length}")
print()
print(content)
