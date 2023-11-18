#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "common.h"
#include "file.h"

int create_db_file(const char *filename)
{
  int fd = open(filename, O_RDWR);
  if (fd != -1)
  {
    close(fd);
    fprintf(stderr, "File already exist\n");
    return STATUS_ERROR;
  }
  fd = open(filename, O_RDWR | O_CREAT, 0644);
  CHECK(fd, strerror(errno));
  return fd;
}

int open_db_file(const char *filename)
{
  int fd = open(filename, O_RDWR, 0644);
  CHECK(fd, strerror(errno));
  return fd;
}