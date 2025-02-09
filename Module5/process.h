#ifndef PROCESS_H
#define PROCESS_H

typedef struct
{
    int id;
    char *name;
    long runtime;
    int priority;
} process_t;

#endif
