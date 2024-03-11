#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>

#include "structures/Objects.cpp"


// Scene :
std::vector< Sphere > objects;
std::vector< unsigned int > lights;
// lights is the whole set of emissive objects


void createScene() {
    // setup scene:
    objects.push_back( Sphere(1e5, Vec3( 1e5+1,40.8,81.6),    Vec3(0,0,0)    ,Vec3(.75,.25,.25), DIFFUSE)   );//Left
    objects.push_back( Sphere(1e5, Vec3(-1e5+99,40.8,81.6),   Vec3(0,0,0)    ,Vec3(.25,.25,.75), DIFFUSE)   );//Right
    objects.push_back( Sphere(1e5, Vec3(50,40.8, 1e5),        Vec3(0,0,0)    ,Vec3(.75,.75,.75), DIFFUSE)   );//Back
    objects.push_back( Sphere(1e5, Vec3(50,40.8,-1e5+170),    Vec3(0,0,0)    ,Vec3(0,0,0)      , DIFFUSE)   );//Front
    objects.push_back( Sphere(1e5, Vec3(50, 1e5, 81.6),       Vec3(0,0,0)    ,Vec3(.75,.75,.75), DIFFUSE)   );//Bottom
    objects.push_back( Sphere(1e5, Vec3(50,-1e5+81.6,81.6),   Vec3(0,0,0)    ,Vec3(.75,.75,.75), DIFFUSE)   );//Top
    objects.push_back( Sphere(16.5,Vec3(27,16.5,47),          Vec3(0,0,0)    ,Vec3(1,1,1)*.999 , MIRROR)    );//Mirr
    objects.push_back( Sphere(16.5,Vec3(73,16.5,78),          Vec3(0,0,0)    ,Vec3(1,1,1)*.999 , MIRROR)    );//Change to Glass

    objects.push_back( Light(5,   Vec3(50,70,50)) );//Light
    lights.push_back( 8 );

}

inline bool intersectScene(const Ray &r, double &t, int &id){
    double d, inf=t=1e20;
    bool intersect;
    for(int i= 0 ;i < objects.size(); ++i) {
        if((d=objects[i].intersects(r,intersect))&&d<t){
            t=d;
            id=i;
        }
    }
    return t<inf;
}

Vec3 radiance(const Ray &r, int depth){
    double hitDistance;                               
    int objectId=0;            
    if (!intersectScene(r, hitDistance, objectId)) return Vec3(); // if miss, return black

    const Object &obj = objects[objectId];        // the hit object

    Vec3 x=r.origin+r.direction*hitDistance,                  // the hit position
            n=(x-obj.position).normalize(),  // the normal of the object at the hit
            f=obj.color;                  // the color of the object at the hit

    if (++depth>5) return Vec3(); // we limit the number of rebounds in the scene

    if (obj.reflexion == EMMISSIVE){ // we hit a light
        return obj.emission;
    }
    if (obj.reflexion == DIFFUSE){                  // Ideal DIFFUSE reflection
        // We shoot rays towards all lights:
        Vec3 rad;
        for( unsigned int lightIt = 0 ; lightIt < lights.size() ; ++lightIt ) {
            const Object &light = objects[ lights[lightIt] ];
            Vec3 lightDirection = (light.randomSample() -x).normalize();
            int nbOfRays = 20;
            int comptIntersectFree = 0;
            for (unsigned int i = 0 ; i<nbOfRays ; i++){
                Ray rayToLight= Ray(x+0.001*lightDirection,lightDirection);
                int intersectId;
                double distance;
                intersectScene(rayToLight,distance, intersectId);
                if (intersectId == lights[lightIt] ){comptIntersectFree +=1;}
            }
            rad = rad + float(comptIntersectFree) / float(nbOfRays) * f.mult(light.emission)* fabs(n.dot(lightDirection));
        }
        int nbOfRays = 20;
        for (unsigned int i = 0 ; i<nbOfRays ; i++){
            Vec3 randomDirection = randomSampleOnHemisphere(n);
            Ray randomRay= Ray(x+0.001*randomDirection,randomDirection);
            int intersectId;
            double distance;
            depth+=1;

            rad = rad + radiance(randomRay,depth);
        }
        return rad;
    }
    else if (obj.reflexion == MIRROR) {           // Ideal SPECULAR reflection
        Vec3 rad;
        depth+=1;
        Vec3 R =  -2 * n *r.direction.dot(n) + r.direction;

        return radiance(Ray(x+0.001*R,R),depth).mult(f);
    }
    else if (obj.reflexion == GLASS) {           // Ideal SPECULAR reflection
        // TODO
        return Vec3();
    }


    return Vec3();
}

int main(int argc, char *argv[]){
    int w=1024, h=768, samps = argc==2 ? atoi(argv[1])/4 : 1; // # samples
    Ray cam(Vec3(50,52,295.6), Vec3(0,-0.042612,-1).normalize()); // camera center and direction
    Vec3 cx=Vec3(w*.5135/h), cy=(cx.cross(cam.direction)).normalize()*.5135, *pixelsColor=new Vec3[w*h];

    // setup scene:
    createScene();

    // ray trace:
    for (int y=0; y<h; y++) {                       // Loop over image rows
        fprintf(stderr,"\rRendering (%d spp) %5.2f%%",samps*4,100.*y/(h-1));
        for (unsigned short x=0; x<w; x++) {  // Loop cols
            Vec3 r(0,0,0);
            for( unsigned int sampleIt = 0 ; sampleIt < samps ; ++sampleIt ) {
                double dx = ((double)(rand())/RAND_MAX);
                double dy = ((double)(rand())/RAND_MAX);
                Vec3 d = cx*( ( x + dx )/w - .5) +
                        cy*( ( y + dy )/h - .5) + cam.direction;
                r = r + radiance(Ray(cam.origin+d*140,d.normalize()),0)*(1./samps);
            }

            pixelsColor[x + (h-1-y) * w] = pixelsColor[x + (h-1-y) * w] + Vec3(clamp(r.x),clamp(r.y),clamp(r.z));
        } // Camera rays are pushed ^^^^^ forward to start in interior
    }

    // save image:
    FILE *f = fopen("image.ppm", "w");         // Write image to PPM file.
    fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
    for (int i=0; i<w*h; i++)
        fprintf(f,"%d %d %d ", (int)(pixelsColor[i].x * 255), (int)(pixelsColor[i].y * 255), (int)(pixelsColor[i].z * 255));
    fclose(f);
}