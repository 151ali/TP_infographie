#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED
#include<iostream>
#include"transformation.h"
using namespace std;

class point{

    public:
        //
        double  x,y,z,w;
        ~point(){/*.*/};

        point(){
            x=0;y=0;z=0;w=1;
        };

        point(double a ,double b,double c){
            //
            this->x = a;
            this->y = b;
            this->z = c;
            this->w = 1;
        };

        point(double a ,double b){
            //
            this->x = a;
            this->y = b;
            this->z = 0;
            this->w = 1;
        };

        point(double *V){
            x = V[0];
            y = V[1];
            z = V[2];
            w = 1;
        };

        Vector nvect(){
            double norm = sqrt(x*x+y*y+z*z);
            return Vector(abs(x/norm),abs(y/norm),abs(z/norm));
        }

        Vector operator - (point p){
            return Vector(x-p.x,  y-p.y,  z-p.z);
        }


        Vector toVector(){
            return Vector(this->x, this->y, this->z);
        }


        void print(){cout <<"x = " <<x <<"| y = "<<y<<"| z = "<<z<<endl;};
        void Swap(point &p2){
            swap(this->x,p2.x);
            swap(this->y,p2.y);
            swap(this->z,p2.z);

        }

        // point*
        void transformer(transformation& trsf){
            double tx = trsf.mat[0][0] * x + trsf.mat[0][1] * y + trsf.mat[0][2] * z + trsf.mat[0][3] * w;
            double ty = trsf.mat[1][0] * x + trsf.mat[1][1] * y + trsf.mat[1][2] * z + trsf.mat[1][3] * w;
            double tz = trsf.mat[2][0] * x + trsf.mat[2][1] * y + trsf.mat[2][2] * z + trsf.mat[2][3] * w;
            double tw = trsf.mat[3][0] * x + trsf.mat[3][1] * y + trsf.mat[3][2] * z + trsf.mat[3][3] * w;


            this->x = tx / tw;
            this->y = ty / tw;
            this->z = tz / tw;
            this->w = 1;
            // return this
        };
};


class lookAt : public transformation {
public:

    point   eye;
    Vector  dir,
            up,
            x_cam,
            y_cam,
            z_cam;

    lookAt(){};

	lookAt(point eye, Vector dir, Vector up) {

		this->eye = eye;
		this->dir = dir;
		this->up = up;

		z_cam = -dir.normalize();

		x_cam = (up ^ z_cam).normalize();

		y_cam = z_cam ^ x_cam;

		double mat1[4][4];
		double mat2[4][4];


		mat1[0][0] = x_cam.x; mat1[0][1] = x_cam.y; mat1[0][2] = x_cam.z; mat1[0][3] = 0;
		mat1[1][0] = y_cam.x; mat1[1][1] = y_cam.y; mat1[1][2] = y_cam.z; mat1[1][3] = 0;
		mat1[2][0] = z_cam.x; mat1[2][1] = z_cam.y; mat1[2][2] = z_cam.z; mat1[2][3] = 0;
		mat1[3][0] = 0;       mat1[3][1] = 0;       mat1[3][2] = 0;       mat1[3][3] = 1;

		mat2[0][0] = 1; mat2[0][1] = 0; mat2[0][2] = 0; mat2[0][3] = -eye.x;
		mat2[1][0] = 0; mat2[1][1] = 1; mat2[1][2] = 0; mat2[1][3] = -eye.y;
		mat2[2][0] = 0; mat2[2][1] = 0; mat2[2][2] = 1; mat2[2][3] = -eye.z;
		mat2[3][0] = 0; mat2[3][1] = 0; mat2[3][2] = 0; mat2[3][3] = 1;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				mat[i][j] = 0;
				for (int k = 0; k < 4; k++) {
					mat[i][j] = mat[i][j] + mat1[i][k] * mat2[k][j];
				}
			}

	}

	virtual void print() {
		cout << "Je suis LookAt : " << endl;
		cout << "eye ";  eye.print();
		cout << "dir "; dir.print();
		cout << "up "; up.print();
		cout << "x_cam "; x_cam.print();
		cout << "y_cam "; y_cam.print();
		cout << "z_cam "; z_cam.print();
	}

};

#endif // POINT_H_INCLUDED
