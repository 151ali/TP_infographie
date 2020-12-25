
#include <cstdlib> //pour faire une pause sur l'affichage
#include <iostream>
#include <cmath>

#include <FreeImage.h>

using namespace std;

typedef float Vec2[2];
typedef float Vec3[3];
double pi =  3.1415926535897932384626433832795028841971693993751058209749445923;
int largeur = 800;
int hauteur = 800;

float z_buffer[800][800];

FIBITMAP* bitmap;
FIBITMAP* depth_buffer;
FIBITMAP* texture;

int tex_width, tex_height;

RGBQUAD color;


class Transformation {
public:

	Transformation() {
		mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0; mat[0][3] = 0;//premiere ligne
		mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0; mat[1][3] = 0;//deuxieme ligne
		mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1; mat[2][3] = 0;//troisieme ligne
		mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;//quatrieme ligne
	}

	virtual void print() {
		cout << "class generique : Identite" << endl;
	}

	Transformation* compose(Transformation Tr) { //compose_gauche

		float tmp[4][4];

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
	}

	float mat[4][4];
};

class Point {
public:
	Point() {
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}

	Point(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = 1;
	}

	Point(float* V) {
		x = V[0];
		y = V[1];
		z = V[2];
		w = 1;
	}

	Point* Transform(Transformation& trsf) {

		float tx = trsf.mat[0][0] * x + trsf.mat[0][1] * y + trsf.mat[0][2] * z + trsf.mat[0][3] * w;
		float ty = trsf.mat[1][0] * x + trsf.mat[1][1] * y + trsf.mat[1][2] * z + trsf.mat[1][3] * w;
		float tz = trsf.mat[2][0] * x + trsf.mat[2][1] * y + trsf.mat[2][2] * z + trsf.mat[2][3] * w;
		float tw = trsf.mat[3][0] * x + trsf.mat[3][1] * y + trsf.mat[3][2] * z + trsf.mat[3][3] * w;


		this->x = tx ;
		this->y = ty ;
		this->z = tz ;
		this->w = tw ;

		div_w();

		return this;
	}

	void div_w() {

		this->x /= this->w;
		this->y /= this->w;
		this->z /= this->w;
		this->w = 1;
	}

	float x;
	float y;
	float z;
	float w;

	void print() {

		cout << "x= " << x << " , y= " << y << " , z= " << z << " , w= " << w << endl;
	}

};

class Vector {
public:
	Vector() {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	Vector(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = 0;
	}

	Vector(Point p1, Point p2) {
		this->x = p2.x - p1.x;
		this->y = p2.y - p1.y;
		this->z = p2.z - p1.z;
		this->w = 0;
	}

	float x;
	float y;
	float z;
	float w;

	void print() {

		cout << "x= " << x << " , y= " << y << " , z= " << z << endl;
	}

	float norm() {
		return sqrt(x * x + y * y + z * z);
	}

	Vector normalize() {
		float norme = norm();
		return Vector(x / norme, y / norme, z / norme);
	}

	Vector operator- () {
		return Vector(-x, -y, -z);
	}

	Vector operator^ (Vector V) {

		double v1x = this->x, v1y = this->y, v1z = this->z;
		double v2x = V.x, v2y = V.y, v2z = V.z;

		return   Vector((v1y * v2z) - (v1z * v2y),
			(v1z * v2x) - (v1x * v2z),
			(v1x * v2y) - (v1y * v2x));

		/*return  Vector( -(v1y * v2z) + (v1z * v2y),
			-(v1z * v2x) + (v1x * v2z),
			-(v1x * v2y) + (v1y * v2x));*/

	}
};

class Rotation_Z : public Transformation {
public:

	Rotation_Z(float ang) {

		angle = ang / 180 * 3.14159;

		mat[0][0] = cosf(angle); mat[0][1] = -sinf(angle);  mat[0][2] = 0; mat[0][3] = 0;//premiere ligne
		mat[1][0] = sinf(angle); mat[1][1] = cosf(angle);  mat[1][2] = 0; mat[1][3] = 0;//deuxieme ligne
		mat[2][0] = 0;           mat[2][1] = 0;            mat[2][2] = 1; mat[2][3] = 0;//troisieme ligne
		mat[3][0] = 0;           mat[3][1] = 0;            mat[3][2] = 0; mat[3][3] = 1;//quatrieme ligne
	}
	float angle;

