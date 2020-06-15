/* structs.h */


#define MAXVERTS	8

#define MAXFACETS	6


typedef unsigned int BYTE;

typedef struct
{
    double x,y,z;
} POINT;

typedef struct
{
    BYTE corners;
    BYTE point[4];
} FACET;

typedef struct
{
    int   vertices;
    int   faces;
    POINT world[MAXVERTS];
    FACET face[MAXFACETS];
} MODEL;
