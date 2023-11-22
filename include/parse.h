#ifndef _PARSE_H
#define _PARSE_H

#define HEADER_MAGIC 0x4A55414E // 32-bits (4bytes) 'W','i','k','y'

struct dbheader_t
{
  unsigned int magic;     // uint32_t (4bytes)
  unsigned short version; // uint16_t (2byteS)
  unsigned short count;   // uint16_t
  unsigned int filesize;  // uint32_t
};

struct employee_t
{
  char name[80];      // int8_t
  char address[80];   // int8_t
  unsigned int hours; // uint32_t
};

int create_db_header(int fd, struct dbheader_t **headerOut);
int validate_db_header(int fd, struct dbheader_t **headerOut);
int read_employess(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut);
void output_file(int fd, struct dbheader_t *, struct employee_t *employees);
int add_employee(struct dbheader_t *dbhdr, struct employee_t *employeesOut, char *addstring);
void list_employees(struct dbheader_t *dbhdr, struct employee_t *employeesOut);

#endif