	virtual void print() {
		cout << " je suis une rotation, d'un angle " << angle << endl;
	}

};

class Rotation_X : public Transformation {
public:

	Rotation_X(float ang) {

		angle = ang / 180 * 3.14159;

		mat[0][0] = 1;  mat[0][1] = 0;  mat[0][2] = 0;            mat[0][3] = 0;//premiere ligne
		mat[1][0] = 0;  mat[1][1] = cosf(angle);  mat[1][2] = -sinf(angle);  mat[1][3] = 0;//deuxieme ligne
		mat[2][0] = 0;  mat[2][1] = sinf(angle);  mat[2][2] = cosf(angle);  mat[2][3] = 0;//troisieme ligne
		mat[3][0] = 0;  mat[3][1] = 0;            mat[3][2] = 0;            mat[3][3] = 1;//quatrieme ligne
	}
	float angle;

	virtual void print() {
		cout << " je suis une rotation, d'un angle " << angle << endl;
	}

};

class Rotation_Y : public Transformation {
public:

	Rotation_Y(float ang) {

		angle = ang / 180 * 3.14159;

		mat[0][0] = cosf(angle);  mat[0][1] = 0;  mat[0][2] = sinf(angle);  mat[0][3] = 0;//premiere ligne
		mat[1][0] = 0;            mat[1][1] = 1;  mat[1][2] = 0;            mat[1][3] = 0;//deuxieme ligne
		mat[2][0] = -sinf(angle); mat[2][1] = 0;  mat[2][2] = cosf(angle); mat[2][3] = 0;//troisieme ligne
		mat[3][0] = 0;            mat[3][1] = 0;  mat[3][2] = 0;            mat[3][3] = 1;//quatrieme ligne
	}
	float angle;

	virtual void print() {
		cout << " je suis une rotation, d'un angle " << angle << endl;
	}

};

class Translation : public Transformation {
public:

	Translation(Vector V) {

		mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0; mat[0][3] = V.x; //premiere ligne
		mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0; mat[1][3] = V.y; //deuxieme ligne
		mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1; mat[2][3] = V.z;   //troisieme ligne
		mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;   //troisieme ligne

		this->V = V;

	}

	virtual void print() {
		cout << " je suis une translation, d'un vecteur "; V.print();
	}

	Vector V;

};

class Scaling : public Transformation {
public:

	Scaling(float s) {

		mat[0][0] = s; mat[0][1] = 0; mat[0][2] = 0; mat[0][3] = 0; //premiere ligne
		mat[1][0] = 0; mat[1][1] = s; mat[1][2] = 0; mat[1][3] = 0; //deuxieme ligne
		mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = s; mat[2][3] = 0;   //troisieme ligne
		mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;   //troisieme ligne

		sx = sy = sz = s;

	}

	Scaling(float s_x, float s_y, float s_z) {

		mat[0][0] = s_x; mat[0][1] = 0;   mat[0][2] = 0;   mat[0][3] = 0; //premiere ligne
		mat[1][0] = 0;   mat[1][1] = s_y; mat[1][2] = 0;   mat[1][3] = 0; //deuxieme ligne
		mat[2][0] = 0;   mat[2][1] = 0;   mat[2][2] = s_z; mat[2][3] = 0;   //troisieme ligne
		mat[3][0] = 0;   mat[3][1] = 0;   mat[3][2] = 0;   mat[3][3] = 1;   //troisieme ligne

		sx = s_x;
		sy = s_y;
		sz = s_z;

	}

	virtual void print() {
		cout << " je suis un changement d'echelle , sx= " << sx << " sy= " << sy << " sz= " << sz<<endl;
	}

	float sx;
	float sy;
	float sz;


};

class Viewport : public Transformation {
public:

