#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>

#include "structures/Objects.cpp"


// Scene :
std::vector< Object > objects;
std::vector< unsigned int > lights;
// lights is the whole set of emissive objects


void createScene() {
    // setup scene:
    objects.push_back( Sphere(1e5, Vec( 1e5+1,40.8,81.6),    Vec(0,0,0)    ,Vec(.75,.25,.25), DIFFUSE)   );//Left
    objects.push_back( Sphere(1e5, Vec(-1e5+99,40.8,81.6),   Vec(0,0,0)    ,Vec(.25,.25,.75), DIFFUSE)   );//Right
    objects.push_back( Sphere(1e5, Vec(50,40.8, 1e5),        Vec(0,0,0)    ,Vec(.75,.75,.75), DIFFUSE)   );//Back
    objects.push_back( Sphere(1e5, Vec(50,40.8,-1e5+170),    Vec(0,0,0)    ,Vec(0,0,0)      , DIFFUSE)   );//Front
    objects.push_back( Sphere(1e5, Vec(50, 1e5, 81.6),       Vec(0,0,0)    ,Vec(.75,.75,.75), DIFFUSE)   );//Bottom
    objects.push_back( Sphere(1e5, Vec(50,-1e5+81.6,81.6),   Vec(0,0,0)    ,Vec(.75,.75,.75), DIFFUSE)   );//Top
    objects.push_back( Sphere(16.5,Vec(27,16.5,47),          Vec(0,0,0)    ,Vec(1,1,1)*.999 , MIRROR)    );//Mirr
    objects.push_back( Sphere(16.5,Vec(73,16.5,78),          Vec(0,0,0)    ,Vec(1,1,1)*.999 , MIRROR)    );//Change to Glass
    objects.push_back( Sphere(5,   Vec(50,70,50)  ,          Vec(1,1,1)    ,Vec(0,0,0)      , EMMISSIVE) );//Light
}

int main(int argc, char *argv[]){
    int w=1024, h=768, samps = argc==2 ? atoi(argv[1])/4 : 1; // # samples
    Ray cam(Vec(50,52,295.6), Vec(0,-0.042612,-1).normalize()); // camera center and direction
    Vec cx=Vec(w*.5135/h), cy=(cx.cross(cam.d)).normalize()*.5135, *pixelsColor=new Vec[w*h];

    // setup scene:
    spheres.push_back( Sphere(1e5, Vec( 1e5+1,40.8,81.6),    Vec(0,0,0)    ,Vec(.75,.25,.25), DIFFUSE)   );//Left
    spheres.push_back( Sphere(1e5, Vec(-1e5+99,40.8,81.6),   Vec(0,0,0)    ,Vec(.25,.25,.75), DIFFUSE)   );//Right
    spheres.push_back( Sphere(1e5, Vec(50,40.8, 1e5),        Vec(0,0,0)    ,Vec(.75,.75,.75), DIFFUSE)   );//Back
    spheres.push_back( Sphere(1e5, Vec(50,40.8,-1e5+170),    Vec(0,0,0)    ,Vec(0,0,0)      , DIFFUSE)   );//Front
    spheres.push_back( Sphere(1e5, Vec(50, 1e5, 81.6),       Vec(0,0,0)    ,Vec(.75,.75,.75), DIFFUSE)   );//Bottom
    spheres.push_back( Sphere(1e5, Vec(50,-1e5+81.6,81.6),   Vec(0,0,0)    ,Vec(.75,.75,.75), DIFFUSE)   );//Top
    spheres.push_back( Sphere(16.5,Vec(27,16.5,47),          Vec(0,0,0)    ,Vec(1,1,1)*.999 , MIRROR)    );//Mirr
    spheres.push_back( Sphere(16.5,Vec(73,16.5,78),          Vec(0,0,0)    ,Vec(1,1,1)*.999 , MIRROR)    );//Change to Glass
    spheres.push_back( Sphere(5,   Vec(50,70,50)  ,          Vec(1,1,1)    ,Vec(0,0,0)      , EMMISSIVE) );//Light
    lights.push_back( 8 );

    // ray trace:
    for (int y=0; y<h; y++) {                       // Loop over image rows
        fprintf(stderr,"\rRendering (%d spp) %5.2f%%",samps*4,100.*y/(h-1));
        for (unsigned short x=0; x<w; x++) {  // Loop cols
            Vec r(0,0,0);
            for( unsigned int sampleIt = 0 ; sampleIt < samps ; ++sampleIt ) {
                double dx = ((double)(rand())/RAND_MAX);
                double dy = ((double)(rand())/RAND_MAX);
                Vec d = cx*( ( x + dx )/w - .5) +
                        cy*( ( y + dy )/h - .5) + cam.d;
                r = r + radiance(Ray(cam.o+d*140,d.normalize()),0)*(1./samps);
            }

            pixelsColor[x + (h-1-y) * w] = pixelsColor[x + (h-1-y) * w] + Vec(clamp(r.x),clamp(r.y),clamp(r.z));
        } // Camera rays are pushed ^^^^^ forward to start in interior
    }

    // save image:
    FILE *f = fopen("image.ppm", "w");         // Write image to PPM file.
    fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
    for (int i=0; i<w*h; i++)
        fprintf(f,"%d %d %d ", (int)(pixelsColor[i].x * 255), (int)(pixelsColor[i].y * 255), (int)(pixelsColor[i].z * 255));
    fclose(f);
}