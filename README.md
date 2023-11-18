# CLI database

El CLI trabajara como un CRUD, algunas de otros flags disponibles son:

- `-n` (New File) para crear un data file `./db-cli -f ./data.db -n`
- `-f ./my-db.db` (File name) Si existe un archivo enviamos un `char *str = "./db.db"` 

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