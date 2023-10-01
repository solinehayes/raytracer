#include "Math.cpp"

struct Object {
    Vec3 position;
    Vec3 color;
    Object(Vec3 p) : position(p){}

    double intersects(const Ray &r, bool& intersect) const {return 0}
}

struct Sphere : Object{
    double radius;       // radius
    Sphere(Vec3 p, double rad): Object(p) , radius(rad) {}

    double intersects(const Ray &r, bool& intersect) const { // returns distance, 0 if nohit
        double M = (r.direction).dot(r.direction); // with mX²+nX + p = 0
        double N = 2.f * (r.direction).dot(r.origin - position);
        double P = (r.origin - p).dot(r.origin - position) - pow(radius,2);

        double discrim = pow(N,2) - 4.f * M * P ;

        if(discrim < 0){
            intersect = false;
            return 0.0;
        }
        else if (discrim == 0 ){
            intersect = true;

            return - N / (2.f * M) * (r.direction).dot(r.direction);
        }
        else {
            intersect = true;

            double distance1 = (-N - pow(discrim,0.5))/ (2.f*M) * (r.direction).dot(r.direction);
            double distance2 = (-N + pow(discrim,0.5))/ (2.f*M) * (r.direction).dot(r.direction);
            if (distance1>0) return distance1;
            if(distance2>0) return distance2;
        }
        return 0.0;
    }
};

struct Cube : Object {
    // TODO
    double size; 
    Cube(Vec3 p, double size_): Object(p) , size(size_) {}
    double intersects(const Ray &r, bool& intersect) const { // returns distance, 0 if nohit
       
    }
}

struct Plane : Object {
    Vec3 normal; 
    Cube(Vec3 p, double size_): Object(p) , size(size_) {}
    double intersects(const Ray &r, bool& intersect) const { // returns distance, 0 if nohit
       double condition = r.direction.dot(normal)

       if(condition != 0 ) {
            return (position - r.origin).dot(normal) / condition;
       }
       if((r.origin - position).dot(normal) == 0) {
        // Ray contained in plane
            return 0
       }
       // No intersection
       return 0
    }
}