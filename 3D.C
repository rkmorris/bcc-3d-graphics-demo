/* 3d.c - simple 3D graphics demonstration program
   Copyright (C) 1993 - 1994 Richard K. Morris

   Compile with Borland C++ 3.1 for DOS
   Uses Borland Graphics Interface (BGI)

   Build: BCC.EXE 3D.C 3DFUNC.C GRAPHICS.LIB

   Redistribute: 3D.EXE EGAVGA.BGI


   Ideas:
   1. Incorporate additional models: cube, etc.
   2. User-adjustable light source
   3. User-adjustable model (roll, yaw, pitch)
   4. Translation/Scaling/Reflection
   5. Depth sort - using a simple concave model

*/

#include <dos.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <graphics.h>

#include "structs.h"
#include "shapes.h"
#include "3dfunc.h"


/* define light source location in world coordinates */
POINT light_src = {300, 200, 150};


void apply_perspective(POINT *point, double distance)
{
    double Ze;

    Ze = 1000-(point->z);

    point->x = distance*point->x / Ze;
    point->y = distance*point->y / Ze;
}

void init_light_source(void)
{
    double l;

    l = sqrt(light_src.x*light_src.x +
             light_src.y*light_src.y +
             light_src.z*light_src.z);

    light_src.x /= l;
    light_src.y /= l;
    light_src.z /= l;
}

int is_visible(MODEL *model, BYTE face)
{
    double Xa,Ya,Xb,Yb;
    BYTE   *p;

    /* vector from vertex 0 to vertex 1 */
    p  = &model->face[face].point[0];
    Xa = model->world[*p].x - model->world[*(p+1)].x;
    Ya = model->world[*p].y - model->world[*(p+1)].y;

    /* vector from vertex 2 to vertex 1 */
    p++;
    Xb = model->world[*(p+1)].x - model->world[*p].x;
    Yb = model->world[*(p+1)].y - model->world[*p].y;

    /* face visible if result > 0 */
    return ((Xa*Yb - Ya*Xb) >= 0);
}

double compute_illumination(MODEL *model, BYTE face)
{
    double n,Xa,Xb,Za,Zb,Ya,Yb;
    POINT  nh;
    BYTE   *p;

    /* vector from vertex 0 to vertex 1 */
    p  = &model->face[face].point[0];
    Xa = model->world[*p].x - model->world[*(p+1)].x;
    Ya = model->world[*p].y - model->world[*(p+1)].y;
    Za = model->world[*p].z - model->world[*(p+1)].z;

    /* vector from vertex 1 to vertex 2 */
    p++;
    Xb = model->world[*(p+1)].x - model->world[*p].x;
    Yb = model->world[*(p+1)].y - model->world[*p].y;
    Zb = model->world[*(p+1)].z - model->world[*p].z;

    /* calculate surface perpendicular vector */
    nh.x = Ya*Zb - Za*Yb;
    nh.y = Xa*Zb - Za*Xb;
    nh.z = Xa*Yb - Ya*Xb;

    n = sqrt(nh.x*nh.x + nh.y*nh.y + nh.z*nh.z);

    nh.x /= n;
    nh.y /= n;
    nh.z /= n;

    n = nh.x*light_src.x + nh.y*light_src.y + nh.z*light_src.z;

    if (n<0)
        n = 0;

    return (n);
}

void draw_model(MODEL *model, int x, int y)
{
    register BYTE face;
    FACET *p;
//  POINT temp_point;
    int   polypoints[10];
    int   i,j;
    double illumination;

    for (face = 0; face < model->faces; face++)
    {
        if (is_visible(model, face))
        {
            p = &model->face[face];
            for (i=j=0; i < model->face[face].corners*2; i+=2,j++)
            {
				/* calculate perspective */
//              temp_point.x = model->world[p->point[j]].x;
//              temp_point.y = model->world[p->point[j]].y;
//              temp_point.z = model->world[p->point[j]].z;
//              apply_perspective(&temp_point, -500);
//              polypoints[i]   = (int) temp_point.x+x;
//              polypoints[i+1] = (int) temp_point.y+y;

				/* add polygon points */
                polypoints[i]   = (int) model->world[p->point[j]].x+x;
                polypoints[i+1] = (int) model->world[p->point[j]].y+y;
            }
            /* close polygon */
            polypoints[i]   = polypoints[0];
            polypoints[i+1] = polypoints[1];

			/* illumination calculation determines colour index */
            illumination = compute_illumination(model, face);

			/* draw face using filled polygon */
            setfillstyle(SOLID_FILL, illumination*14.0+1);
            setcolor(illumination);
            fillpoly(model->face[face].corners+1, polypoints);
        }
    }
}

int main(void)
{
    int gdriver = VGA, gmode = VGAMED;
    int errorcode, page = 1;
    double cosine, sine, ncosine, nsine;

    MODEL* model;

    /* initialise graphics mode */
    initgraph(&gdriver, &gmode, "C:\\TOOLS\\BCC31\\BGI");
    if ((errorcode = graphresult()) != grOk)
    {
        printf("Graphics error: %s\n", grapherrormsg(errorcode));
        exit(1);
    }

    /* create gradient palette entries */
    setrgbpalette(EGA_BLUE, 0, 0, 26);			// 5, 5, 5
    setrgbpalette(EGA_GREEN, 0, 0, 28);			// 8, 8, 8
    setrgbpalette(EGA_CYAN, 0, 0, 30);			// 11, 11, 11
    setrgbpalette(EGA_RED, 0, 0, 32);			// 14, 14, 14
    setrgbpalette(EGA_MAGENTA, 0, 0, 34);		// 17, 17, 17
    setrgbpalette(EGA_BROWN, 0, 0, 36);			// 20, 20, 20
    setrgbpalette(EGA_LIGHTGRAY, 0, 0, 38);		// 24, 24, 24
    setrgbpalette(EGA_DARKGRAY, 0, 0, 39);		// 28, 28, 28
    setrgbpalette(EGA_LIGHTBLUE, 0, 0, 40);		// 32, 32, 32
    setrgbpalette(EGA_LIGHTGREEN, 0, 0, 41);	// 36, 36, 36
    setrgbpalette(EGA_LIGHTCYAN, 0, 0, 42);		// 40, 40, 40
    setrgbpalette(EGA_LIGHTRED, 0, 0, 43);		// 45, 45, 45
    setrgbpalette(EGA_LIGHTMAGENTA, 0, 0, 44);	// 50, 50, 50
    setrgbpalette(EGA_YELLOW, 0, 0, 45);		// 56, 56, 56
    setrgbpalette(EGA_WHITE, 0, 0, 46);			// 63, 63, 63

    /* initialise light source */
    init_light_source();

    /* setup model */
    model = &wedge;
    scale(model, 2);

    /* start spinning model until key press */
    cosine = cos(0.1); sine = sin(0.1);
    ncosine = cos(-0.1); nsine = sin(-0.1);
    do
    {
       /* render new page */
       setactivepage(!page);
       setfillstyle(SOLID_FILL, BLACK);
       bar(0, 0, getmaxx(), getmaxy());

       /* apply rotation to x,y,z axis */
       rotate_x(model, cosine, sine);
       rotate_y(model, cosine, sine);
       rotate_z(model, ncosine, nsine);

       /* finally, redraw model */
       draw_model(model, 280, 110);

       /* page flip */
       setvisualpage(page = !page);

       delay(30);
    } while (!kbhit());

    getch();	/* flush keyboard buffer */

    closegraph();
    return 0;
}
