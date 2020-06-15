/* 3dfunc.c - transformation functions */


#include "structs.h"


void rotate_x(MODEL *model, double cosine, double sine)
{
    register int vertex;
    double   y,z;

    for (vertex = 0; vertex < model->vertices; vertex++)
    {
        y = model->world[vertex].y;
        z = model->world[vertex].z;

        model->world[vertex].y = y*cosine - z*sine;
        model->world[vertex].z = y*sine + z*cosine;
    }
}

void rotate_y(MODEL *model, double cosine, double sine)
{
    register int vertex;
    double   x,z;

    for (vertex = 0; vertex < model->vertices; vertex++)
    {
        x = model->world[vertex].x;
        z = model->world[vertex].z;

        model->world[vertex].x = x*cosine - z*sine;
        model->world[vertex].z = x*sine + z*cosine;
    }
}

void rotate_z(MODEL *model, double cosine, double sine)
{
    register int vertex;
    double   x,y;

    for (vertex = 0; vertex < model->vertices; vertex++)
    {
        x = model->world[vertex].x;
        y = model->world[vertex].y;

        model->world[vertex].x = x*cosine - y*sine;
        model->world[vertex].y = x*sine + y*cosine;
    }
}

void translate(MODEL *model, double x, double y, double z)
{
    register int vertex;

    for (vertex = 0; vertex < model->vertices; vertex++)
    {
        model->world[vertex].x += x;
        model->world[vertex].y += y;
        model->world[vertex].z += z;
    }
}

void scale(MODEL *model, double scale_factor)
{
    register int vertex;

    for (vertex = 0; vertex < model->vertices; vertex++)
    {
        model->world[vertex].x *= scale_factor;
        model->world[vertex].y *= scale_factor;
        model->world[vertex].z *= scale_factor;
    }
}
