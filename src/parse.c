#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "parse.h"

int create_db_header(int fd, struct dbheader_t **headerOut)
{
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL)
  {
    fprintf(stderr, "Malloc failed to create DB header\n");
    return STATUS_ERROR;
  }
  header->version = 0x1;
  header->count = 0;
  header->magic = HEADER_MAGIC;
  header->filesize = sizeof(struct dbheader_t);
  *headerOut = header;
  return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t **headerOut)
{
  if (fd < 0)
  {
    fprintf(stderr, "Got a bad FD from the user\n");
    return STATUS_ERROR;
  }
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL)
  {
    fprintf(stderr, "Malloc failed to create a db header\n");
    return STATUS_ERROR;
  }
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
int read_employess(int fd, struct dbheader_t *, struct employee_t *employeesOut)
{
}

void output_file(int fd, struct dbheader_t *dbhdr)
{
  if (fd < 0)
  {
    fprintf(stderr, "Got a bad FD from the user");
    exit(-1);
  }
  dbhdr->version = ntohs(dbhdr->version);
  dbhdr->count = ntohs(dbhdr->count);
  dbhdr->magic = ntohl(dbhdr->magic);
  dbhdr->filesize = ntohl(dbhdr->filesize);
  lseek(fd, 0, SEEK_SET);
  write(fd, dbhdr, sizeof(struct dbheader_t));
}