	Viewport(int n_x, int n_y) {

		mat[0][0] = float(n_x) /2.0; mat[0][1] = 0;                mat[0][2] = 0;  mat[0][3] = float(n_x - 1) / 2.0; //premiere ligne
		mat[1][0] = 0;               mat[1][1] = float(n_y) / 2.0; mat[1][2] = 0;  mat[1][3] = float(n_y - 1) / 2.0; //deuxieme ligne
		mat[2][0] = 0;               mat[2][1] = 0;                mat[2][2] = 1;  mat[2][3] = 0;   //troisieme ligne
		mat[3][0] = 0;               mat[3][1] = 0;                mat[3][2] = 0;  mat[3][3] = 1;   //troisieme ligne

		res_x = n_x;
		res_y = n_y;
	}

	virtual void print() {
		cout << " je suis Viewport res_x= " << res_x << "res_y= " << res_y << endl;
	}

	int res_x;
	int res_y;
};

class Perspective : public Transformation {
public:

	Perspective(float ang, float ratio, float n, float f) {

		float angle = ang / 180 * 3.14159;

		mat[0][0] = -1 / ratio/ tanf(angle / 2.0);  mat[0][1] = 0;                      mat[0][2] = 0;                 mat[0][3] = 0; //premiere ligne
		mat[1][0] = 0;                              mat[1][1] = -1 / tanf(angle / 2.0);  mat[1][2] = 0;                 mat[1][3] = 0; //deuxieme ligne
		mat[2][0] = 0;                              mat[2][1] = 0;                      mat[2][2] = (f + n) / (n - f); mat[2][3] = 2 * f * n / (f - n);   //troisieme ligne
		mat[3][0] = 0;                              mat[3][1] = 0;                      mat[3][2] = 1;                 mat[3][3] = 0;   //troisieme ligne

		this->angle = angle;
		this->ratio = ratio;
		this->n = n;
		this->f = f;
	}

	virtual void print() {
		cout << " je suis Perspective : angle= " << angle << " ratio= " << ratio << "n= " << n << "f= " << f << endl;
	}

	float angle; float ratio; float n; float f;
};

class LookAt : public Transformation {
public:

	LookAt(Point eye, Point centre, Vector up) {

		this->eye = eye;

		this->dir.x = centre.x - eye.x;
		this->dir.y = centre.y - eye.y;
		this->dir.z = centre.z - eye.z;

		this->up = up;

		z_cam = - dir.normalize();

		x_cam = (up ^ z_cam).normalize();

		y_cam = z_cam ^ x_cam;


		float mat1[4][4];
		float mat2[4][4];


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
		cout << "up " ; up.print();
		cout << "x_cam "; x_cam.print();
		cout << "y_cam "; y_cam.print();
		cout << "z_cam "; z_cam.print();
	}

	Point eye; Vector dir; Vector up; Vector x_cam; Vector  y_cam; Vector z_cam;
};

int invertmatrix(float mat [3][3], float invmat[3][3]) {

	int i, j;
	float determinant = 0;

	/*for (i = 0; i < 3; i++) {
		cout << "\n";

		for (j = 0; j < 3; j++)
			cout << mat[i][j] << "\t";
	}*/


	//finding determinant
	for (i = 0; i < 3; i++)
		determinant = determinant + (mat[0][i] * (mat[1][(i + 1) % 3] * mat[2][(i + 2) % 3] - mat[1][(i + 2) % 3] * mat[2][(i + 1) % 3]));

	//cout << "\n\ndeterminant: " << determinant;

	//cout << "\n\nInverse of matrix is: \n";

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			invmat[i][j] = ((mat[(j + 1) % 3][(i + 1) % 3] * mat[(j + 2) % 3][(i + 2) % 3]) - (mat[(j + 1) % 3][(i + 2) % 3] * mat[(j + 2) % 3][(i + 1) % 3])) / determinant ;
	}

	return 0;
}

