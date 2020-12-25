#ifndef VECTEUR_H_INCLUDED
#define VECTEUR_H_INCLUDED
#include<iostream>
#include<cmath>
#include "point.h"
using namespace std;

class Vector{
    public:
    //
        double x,y,z,w;

        Vector(){
            x=0;y=0;z=0;w=0;
        };

        Vector(double x, double y, double z) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = 0;
        }

        Vector(double x, double y) {
            this->x = x;
            this->y = y;
            this->z = 0;
            this->w = 0;
        }


        void print() {

            cout << "x= " << x << " , y= " << y << " , z= " << z << endl;
        }

        double norm() {
            return sqrt(x * x + y * y + z * z);
        }

        Vector normalize() {
            double norme = norm();
            return Vector(x / norme, y / norme, z / norme);
        }

        Vector operator - () {
            return Vector(-x, -y, -z);
        }

        Vector operator * (int k)  {
            return Vector(k * x, k * y, k * z);
        }

        Vector operator^ (Vector V) {

            double v1x = this->x, v1y = this->y, v1z = this->z;
            double v2x = V.x, v2y = V.y, v2z = V.z;

            return Vector((v1y * v2z) - (v1z * v2y),
                         (v1z * v2x) - (v1x * v2z),
                         (v1x * v2y) - (v1y * v2x));
        }

};

#endif // VECTEUR_H_INCLUDED
