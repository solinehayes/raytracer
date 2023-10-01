#ifndef VEC3_H
#define VEC3_H

struct Vec3 
{
    double x,y,z;
      Vec3 operator+(const Vec3 &b);

    Vec3 operator-(const Vec3 &b);

    Vec3 operator*(double b) const { return Vec3(x*b,y*b,z*b); }

    Vec3 mult(const Vec3 &b) const { return Vec3(x*b.x,y*b.y,z*b.z); }

    Vec3& normalize(){ return *this = *this * (1/sqrt(x*x+y*y+z*z)); }

    double dot(const Vec3 &b) const { return x*b.x+y*b.y+z*b.z; }

    Vec3 cross(Vec3&b){return Vec3(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);}
}

#endif