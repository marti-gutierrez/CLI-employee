# CLI database

El CLI permite realizar operaciones de CRUD (Create,Read, Update y Delete) en un datafile 
`*.db` el cual almacena la estructura de datos Employee, cada employee tiene los campos: nombre,Dirección y horas de trabajo.
Estos datos se guardan en el archivo para cargar y editarlos.

## Flags

Los flags para operar el CLI son los siguientes:

- `-f` (File name) necesita el path o el nombre del archivo *.db* `./data.db` 
- `-n` (New File)
- `-a` (Add file) necesita un string con nombre,direccion y horas de trabajo separadas por una coma`"Martin G.H, street view 28, 23"` 
- `-l` (list employee)

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
