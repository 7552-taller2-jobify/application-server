[![Build Status](https://travis-ci.org/7552-taller2-jobify/application-server.svg?branch=master)](https://travis-ci.org/7552-taller2-jobify/application-server)

# application-server
Aplicación con el objetivo de brindar una interfaz para la comunicación de los diferentes clientes que se conecten a la misma. Para la interfaz de comunicación se utilizará Restful API, que definirá la forma de las solicitudes y respuestas de los diferentes servicios que brindará el servidor.


## Requerimientos

1. Instalar docker
  ```
  docker run -it -v $PWD:/usr/src/app 7552taller2jobify/application-server /bin/bash
  ```
  Esto genera un contenedor y podemos interactuarl con la consola que ya tiene todo para seguir a Como correrlo

## Como correrlo

1. Correr "cmake ." sobre la carpeta principal
  ```
  cmake .
  ```

2. Correr "make"
  ```
  make
  ```

3. Correr "./Server"
  ```
  ./Server
  ```

4. Desde el browser ir a <http://localhost:8000/api/hola>

