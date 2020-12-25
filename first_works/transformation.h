#ifndef TRANSFORMATION_H_INCLUDED
#define TRANSFORMATION_H_INCLUDED

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923
#include<iostream>
#include"vecteur.h"
#include"point.h"

using namespace std;

class transformation {
public:
    double mat[4][4];


	transformation() {
		mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0; mat[0][3] = 0;//premiere ligne
		mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0; mat[1][3] = 0;//deuxieme ligne
		mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1; mat[2][3] = 0;//troisieme ligne
		mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;//quatrieme ligne
	};

	virtual void print() {
		cout << "class generique : Identite" << endl;
	};

	transformation* compose(transformation Tr) { //compose_gauche

		double tmp[4][4];

		for (int i = 0; i < 4; i++)

			for (int j = 0; j < 4; j++)
			{
				tmp[i][j] = 0;
				for (int k = 0; k < 4; k++)
					tmp[i][j] += Tr.mat[i][k] * mat[k][j];
			}

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				mat[i][j] = tmp[i][j];

		return this;
	};


};






class rotation_Z : public transformation {
public:
    double angle;

	rotation_Z(double ang) {

		angle = ang / 180 * PI;

		mat[0][0] = cosf(angle); mat[0][1] = -sinf(angle);  mat[0][2] = 0; mat[0][3] = 0;//premiere ligne
		mat[1][0] = sinf(angle); mat[1][1] = cosf(angle);  mat[1][2] = 0; mat[1][3] = 0;//deuxieme ligne
		mat[2][0] = 0;           mat[2][1] = 0;            mat[2][2] = 1; mat[2][3] = 0;//troisieme ligne
		mat[3][0] = 0;           mat[3][1] = 0;            mat[3][2] = 0; mat[3][3] = 1;//quatrieme ligne
	};


	virtual void print() {
		cout << " je suis une rotation Z, d'un angle " << angle << endl;
	};

};


class rotation_X : public transformation {
public:
	double angle;

	rotation_X(double ang) {

		angle = ang / 180 * PI;

		mat[0][0] = 1;  mat[0][1] = 0;  mat[0][2] = 0;            mat[0][3] = 0;//premiere ligne
		mat[1][0] = 0;  mat[1][1] = cosf(angle);  mat[1][2] = -sinf(angle);  mat[1][3] = 0;//deuxieme ligne
		mat[2][0] = 0;  mat[2][1] = sinf(angle);  mat[2][2] = cosf(angle);  mat[2][3] = 0;//troisieme ligne
		mat[3][0] = 0;  mat[3][1] = 0;            mat[3][2] = 0;            mat[3][3] = 1;//quatrieme ligne
	};

	virtual void print() {
		cout << " je suis une rotation X, d'un angle " << angle << endl;
	};

};





class rotation_Y : public transformation {
public:
    double angle;

	rotation_Y(double ang) {

		angle = ang / 180 * 3.14159;

		mat[0][0] = cosf(angle);  mat[0][1] = 0;  mat[0][2] = sinf(angle);  mat[0][3] = 0;//premiere ligne
		mat[1][0] = 0;            mat[1][1] = 1;  mat[1][2] = 0;            mat[1][3] = 0;//deuxieme ligne
		mat[2][0] = -sinf(angle); mat[2][1] = 0;  mat[2][2] = cosf(angle); mat[2][3] = 0;//troisieme ligne
		mat[3][0] = 0;            mat[3][1] = 0;  mat[3][2] = 0;            mat[3][3] = 1;//quatrieme ligne
	};


	virtual void print() {
		cout << " je suis une rotation Y, d'un angle " << angle << endl;
	};

};




class translation : public transformation {
public:

	translation(Vector V) {

		mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0; mat[0][3] = V.x; //premiere ligne
		mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0; mat[1][3] = V.y; //deuxieme ligne
		mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1; mat[2][3] = V.z;   //troisieme ligne
		mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;   //troisieme ligne

		this->V = V;

	};

	virtual void print() {
		cout << " je suis une translation, d'un vecteur "; V.print();
	};

	Vector V;

};



class viewport : public transformation {
public:

	int res_x;
	int res_y;

	viewport(int n_x, int n_y) {

		mat[0][0] = double(n_x) / 2.0; mat[0][1] = 0;              mat[0][2] = 0; mat[0][3] = double(n_x - 1) / 2.0; //premiere ligne
		mat[1][0] = 0;               mat[1][1] = double(n_y) / 2.0; mat[1][2] = 0; mat[1][3] = double(n_y - 1) / 2.0; //deuxieme ligne
		mat[2][0] = 0;               mat[2][1] = 0;              mat[2][2] = 1; mat[2][3] = 0;   //troisieme ligne
		mat[3][0] = 0;               mat[3][1] = 0;              mat[3][2] = 0; mat[3][3] = 1;   //troisieme ligne

		res_x = n_x;
		res_y = n_y;
	};

	virtual void print() {
		cout << " je suis Viewport res_x= " << res_x << "res_y= " << res_y << endl;
	};

};




class perspective : public transformation {
public:
    float angle; float ratio; float n; float f;

	perspective(double ang, double ratio, double n, double f) {

		float angle = ang / 180 * PI;
        ///***  ici
		mat[0][0] = -1 / ( ratio * tanf(angle / 2.0)); mat[0][1] = 0;                  mat[0][2] = 0;           mat[0][3] = 0; //premiere ligne
		mat[1][0] = 0;                                 mat[1][1] = -1 / tanf(angle / 2.0);  mat[1][2] = 0;           mat[1][3] = 0; //deuxieme ligne
		mat[2][0] = 0;                                 mat[2][1] = 0;                      mat[2][2] = (f + n) / (n - f); mat[2][3] = 2 * f * n / (f - n);   //troisieme ligne
		mat[3][0] = 0;                                 mat[3][1] = 0;                      mat[3][2] = 1;           mat[3][3] = 0;   //troisieme ligne

		this->angle = angle;
		this->ratio = ratio;
		this->n = n;
		this->f = f;
	};

	virtual void print() {
		cout << " je suis Perspective : angle= " << angle << " ratio= " << ratio << "n= " << n << "f= " << f << endl;
	};

};


#endif // TRANSFORMATION_H_INCLUDED
