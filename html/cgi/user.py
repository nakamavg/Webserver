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

# Mostrar el nombre de usuario si se ha enviado el formulario
if User and password:
    print(f"<p>Usuario registrado: {User}</p>")

# Agregar enlace para volver al home
print('<a href="../index.html">Volver al home</a>')

print("</form>")
print("</body>")
print("</html>")