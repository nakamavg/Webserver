#!/usr/bin/env python3

import cgi

print("Content-type: text/html\n")

form = cgi.FieldStorage()
your_name = form.getvalue('User')
company_name = form.getvalue('company_name')

print("<html>")
print("<head>")
print("<title>Datos Recibidos</title>")
print("</head>")
print("<body>")
print("<h1>Datos Recibidos</h1>")
print(f"<p>Tu Nombre: {your_name}</p>")
print(f"<p>Nombre de la Compañía: {company_name}</p>")
print({form})
print("</body>")
print("</html>")