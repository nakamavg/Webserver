# Usa la imagen base de Ubuntu
FROM ubuntu:latest

# Actualiza los paquetes e instala siege
RUN apt-get update && \
    apt-get install -y siege && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Establece el directorio de trabajo
WORKDIR /app

# Copia los archivos del proyecto al contenedor
COPY . .

# Define el punto de entrada
CMD ["./webserv"]
