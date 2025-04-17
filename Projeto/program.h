#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX_TITLE   200
#define MAX_AUTHORS 200
#define MAX_YEAR      5
#define MAX_PATH     65
#define MAX_ID       20
#define MAX_DOCS    100

typedef struct MetaInfo{

    char id[MAX_ID];
    char title[MAX_TITLE];
    char authors[MAX_AUTHORS];
    char year[MAX_YEAR];
    char path[MAX_PATH];

} MetaInfo;