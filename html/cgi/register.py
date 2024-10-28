import cgi

#!/usr/bin/env python

# Ruta del archivo donde se guardarán los datos
archivo_datos = "/sgoinfre/students/dgomez-m/cookies/html/cgi/datos.txt"

# Obtener los datos del formulario
form = cgi.FieldStorage()
User = form.getvalue("User")
password = form.getvalue("password")

# Guardar los datos en el archivo
with open(archivo_datos, "a") as archivo:
    archivo.write(f"User: {User}, password: {password}\n")

# Imprimir la respuesta HTTP y el formulario
print("Content-type: text/html\n")
print("<html>")
print("<head>")
print("<title>Formulario</title>")
print("</head>")
print("<body>")
print("<h1>Formulario</h1>")
print("<form method='post' action='user.py'>")
print("User: <input type='text' name='User' required><br>")
print("password: <input type='password' name='password' required><br>")
print("<input type='submit' value='Enviar'>")
print("</form>")
print("</body>")
print("</html>")