# CLI database

El CLI permite realizar operaciones de CRUD (Create,Read, Update y Delete) en un datafile 
`*.db` el cual almacena la estructura de datos Employee, cada employee tiene los campos: nombre,Dirección y horas de trabajo.
Estos datos se guardan en el archivo para cargar y editarlos.

## Flags

Los flags para operar el CLI son los siguientes:

- `-f` (File name) 
- `-n` (New File) para crear un data file `./db-cli -f ./data.db -n`

## Structure of project
La estructura del proyecto es la siguiente:

- bin/
- include/
- src/
  - main.c
  - file.c
  - parse.c
- obj/
- Makefile
- README.md