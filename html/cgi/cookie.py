import os
from http.cookies import SimpleCookie

# Obtener las cookies del entorno
cookie_string = os.environ.get('HTTP_COOKIE')
if cookie_string:
    cookie = SimpleCookie(cookie_string)
    user = cookie.get('User')
    if user:
        print(f'Hello {user.value}!')
    else:
        print('Hello Guest!')
else:
    print('Hello Guest!')