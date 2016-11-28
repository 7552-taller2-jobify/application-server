[![Build Status](https://travis-ci.org/7552-taller2-jobify/application-server.svg?branch=master)](https://travis-ci.org/7552-taller2-jobify/application-server)

# application-server
Aplicación con el objetivo de brindar una interfaz para la comunicación de los diferentes clientes que se conecten a la misma. Para la interfaz de comunicación se utilizará Restful API, que definirá la forma de las solicitudes y respuestas de los diferentes servicios que brindará el servidor.


## Requerimientos

1. Instalar docker
  ```
  dev/run
  ```
  Esto genera un contenedor y podemos interactuarl con la consola que ya tiene todo para seguir a Como correrlo

## Como correrlo

1. Correr "cmake ." sobre la carpeta principal
  ```
  dev/run make
  ```

2. Correr "./Server"
  ```
  dev/run ./Server
  ```

3. Desde el browser ir a <http://localhost:8000/api/hola>

4. Apaga la maquina
  ```
  docker ps
  docker stop <CONTAINER>
  ```

# DEB instalación

## Testing

Te monta un docker pelado para poder testear la instalación del DEB.

```
$ empty/run
```
