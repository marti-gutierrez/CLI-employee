#ifndef COMMON_H
#define COMMON_H

#define STATUS_ERROR -1
#define STATUS_SUCCESS 0

#define CHECK(X, STR) ({int __val = (X); (__val == -1 ? \
  ({fprintf(stderr,"ERROR ("__FILE__":%d) -- %s\n",__LINE__,(STR)); \
  return STATUS_ERROR;-1;}) : __val); })

#endif /*COMMON_H*/