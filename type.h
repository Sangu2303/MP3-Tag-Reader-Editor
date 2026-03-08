#ifndef TYPES_H
#define TYPES_H



/* Status will be used in fn. return type */
typedef enum
{
    success,
    failure
} status;

typedef enum
{
    view,
    edit,
    help,
    unsupported
} operationType;

#endif
