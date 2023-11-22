#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "parse.h"

#define CHECK_FD(X)                                           \
  do                                                          \
  {                                                           \
    if ((X) < 0)                                              \
    {                                                         \
      fprintf(stderr, "ERROR ("__FILE__                       \
                      ":%d) -- Got a bad FD from the user\n", \
              __LINE__);                                      \
      return STATUS_ERROR;                                    \
    }                                                         \
  } while (0)
#define CHECK_HEAP(X)                                               \
  do                                                                \
  {                                                                 \
    if ((X) == NULL)                                                \
    {                                                               \
      fprintf(stderr, "ERROR ("__FILE__                             \
                      ":%d) -- Malloc failed to create DB space\n", \
              __LINE__);                                            \
      return STATUS_ERROR;                                          \
    }                                                               \
  } while (0)

void list_employees(struct dbheader_t *dbhdr, struct employee_t *employeesOut)
{
  unsigned short i = 0;
  printf("Total: %d\n", dbhdr->count);
  for (; i < dbhdr->count; i++)
  {
    printf("Employee [%i]\n", i);
    printf("\tName: %s\n", employeesOut[i].name);
    printf("\tAddress: %s\n", employeesOut[i].address);
    printf("\tHours: %d\n", employeesOut[i].hours);
  }
}
int add_employee(struct dbheader_t *dbhdr, struct employee_t *employeesOut, char *addstring)
{
  char *name = strtok(addstring, ",");
  char *address = strtok(NULL, ",");
  char *hours = strtok(NULL, ",");
  printf("name: %s\naddress: %s\nhours:%s ", name, address, hours);
  strncpy(employeesOut[dbhdr->count - 1].name, name, sizeof(employeesOut[dbhdr->count - 1].name));
  strncpy(employeesOut[dbhdr->count - 1].address, address, sizeof(employeesOut[dbhdr->count - 1].address));
  employeesOut[dbhdr->count - 1].hours = atoi(hours);
  printf("(%d int)\n", employeesOut[dbhdr->count - 1].hours);
  return STATUS_SUCCESS;
}

int create_db_header(int fd, struct dbheader_t **headerOut)
{
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  CHECK_HEAP(header);
  header->version = 0x1;
  header->count = 0;
  header->magic = HEADER_MAGIC;
  header->filesize = sizeof(struct dbheader_t);
  *headerOut = header;
  return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t **headerOut)
{
  CHECK_FD(fd);
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  CHECK_HEAP(header);
  if (read(fd, header, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t))
  {
    perror("read:");
    free(header);
    return STATUS_ERROR;
  }
  header->version = ntohs(header->version);
  header->count = ntohs(header->count);
  header->magic = ntohl(header->magic);
  header->filesize = ntohl(header->filesize);
  if (header->magic != HEADER_MAGIC)
  {
    fprintf(stderr, "Impromper header magic\n");
    free(header);
    return STATUS_ERROR;
  }
  if (header->version != 1)
  {
    fprintf(stderr, "Impromper header version\n");
    free(header);
    return STATUS_ERROR;
  }
  struct stat dbstat = {0};
  fstat(fd, &dbstat);
  if (header->filesize != dbstat.st_size)
  {
    fprintf(stderr, "Corrupted database");
    free(header);
    return STATUS_ERROR;
  }
  *headerOut = header;
}
int read_employess(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut)
{
  CHECK_FD(fd);
  unsigned short count = dbhdr->count; // count of users
  struct employee_t *employees = calloc(count, sizeof(struct employee_t));
  CHECK_HEAP(employees);
  read(fd, employees, count * sizeof(struct employee_t));
  unsigned short i = 0;
  for (; i < count; i++)
  {
    employees[i].hours = ntohl(employees[i].hours);
  }
  *employeesOut = employees;
  return STATUS_SUCCESS;
}

void output_file(int fd, struct dbheader_t *dbhdr, struct employee_t *employees)
{
  if (fd < 0)
  {
    fprintf(stderr, "Got a bad FD from the user\n");
    exit(-1);
  }
  unsigned short realcount = dbhdr->count;
  dbhdr->version = ntohs(dbhdr->version);
  dbhdr->count = ntohs(dbhdr->count);
  dbhdr->magic = ntohl(dbhdr->magic);
  dbhdr->filesize = ntohl(sizeof(struct dbheader_t) + realcount * sizeof(struct employee_t));
  lseek(fd, 0, SEEK_SET);
  write(fd, dbhdr, sizeof(struct dbheader_t));

  unsigned short i = 0;
  for (; i < realcount; i++)
  {
    employees->hours = htonl(employees[i].hours);
    write(fd, &employees[i], sizeof(struct employee_t));
  }
}