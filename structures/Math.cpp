struct Vec3 {
    double x, y, z;
    Vec3(double x_= 0, double y_=0 , double z_= 0){ x=x_; y=y_; z=z_; }

    Vec3 operator+(const Vec3 &b) const { return Vec3(x+b.x,y+b.y,z+b.z); }

    Vec3 operator-(const Vec3 &b) const { return Vec3(x-b.x,y-b.y,z-b.z); }

    Vec3 operator*(double b) const { return Vec3(x*b,y*b,z*b); }

    Vec3 mult(const Vec3 &b) const { return Vec3(x*b.x,y*b.y,z*b.z); }

    Vec3& normalize(){ return *this = *this * (1/sqrt(x*x+y*y+z*z)); }

    double dot(const Vec3 &b) const { return x*b.x+y*b.y+z*b.z; }

    Vec3 cross(Vec3&b){return Vec3(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);}
};

Vec3 operator*(double b , Vec3 const & o) { return Vec3(o.x*b,o.y*b,o.z*b); }


struct Ray {
    Vec3 origin, direction;
    Ray(Vec3 o, Vec3 d) : origin(o), direction(d) {}
};

