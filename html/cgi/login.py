import cgi
import os
from http.cookies import SimpleCookie

#!/usr/bin/env python

# Ruta del archivo donde se guardan los datos
archivo_datos = "/sgoinfre/students/dgomez-m/cookies/html/cgi/datos.txt"

# Obtener los datos del formulario
form = cgi.FieldStorage()
User = form.getvalue("User")
password = form.getvalue("password")

# Leer las cookies
cookie = SimpleCookie(os.environ.get("HTTP_COOKIE"))
cookie_user = cookie.get("User")
cookie_password = cookie.get("password")

# Función para verificar credenciales
def verificar_credenciales(User, password):
    with open(archivo_datos, "r") as archivo:
        for linea in archivo:
            datos_guardados = linea.strip().split(", ")
            datos_usuario = {dato.split(": ")[0]: dato.split(": ")[1] for dato in datos_guardados}
            if datos_usuario.get("User") == User and datos_usuario.get("password") == password:
                return True
    return False

# Verificar si hay cookies válidas
login_exitoso = False
if cookie_user and cookie_password:
    login_exitoso = verificar_credenciales(cookie_user.value, cookie_password.value)

# Si no hay cookies válidas, verificar las credenciales del formulario
if not login_exitoso and User and password:
    login_exitoso = verificar_credenciales(User, password)
    if login_exitoso:
        # Establecer cookies si el login es exitoso
        print(f"Set-Cookie: User={User}; Path=/")
        print(f"Set-Cookie: password={password}; Path=/")

# Imprimir la respuesta HTTP y el resultado del login
print("Content-type: text/html\n")
print("<html>")
print("<head>")
print("<title>Login</title>")
print("</head>")
print("<body>")
print("<h1>Login</h1>")

if login_exitoso:
    print("<p>Login exitoso. ¡Bienvenido!</p>")
else:
    print("<p>Usuario o contraseña incorrectos. Inténtalo de nuevo.</p>")

print("<form method='post' action='login.py'>")
print("User: <input type='text' name='User' required><br>")
print("password: <input type='password' name='password' required><br>")
print("<input type='submit' value='Enviar'>")
print("</form>")

print("</body>")
print("</html>")