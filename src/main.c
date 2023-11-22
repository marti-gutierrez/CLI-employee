#include "file.h"
#include "parse.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>

void printf_usage(char *argv[])
{
  printf("Usage: %s -n -f <database file>\n", argv[0]);
  printf("\t -n - create new database file\n");
  printf("\t -f - (required) path to database file\n");
}

int main(int argc, char **argv)
{
  int opt = 0;
  struct dbheader_t *dbhdr = NULL;
  struct employee_t *employees = NULL;
  int dbfd = -1;
  char *filepath = NULL;
  char *addString = NULL;
  bool newFile = false;
  bool list = false;
  while ((opt = getopt(argc, argv, "nf:a:l")) != -1)
  {
    switch (opt)
    {
    case 'n':
      newFile = true;
      break;
    case 'f':
      filepath = optarg;
      break;
    case 'a':
      addString = optarg;
      break;
    case 'l':
      list = true;
      break;
    case '?':
      printf("Unknown option -%c\n", opt);
      break;
    default:
      return -1;
    }
  }
  if (filepath == NULL)
  {
    printf("Filepath is a required argument\n");
    printf_usage(argv);
    return 0;
  }
  if (newFile)
  {
    dbfd = create_db_file(filepath);
    CHECK(dbfd, "Unable to create DB file");
    CHECK(create_db_header(dbfd, &dbhdr), "Unable to create DB header");
  }
  else
  {
    dbfd = open_db_file(filepath);
    CHECK(dbfd, "Unable to create DB file");
    CHECK(validate_db_header(dbfd, &dbhdr), "Unable to create DB header");
  }
  CHECK(read_employess(dbfd, dbhdr, &employees), "Failed to read employees");
  if (addString)
  {
    dbhdr->count++;
    employees = realloc(employees, dbhdr->count * sizeof(struct employee_t));
    add_employee(dbhdr, employees, addString);
  }
  if (list)
    list_employees(dbhdr, employees);
  output_file(dbfd, dbhdr, employees);
  return 0;
}