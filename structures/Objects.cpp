#include "Math.cpp"

enum Refl_t { DIFFUSE, MIRROR, GLASS , EMMISSIVE };  // material types

class Object {
    public:
        Vec3 position;
        Vec3 color;
        Vec3 emission; 
        Refl_t reflexion;
        Object(Vec3 p, Vec3 c, Vec3 em, Refl_t refl) : position(p), color(c), emission(em), reflexion(refl) {};

    virtual double intersects(const Ray &r, bool& intersect) const {
        std::cout << "Vitual class called" << std::endl;
        return 0;
    }

    virtual Vec3 randomSample() const {
        std::cout << "Vitual class called" << std::endl;
        return Vec3();
    }

    
};

class Sphere : public Object {
    public: 
        double radius;       // radius
        Sphere(double rad, Vec3 pos, Vec3 em, Vec3 co, Refl_t refl): Object(pos, co, em, refl) ,radius(rad) {}

        double intersects(const Ray &r, bool& intersect) const override { // returns distance, 0 if nohit
            double M = (r.direction).dot(r.direction); // with mX²+nX + p = 0
            double N = 2.f * (r.direction).dot(r.origin - position);
            double P = (r.origin - position).dot(r.origin - position) - pow(radius,2);

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

        Vec3 randomSample() const override {
            return position + radius * randomSampleOnSphere();
        }
};

class Light : public Sphere {
    public: 
        Light(double rad, Vec3 pos): Sphere(rad, pos, Vec3(1,1,1), Vec3(0,0,0), EMMISSIVE) {}
};