float edgeFunction(Point& a, Point& b, Point& c){
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

void trace_tirangle(Point& v0, Point& v1, Point& v2, Vec3 c0, Vec3 c1, Vec3 c2) {

	float area = edgeFunction(v0, v1, v2);





	int   x_min = floor(min(min(v0.x,v1.x),v2.x)) - 1 ,
          x_max = floor(max(max(v0.x,v1.x),v2.x)) + 1 ,
          y_min = floor(min(min(v0.y,v1.y),v2.y)) - 1,
          y_max = floor(max(max(v0.y,v1.y),v2.y)) + 1;


for (int i = x_min; i < x_max; ++i) {
	for (int j = y_min; j < y_max; ++j) {


			Point p(i + 0.5f, j + 0.5f, 0);

			float w0 = edgeFunction(v1, v2, p);
			float w1 = edgeFunction(v2, v0, p);
			float w2 = edgeFunction(v0, v1, p);

			Vec2 edge0 = { v2.x - v1.x, v2.y - v1.y };
			Vec2 edge1 = { v0.x - v2.x, v0.y - v2.y };
			Vec2 edge2 = { v1.x - v0.x, v1.y - v0.y };

			/*bool overlaps = true;

			overlaps &= (w0 == 0 ? ((edge0[1] == 0 && edge0[0] > 0) || edge0[1] > 0) : (w0 > 0));
			overlaps &= (w1 == 0 ? ((edge1[1] == 0 && edge1[0] > 0) || edge1[1] > 0) : (w1 > 0));
			overlaps &= (w2 == 0 ? ((edge2[1] == 0 && edge2[0] > 0) || edge2[1] > 0) : (w2 > 0));

			if (overlaps) {*/

			if (w0 >= 0 && w1 >= 0 && w2 >= 0) {

				w0 /= area;
				w1 /= area;
				w2 /= area;

				float z_inv = w0 / v0.z + w1 / v1.z + w2 / v2.z;

				float z = 1.0 / z_inv;

				if (z > z_buffer[i][j]) {

					z_buffer[i][j] = z;

					color.rgbRed = (unsigned char)  ((1+z/20) * 255);
					color.rgbGreen = (unsigned char)((1+z/20)* 255);
					color.rgbBlue = (unsigned char) ((1+z/20) * 255);

					FreeImage_SetPixelColor(depth_buffer, i, j, &color);

					float r = w0 * c0[0] + w1 * c1[0] + w2 * c2[0];
					float g = w0 * c0[1] + w1 * c1[1] + w2 * c2[1];
					float b = w0 * c0[2] + w1 * c1[2] + w2 * c2[2];

					//Perspective correction
					float s= (w0 * c0[0]/ v0.z + w1 * c1[0]/ v1.z + w2 * c2[0]/ v2.z) * z;
					float t= (w0 * c0[1]/ v0.z + w1 * c1[1]/ v1.z + w2 * c2[1]/ v2.z) * z;

					//float s= (w0 * c0[0] + w1 * c1[0] + w2 * c2[0]);
					//float t= (w0 * c0[1] + w1 * c1[1] + w2 * c2[1]);

					color.rgbRed   = (unsigned char)(r * 255);
					color.rgbGreen = (unsigned char)(g * 255);
					color.rgbBlue  = (unsigned char)(b * 255);

					FreeImage_SetPixelColor(bitmap, i, j, &color);

					int u= floor((tex_width-1) * s ) ;
					int v= floor((tex_height-1)* t ) ;

					if (u > (tex_width-1) )  u = tex_width-1;
					if (v > (tex_height-1)) v = tex_height-1;

					if (u < 0 )  u = 0;
					if (v < 0 )  v = 0;

					//cout << "i="<< i << "  j=" << j <<endl;
					bool texturing =FreeImage_GetPixelColor(texture, u , v ,&color);
					if (texturing)
						{
				     	//cout << "texturing ..."<< endl;
					     FreeImage_SetPixelColor(bitmap, i, j, &color);
					}
					else cout << "Echec lecture texture" <<endl;
				}
			}
		}
	}
}

//TODO: A optimiser ...
int DDA(Point p1, Point p2, FIBITMAP* bitmap, RGBQUAD color) {

	Point start_p;
	Point end_p;

	if (p1.x <= p2.x)
	{
		start_p.x = p1.x;
		start_p.y = p1.y;

		end_p.x = p2.x;
		end_p.y = p2.y;

	}
	else {
		start_p.x = p2.x;
		start_p.y = p2.y;

		end_p.x = p1.x;
		end_p.y = p1.y;
	}

	//cout << "start ";  start_p.print();
	//cout << "end "; end_p.print();

	int delta_x = end_p.x - start_p.x;
	int delta_y = end_p.y - start_p.y;

	float m = (double)delta_y / (double)delta_x;


	FreeImage_SetPixelColor(bitmap, floor(start_p.x + 0.5), floor(start_p.y + 0.5), &color);

	if (delta_x == 0) {

		if (start_p.y < end_p.y)
			for (int y = start_p.y + 1; y < end_p.y; y++)
				FreeImage_SetPixelColor(bitmap, start_p.x, y, &color);
		else
			for (int y = end_p.y + 1; y < start_p.y; y++)
				FreeImage_SetPixelColor(bitmap, start_p.x, y, &color);

	}
	else if (delta_y == 0)
	{
		if (start_p.x < end_p.x)
			for (int x = start_p.x + 1; x < end_p.x; x++)
				FreeImage_SetPixelColor(bitmap, x, start_p.y, &color);
		else
			for (int x = end_p.x + 1; x < start_p.x; x++)
				FreeImage_SetPixelColor(bitmap, x, start_p.y, &color);


	}
	else if (abs(m) < 1) {

		//cout << "m < 1 " << endl;
		//cout << "m =  " << m << endl;

		float y = start_p.y;

		for (int x = start_p.x + 1; x < end_p.x; x++)
		{
			y = y + m;
			FreeImage_SetPixelColor(bitmap, x, floor(y + 0.5), &color);

		}
	}
	else {
		//cout << "m =  " << m << endl;
		float x = start_p.x;

		if (start_p.y < end_p.y)
			for (int y = start_p.y + 1; y < end_p.y; y++)
			{
				x = x + 1.0 / m;
				FreeImage_SetPixelColor(bitmap, floor(x + 0.5), y, &color);

			}
		else
			for (int y = start_p.y - 1; y > end_p.y; y--)
			{
				x = x - 1.0 / m;
				FreeImage_SetPixelColor(bitmap, floor(x + 0.5), y, &color);

			}

	}

	FreeImage_SetPixelColor(bitmap, floor(end_p.x + 0.5), floor(end_p.y + 0.5), &color);

	return 0;
}

void Dessiner_Quad(Point p1, Point p2, Point p3, Point p4, FIBITMAP* bitmap, RGBQUAD color) {
	DDA(p1, p2, bitmap, color);
	DDA(p2, p3, bitmap, color);
    DDA(p3, p4, bitmap, color);
	DDA(p4, p1, bitmap, color);
}

bool load_texture(const char* filename){

	texture=0;
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		texture = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if(!texture)
		return false;

	tex_width = FreeImage_GetWidth(texture);
	tex_height = FreeImage_GetHeight(texture);

	return true;
}
/// *******************************************************************************
/// *******************************************************************************
/// ************************** ** Main Function ** ********************************
/// *******************************************************************************

int sphere();
int cylindre();
int main() {
    //sphere();
    //cylindre();
}

/// *******************************************************************************
/// *******************************************************************************
/// *******************************************************************************
/// *******************************************************************************


int sphere() {

	FreeImage_Initialise();
	std::cout << "FreeImage " << FreeImage_GetVersion() << endl << endl;


	if (!load_texture("chess.png"))
	//if (!load_texture("earth.png"))
	//if (!load_texture("moon.jpg"))
	//if (!load_texture("mars.jpg"))
		return -1;
	cout << "tex_width="<< tex_width << ", tex_height= "<<tex_height <<endl;

	for (int j = 0; j < hauteur; ++j) {
		for (int i = 0; i < largeur; ++i) {
			z_buffer[i][j] = -INFINITY;
		}
	}


	int BPP = 24; //Bits par pixel (8 bits pour chaque caanal RGB

	bitmap = FreeImage_Allocate(largeur, hauteur, BPP);
	depth_buffer = FreeImage_Allocate(largeur, hauteur, BPP);

	LookAt cam(Point(-8, 0, 0), Point(0, 0, 0), Vector(0, 0, 1));
	Perspective persp(45, 1, -1 ,-10);
	Viewport viewport(largeur, hauteur);

	Scaling s(2, 1, 1);
	//Sera appliqu\E9e \E0 tous les sommets
	Transformation Tf, Tf_cam;
	Tf_cam.compose(s)->compose(cam);
	Tf.compose(persp)->compose(viewport);



    double pas = pi/20;
    int max_th  = 2*pi/pas+1,
        max_phi = pi/pas+1;
    Point sph[max_th][max_phi];


    int R = 2;


    for(int i=0;i<max_th;i++){
        for(int j=0;j<max_phi;j++){

            sph[i][j].x = (double)(R * cos (i * pas) * sin(j * pas));
            sph[i][j].y = (double)(R * sin (i * pas) * sin(j * pas));
            sph[i][j].z = (double)(R * cos (j * pas));

            sph[i][j].Transform(Tf_cam);
        }
    }

    Point p1,p2,p3,p4;
    for(int i=0;i<max_th;i++)
        for(int j=0;j<max_phi;j++){

            p1 = sph[i][j];
            p2 = sph[i][(j+1)%max_phi];
            p3 = sph[(i+1)%max_th][(j+1)%max_phi];
            p4 = sph[(i+1)%max_th][j];




            //Vec3 c0 = { 1,1,1};

            Vec3 tex1 = {i*pas/(2*pi),                 1-(j*pas)/(pi)                 ,-1};
            Vec3 tex2 = {i*pas/(2*pi),                 1-(((j+1)%max_phi)*pas)/(pi)   ,-1};
            Vec3 tex3 = {((i+1)%max_th)*pas/(2*pi),    1-(((j+1)%max_phi)*pas)/(pi)   ,-1};
            Vec3 tex4 = {((i+1)%max_th)*pas/(2*pi),    1-(j*pas)/(pi)                 ,-1};


            float z1 = p1.z,
                  z2 = p2.z,
                  z3 = p3.z,
                  z4 = p4.z;

            // apply perspective && view port
            p1.Transform(Tf);
            p2.Transform(Tf);
            p3.Transform(Tf);
            p4.Transform(Tf);

            //Save z in order to make the perspective correction

			p1.z= z1;
			p2.z= z2;
			p3.z= z3;
			p4.z= z4;

            Vector V1(p1, p2);
			Vector V2(p1, p4);
			Vector V = V1 ^ V2;


				if(V.z < 0) // Oui
			{
				//trace_tirangle(p1, p2, p3, c0, c0, c0);
				//trace_tirangle(p1, p3, p4, c0, c0, c0);

				trace_tirangle(p1, p2, p3, tex1, tex2, tex3);
				trace_tirangle(p1, p3, p4, tex1, tex3, tex4);
			}
			else { // Non : il faut inverser l'ordre de parcours
				//cout << "Inversion face  " <<i<<  endl;
				//trace_tirangle(p1, p4, p3, c0, c0, c0);
				//trace_tirangle(p1, p3, p2, c0, c0, c0);

				 trace_tirangle(p1, p4, p3, tex1, tex4, tex3);
				 trace_tirangle(p1, p3, p2, tex1, tex3, tex2);
			}

        }

	if (FreeImage_Save(FIF_PNG, bitmap, "sphere.png", 0)) {
		cout << "Image successfully saved!" << endl;
		FreeImage_Unload(bitmap);
	}

	if (FreeImage_Save(FIF_PNG, depth_buffer, "sphere_depth.png", 0)) {
		cout << "Image successfully saved!" << endl;
		FreeImage_Unload(depth_buffer);
	}

	FreeImage_Unload(texture);

	FreeImage_DeInitialise();

	//system("PAUSE");
	return 0;
}

/// *******************************************************************************
/// *******************************************************************************


int cylindre(){
	FreeImage_Initialise();
	std::cout << "FreeImage " << FreeImage_GetVersion() << endl << endl;

    if (!load_texture("chess.png"))
	//if (!load_texture("redbull.jpg"))
	//if (!load_texture("tex.jpg"))
	//if (!load_texture("tex.png"))
	//if (!load_texture("tex.jpg"))
    //if (!load_texture("Van_Goghs_Starry_Night.jpg"))
        	return -1;



	cout << "tex_width="<< tex_width << ", tex_height= "<<tex_height <<endl;

	for (int j = 0; j < hauteur; ++j) {
		for (int i = 0; i < largeur; ++i) {
			z_buffer[i][j] = -INFINITY;
		}
	}


	int BPP = 24; //Bits par pixel (8 bits pour chaque caanal RGB

	bitmap = FreeImage_Allocate(largeur, hauteur, BPP);
	depth_buffer = FreeImage_Allocate(largeur, hauteur, BPP);


    double pas_angle = pi/20;
    double pas_haut  = 2;
    int H = 5;

    int max_angle  = 2*pi/pas_angle+1,
        max_haut   = H / pas_haut+1;


    Point cyl[max_angle][max_haut];


    int R = 1;



	LookAt cam(Point(7, H/2, H/2), Point(0, 0, H/2), Vector(0, 0, 1));
	Perspective persp(45, 1, -1 ,-10);
	Viewport viewport(largeur, hauteur);

	Scaling s(1, 1, 1);
	//Sera appliqu\E9e \E0 tous les sommets
	Transformation Tf, Tf_cam;
	Tf_cam.compose(s)
	->compose(Rotation_Z(-150))
	->compose(cam);
	Tf.compose(persp)->compose(viewport);





    for(int i=0;i<max_angle;i++){
        for(int j=0;j<max_haut;j++){

            cyl[i][j].x = (double)(R * cos (i * pas_angle));
            cyl[i][j].y = (double)(R * sin (i * pas_angle));
            cyl[i][j].z = (double)(j * pas_haut);

            cyl[i][j].Transform(Tf_cam);
        }
    }

    Point p1,p2,p3,p4;
    for(int i=0;i<max_angle;i++)
        for(int j=0;j<max_haut;j++){

            p1 = cyl[i][j];
            p2 = cyl[i][(j+1)%max_haut];
            p3 = cyl[(i+1)%max_angle][(j+1)%max_haut];
            p4 = cyl[(i+1)%max_angle][j];




            Vec3 c0 = {1,1,1};

            Vec3 tex1 = {i*pas_angle/(2*pi),                    (j)*pas_haut/H                    ,-1};
            Vec3 tex2 = {i*pas_angle/(2*pi),                   ((j+1)%max_haut)*pas_haut/H                   ,-1};
            Vec3 tex3 = {((i+1)%max_angle)*pas_angle/(2*pi),   ((j+1)%max_haut)*pas_haut/H                   ,-1};
            Vec3 tex4 = {((i+1)%max_angle)*pas_angle/(2*pi),   (j)*pas_haut/H                     ,-1};


            float z1 = p1.z,
                  z2 = p2.z,
                  z3 = p3.z,
                  z4 = p4.z;

            // apply perspective && view port
            p1.Transform(Tf);
            p2.Transform(Tf);
            p3.Transform(Tf);
            p4.Transform(Tf);

            //Save z in order to make the perspective correction

			p1.z= z1;
			p2.z= z2;
			p3.z= z3;
			p4.z= z4;

            Vector V1(p1, p2);
			Vector V2(p1, p4);
			Vector V = V1 ^ V2;


				if(V.z < 0) // Oui
			{

				//trace_tirangle(p1, p2, p3, c0, c0, c0);
				//trace_tirangle(p1, p3, p4, c0, c0, c0);

				trace_tirangle(p1, p2, p3, tex1, tex2, tex3);
				trace_tirangle(p1, p3, p4, tex1, tex3, tex4);
			}
			else { // Non : il faut inverser l'ordre de parcours
				//cout << "Inversion face  " <<i<<  endl;
				//trace_tirangle(p1, p4, p3, c0, c0, c0);
				//trace_tirangle(p1, p3, p2, c0, c0, c0);

				trace_tirangle(p1, p4, p3, tex1, tex4, tex3);
                trace_tirangle(p1, p3, p2, tex1, tex3, tex2);
			}

        }

	if (FreeImage_Save(FIF_PNG, bitmap, "cylindre.png", 0)) {
		cout << "Image successfully saved!" << endl;
		FreeImage_Unload(bitmap);
	}

	if (FreeImage_Save(FIF_PNG, depth_buffer, "cylindre_depth.png", 0)) {
		cout << "Image successfully saved!" << endl;
		FreeImage_Unload(depth_buffer);
	}

	FreeImage_Unload(texture);

	FreeImage_DeInitialise();

	//system("PAUSE");
	return 0;
}








